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
	portB_0 = false;
	portB_1 = false;
	portB_3 = false;
	portB_4 = false;
	portB_5 = false;
	portB_6 = false;
	portB_7 = false;
	portC_6 = false;
	portC_7 = false;
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
	//printf("portC7 %d\n", portC_7);
	if (!portC_7 && !not_aus_reset) {
		if (code == 2) {
			if (((val & BIT_0) == 0) && !portB_0) {
				cout << "Werkstueck in Einlauf" << endl;
				MState->SensEntry = true;
				controller->entryStartSens();
				portB_0 = true;
			} else if ((val & BIT_0) && portB_0) {
				cout << "Werkstueck nicht mehr in Einlauf" << endl;
				MState->SensEntry = false;
				controller->exitStartSens();
				portB_0 = false;
			}
			if (((val & BIT_1) == 0) && !portB_1) {
				cout << "Werkstueck in Hoehenmessung" << endl;
				//printf("AD PORT: %d \n",getHeight());
				MState->SensHeight = true;
				MState->height = HALs->getHeight();
				controller->entryHeightMessure();
				portB_1 = true;
			} else if ((val & BIT_1) && portB_1) {
				cout << "Werkstueck nicht mehr in Hoehenmessung" << endl;
				MState->SensHeight = false;
				controller->exitHeightMessure();
				portB_1 = false;
			}
			//		if ((val & BIT_2) && !portB_2) {
			//			cout << "Werkstueck im Toleranzbereich" << endl;
			//			portB_2 = true;
			//		} else if (((val & BIT_2) == 0) && portB_2) {
			//			cout << "Werkstueck nicht im Toleranzbereich" << endl;
			//			portB_2 = false;
			//		}
			if (((val & BIT_3) == 0) && !portB_3) {
				cout << "Werkstueck in Weiche" << endl;
				MState->SensSwitch = true;
				controller->entrySwitch();
				portB_3 = true;
			} else if ((val & BIT_3) && portB_3) {
				cout << "Werkstueck nicht mehr in Weiche" << endl;
				MState->SensSwitch = false;
				controller->exitSwitch();
				portB_3 = false;
			}
			if ((val & BIT_4) && !portB_4) {
				cout << "Werkstueck Metall" << endl;
				MState->SensMetall = true;
				controller->metalFound();
				portB_4 = true;
			} else if (((val & BIT_4) == 0) && portB_4) {
				cout << "Metallwerkstueck hat messung verlassen" << endl;
				MState->SensMetall = false;
				portB_4 = false;
			}
			if ((val & BIT_5) && !portB_5) {
				cout << "Weiche offen" << endl;
				MState->SwitchOpen = true;
				portB_5 = true;
			} else if (((val & BIT_5) == 0) && portB_5) {
				cout << "Weiche wieder zu" << endl;
				MState->SwitchOpen = false;
				portB_5 = false;
			}
			if (((val & BIT_6) == 0) && !portB_6) {
				cout << "Rutsche ist voll" << endl;
				MState->SensSlip = true;
				controller->printPuk(0);
				controller->printPuk(1);
				portB_6 = true;
			} else if ((val & BIT_6) && portB_6) {
				cout << "Rutsche nicht mehr voll" << endl;
				MState->SensSlip = false;
				portB_6 = false;
			}
			if (((val & BIT_7) == 0) && !portB_7) {
				cout << "Werkstueck in Auslauf" << endl;
				MState->SensExit = true;
				controller->entryFinishSens();
				portB_7 = true;
			} else if ((val & BIT_7) && portB_7) {
				cout << "Werkstueck nicht mehr in Auslauf" << endl;
				MState->SensExit = false;
				portB_7 = false;
			}

		} else if (code == 8) {
			if (val & BIT_4) {
				cout << "Starttaste gedrueckt" << endl;
				//			portC_4 = true;
			}
			//		 else if (((val & BIT_4) == 0) && portC_4) {
			//			cout << "Starttaste losgelassen" << endl;
			//			portC_4 = false;
			//		}
			if ((val & BIT_5) == 0) {
				cout << "Stoptaste gedrueckt" << endl;
				//			portC_5 = true;
			}
			//		 else if ((val & BIT_5) && portC_5) {
			//			cout << "Stoptaste losgelassen" << endl;
			//			portC_5 = false;
			//		}

			if (((val & BIT_7) == 0) && !portC_7) {
				cout << "E-stop gedrueckt" << endl;
				controller->EStopPressed();
				portC_7 = true;
			}

		}
	} else if (code == 8) {
		if ((val & BIT_6) && not_aus_reset) {
			cout << "Resettaste gedrueckt" << endl;
			not_aus_reset = false;
			controller->reset();
			//			portC_6 = true;
		}
		//		 else if (((val & BIT_6) == 0) && portC_6) {
		//			cout << "Resettaste losgelassen" << endl;
		//			portC_6 = false;
		//		}
		if ((val & BIT_7) && portC_7) {
			cout << "E-stop nicht mehr gedrueckt" << endl;
			portC_7 = false;
			not_aus_reset = true;
		}
	}
}
