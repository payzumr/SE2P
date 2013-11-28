/*
 * Dispatcher.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Dispatcher.h"

using namespace hal;

Dispatcher* Dispatcher::instance = NULL;
Mutex* Dispatcher::dispatcher_mutex = new Mutex();
Controller* controller = new Controller;

Dispatcher::Dispatcher() {
	HALSensorik* HALs = HALSensorik::getInstance();
	signalChid = HALs->getSignalChid();

}

Dispatcher::~Dispatcher() {
	// TODO Auto-generated destructor stub
}
Dispatcher* Dispatcher::getInstance() {
	dispatcher_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Dispatcher();
	}
	dispatcher_mutex->unlock();

	return instance;
}
void Dispatcher::shutdown() {

}
void Dispatcher::execute(void* arg) {
	controller->init();
	not_aus_reset = false;
	struct _pulse pulse;
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(signalChid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		setSensorChanges(pulse.code, pulse.value.sival_int);

	}
}

/**
 * @param code: get the Code from Pulse Message
 * @param val: get's the value from Pulse Message
 * Shows the State of the Sensor if any Sensor makes an Interrupt
 */
void Dispatcher::setSensorChanges(int code, int val) {
	MachineState* MState = MachineState::getInstance();
	HALSensorik* HALs = HALSensorik::getInstance();
	HALAktorik* HALa = HALAktorik::getInstance();
	if (!e_stop && !not_aus_reset) {
		if (code == SENSORS) {
			if (!(val & BIT_0) && !MState->SensEntry) {
				cout << "Werkstueck in Einlauf" << endl;
				MState->SensEntry = true;
				controller->entryStartSens();
			} else if ((val & BIT_0) && MState->SensEntry) {
				cout << "Werkstueck nicht mehr in Einlauf" << endl;
				MState->SensEntry = false;
				controller->exitStartSens();
			}
			if (!(val & BIT_1) && !MState->SensHeight) {
				cout << "Werkstueck in Hoehenmessung" << endl;
				MState->SensHeight = true;
				MState->height = HALs->getHeight();
				controller->entryHeightMessure();
			} else if ((val & BIT_1) && MState->SensHeight) {
				cout << "Werkstueck nicht mehr in Hoehenmessung" << endl;
				MState->SensHeight = false;
				controller->exitHeightMessure();
			}
			if (!(val & BIT_3) && !MState->SensSwitch) {
				cout << "Werkstueck in Weiche" << endl;
				MState->SensSwitch = true;
				controller->entrySwitch();
			} else if ((val & BIT_3) && MState->SensSwitch) {
				cout << "Werkstueck nicht mehr in Weiche" << endl;
				MState->SensSwitch = false;
				controller->exitSwitch();
			}
			if ((val & BIT_4) && !MState->SensMetall) {
				cout << "Werkstueck Metall" << endl;
				MState->SensMetall = true;
				controller->metalFound();
			} else if (!(val & BIT_4) && MState->SensMetall) {
				cout << "Metallwerkstueck hat messung verlassen" << endl;
				MState->SensMetall = false;
			}
			if ((val & BIT_5) && !MState->SwitchOpen) {
				cout << "Weiche offen" << endl;
				MState->SwitchOpen = true;
			} else if (!(val & BIT_5) && MState->SwitchOpen) {
				cout << "Weiche wieder zu" << endl;
				MState->SwitchOpen = false;
			}
			if (!(val & BIT_6) && !MState->SensSlip) {
				cout << "Rutsche ist voll" << endl;
				MState->SensSlip = true;
				controller->entrySlide();
			} else if ((val & BIT_6) && MState->SensSlip) {
				cout << "Rutsche nicht mehr voll" << endl;
				MState->SensSlip = false;
				controller->exitSlide();
			}
			if (!(val & BIT_7) && !MState->SensExit) {
				cout << "Werkstueck in Auslauf" << endl;
				MState->SensExit = true;
				controller->entryFinishSens();
			} else if ((val & BIT_7) && MState->SensExit) {
				cout << "Werkstueck nicht mehr in Auslauf" << endl;
				MState->SensExit = false;
			}
		} else if (code == BUTTONS) {
			if (val & START) {
				cout << "Starttaste gedrueckt" << endl;
				HALa->engine_start();
			}
			if (!(val & STOP)) {
				cout << "Stoptaste gedrueckt" << endl;
				HALa->engine_stop();
			}
			if ((val & RESET)) {
				cout << "Resettaste gedrueckt" << endl;
				controller->reset();
					}

			if (!(val & E_STOP) && !e_stop) {
				cout << "E-stop gedrueckt" << endl;
				controller->EStopPressed();
				e_stop = true;
			}

		}
	} else if (code == BUTTONS) {
		if ((val & RESET) && not_aus_reset) {
			cout << "Resettaste gedrueckt" << endl;
			not_aus_reset = false;
			controller->reset();
		}
		if ((val & E_STOP) && e_stop) {
			cout << "E-stop nicht mehr gedrueckt" << endl;
			e_stop = false;
			not_aus_reset = true;
		}
	}
}
