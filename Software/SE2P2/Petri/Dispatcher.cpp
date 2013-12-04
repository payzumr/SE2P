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
#include "HALAktorik.h"

using namespace thread;
using namespace hal;



Dispatcher* Dispatcher::instance = NULL;
Mutex* Dispatcher::dispatcher_mutex = new Mutex();
//Controller* controller = new Controller();

MachineState* MState = MachineState::getInstance();

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

	cout << "shutdown the Dispatcher..." << endl;

}
void Dispatcher::execute(void* arg) {
//#ifdef BAND_1
	Controller1* controller = Controller1::getInstance();
//#endif
#ifdef BAND_2
	Controller2* controller = Controller2::getInstance();
#endif
	printf("hallo hier bin ich\n");
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
#ifdef BAND_1
	Controller1* controller = Controller1::getInstance();
#endif
	HALAktorik* HALa = HALAktorik::getInstance();
	HALSensorik* HALs = HALSensorik::getInstance();
	Timer* tim = Timer::getInstance();

	if (!e_stop && !not_aus_reset) {
		if (code == SENSORS) {
			if (!(val & BIT_0) && !MState->SensEntry) {
#ifdef DEBUG_MESSAGE
				cout << "D:Werkstueck in Einlauf" << endl;
#endif
				MState->SensEntry = true;
				controller->entryStartSens();
			} else if ((val & BIT_0) && MState->SensEntry) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Einlauf" << endl;
#endif
				MState->SensEntry = false;
				controller->exitStartSens();
			}
			if (!(val & BIT_1) && !MState->SensHeight) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck in Hoehenmessung" << endl;
#endif
				MState->SensHeight = true;
				MState->height = HALs->getHeight();
				controller->entryHeightMessure();
			} else if ((val & BIT_1) && MState->SensHeight) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Hoehenmessung" << endl;
#endif
				MState->SensHeight = false;
				controller->exitHeightMessure();
			}
			if (!(val & BIT_3) && !MState->SensSwitch) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck in Weiche" << endl;
#endif
				MState->SensSwitch = true;
				controller->entrySwitch();
			} else if ((val & BIT_3) && MState->SensSwitch) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Weiche" << endl;
#endif
				MState->SensSwitch = false;
				controller->exitSwitch();
			}
			if ((val & BIT_4) && !MState->SensMetall) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck Metall" << endl;
#endif
				MState->SensMetall = true;
				controller->metalFound();
			} else if (!(val & BIT_4) && MState->SensMetall) {
#ifdef DEBUG_MESSAGE
				cout << "Metallwerkstueck hat messung verlassen" << endl;
#endif
				MState->SensMetall = false;
			}
			if ((val & BIT_5) && !MState->SwitchOpen) {
#ifdef DEBUG_MESSAGE
				cout << "Weiche offen" << endl;
#endif
				MState->SwitchOpen = true;
			} else if (!(val & BIT_5) && MState->SwitchOpen) {
#ifdef DEBUG_MESSAGE
				cout << "Weiche wieder zu" << endl;
#endif
				MState->SwitchOpen = false;
			}
			if (!(val & BIT_6) && !MState->SensSlip) {
#ifdef DEBUG_MESSAGE
				cout << "Rutsche ist voll" << endl;
#endif
				MState->SensSlip = true;
				controller->entrySlide();
			} else if ((val & BIT_6) && MState->SensSlip) {
#ifdef DEBUG_MESSAGE
				cout << "Rutsche nicht mehr voll" << endl;
#endif
				MState->SensSlip = false;
				controller->exitSlide();
			}
			if (!(val & BIT_7) && !MState->SensExit) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck in Auslauf" << endl;
#endif
				MState->SensExit = true;
				controller->entryFinishSens();
			} else if ((val & BIT_7) && MState->SensExit) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Auslauf" << endl;
#endif
				MState->SensExit = false;
			}
		} else if (code == BUTTONS) {
			if (val & START) {
#ifdef DEBUG_MESSAGE
				cout << "Starttaste gedrueckt" << endl;
#endif
				HALa->engine_start();
			}
			if (!(val & STOP)) {
#ifdef DEBUG_MESSAGE
				cout << "Stoptaste gedrueckt" << endl;
#endif
				HALa->engine_stop();
			}
			if ((val & RESET)) {
#ifdef DEBUG_MESSAGE
				cout << "Resettaste gedrueckt" << endl;
#endif
				controller->reset();
			}

			if (!(val & E_STOP) && !e_stop) {
#ifdef DEBUG_MESSAGE
				cout << "E-stop gedrueckt" << endl;
#endif
				controller->EStopPressed();
				e_stop = true;
			}

		}
	} else if (code == BUTTONS) {
		if ((val & RESET) && not_aus_reset) {
#ifdef DEBUG_MESSAGE
			cout << "Resettaste gedrueckt" << endl;
#endif
			not_aus_reset = false;
			controller->reset();
		}
		if ((val & E_STOP) && e_stop) {
#ifdef DEBUG_MESSAGE
			cout << "E-stop nicht mehr gedrueckt" << endl;
#endif
			e_stop = false;
			not_aus_reset = true;
		}
	}
#ifdef DEBUG_TIMER
	//tim->showTimeArray();
#endif
}
