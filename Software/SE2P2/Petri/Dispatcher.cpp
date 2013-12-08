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

MachineState* MState = MachineState::getInstance();

Dispatcher::Dispatcher() {
	signalChid = HALSensorik::getInstance()->getSignalChid();
}

Dispatcher::~Dispatcher() {
	delete instance;
	instance = NULL;
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
#ifdef BAND_1
	Controller1::getInstance()->init();
#endif
#ifdef BAND_2
	Controller2::getInstance()->init();
#endif

#ifdef DEBUG_MESSAGE
	cout << "Dispatcher gestartet" << endl;
#endif
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
#ifdef BAND_2
	Controller2* controller = Controller2::getInstance();
#endif
	if (MState->machineIsOn && !controller->errorFlag) {
		if (code == SENSORS) {
			if (!(val & BIT_0) && !MState->sensEntry) {
#ifdef DEBUG_MESSAGE
				cout << "D:Werkstueck in Einlauf" << endl;
#endif
				MState->sensEntry = true;
				controller->entryStartSens();
			} else if ((val & BIT_0) && MState->sensEntry) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Einlauf" << endl;
#endif
				MState->sensEntry = false;
				controller->exitStartSens();
			}
			if (!(val & BIT_1) && !MState->sensHeight) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck in Hoehenmessung" << endl;
#endif
				MState->height = HALSensorik::getInstance()->getHeight();
				MState->sensHeight = true;
				controller->entryHeightMessure();
			} else if ((val & BIT_1) && MState->sensHeight) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Hoehenmessung" << endl;
#endif
				MState->sensHeight = false;
				controller->exitHeightMessure();
			}
			if (!(val & BIT_3) && !MState->sensSwitch) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck in Weiche" << endl;
#endif
				MState->sensSwitch = true;
				controller->entrySwitch();
			} else if ((val & BIT_3) && MState->sensSwitch) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Weiche" << endl;
#endif
				MState->sensSwitch = false;
				controller->exitSwitch();
			}
			if ((val & BIT_4) && !MState->sensMetall) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck Metall" << endl;
#endif
				MState->sensMetall = true;
				controller->metalFound();
			} else if (!(val & BIT_4) && MState->sensMetall) {
#ifdef DEBUG_MESSAGE
				cout << "Metallwerkstueck hat messung verlassen" << endl;
#endif
				MState->sensMetall = false;
			}
			if ((val & BIT_5) && !MState->switchOpen) {
#ifdef DEBUG_MESSAGE
				cout << "Weiche offen" << endl;
#endif
				MState->switchOpen = true;
			} else if (!(val & BIT_5) && MState->switchOpen) {
#ifdef DEBUG_MESSAGE
				cout << "Weiche wieder zu" << endl;
#endif
				MState->switchOpen = false;
			}
			if (!(val & BIT_6) && !MState->sensSlip) {
#ifdef DEBUG_MESSAGE
				cout << "Rutsche ist voll" << endl;
#endif
				MState->sensSlip = true;
				controller->entrySlide();
			} else if ((val & BIT_6) && MState->sensSlip) {
#ifdef DEBUG_MESSAGE
				cout << "Rutsche nicht mehr voll" << endl;
#endif
				MState->sensSlip = false;
				controller->exitSlide();
			}
			if (!(val & BIT_7) && !MState->sensExit) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck in Auslauf" << endl;
#endif
				MState->sensExit = true;
				controller->entryFinishSens();
			} else if ((val & BIT_7) && MState->sensExit) {
#ifdef DEBUG_MESSAGE
				cout << "Werkstueck nicht mehr in Auslauf" << endl;
#endif
				controller->exitFinishSens();
				MState->sensExit = false;
			}
		} else if (code == BUTTONS) {
			if (!(val & STOP)) {
#ifdef DEBUG_MESSAGE
				cout << "Stoptaste gedrueckt" << endl;
#endif
				HALAktorik::getInstance()->resetAktorik();
				controller->reset();
				MState->machineIsOn = false;
			}
			if (!(val & E_STOP)) {
#ifdef DEBUG_MESSAGE
				cout << "E-stop gedrueckt" << endl;
#endif

				controller->EStopPressed();
				e_stop = true;
			}
		}
	} else if (!MState->machineIsOn && (code == BUTTONS)) {

		if (val & START) {
#ifdef DEBUG_MESSAGE
			cout << "Starttaste gedrueckt" << endl;
#endif
			HALAktorik::getInstance()->greenLigths(ON);
			MState->machineIsOn = true;
		}
	} else if (MState->machineIsOn && controller->errorFlag && !e_stop
			&& (code == BUTTONS)) {

		if ((val & RESET)) {
#ifdef DEBUG_MESSAGE
			cout << "Resettaste gedrueckt" << endl;
#endif
			controller->reset();
		}
	} else if (MState->machineIsOn && !e_stop && (code == BUTTONS)) {

		if (!(val & E_STOP)) {
#ifdef DEBUG_MESSAGE
			cout << "E-stop gedrueckt" << endl;
#endif
			controller->EStopPressed();
			e_stop = true;
		}
	} else if (MState->machineIsOn && e_stop && (code == BUTTONS)) {
		if ((val & E_STOP)) {
#ifdef DEBUG_MESSAGE
			cout << "E-stop nicht mehr gedrueckt" << endl;
#endif
			e_stop = false;
		}
	} else if (MState->machineIsOn && (code == BUTTONS)) {
		if (!(val & STOP)) {
#ifdef DEBUG_MESSAGE
			cout << "Stoptaste gedrueckt" << endl;
#endif
			HALAktorik::getInstance()->resetAktorik();
			controller->reset();
			MState->machineIsOn = false;
		}
	}
#ifdef DEBUG_TIMER
	//printf("Maschine ist : %d\n", MState->machineIsOn);
	// Timer::getInstance()->showTimeArray();
#endif
}
