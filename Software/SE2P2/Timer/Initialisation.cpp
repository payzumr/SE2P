/*
 * Initialisation.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Initialisation.h"
#include "HALAktorik.h"
#include "HALSensorik.h"

using namespace thread;
using namespace hal;

Initialisation* Initialisation::instance = NULL;
Mutex* Initialisation::Initialisation_mutex = new Mutex();

Initialisation::Initialisation() {
	HALSensorik* HALs = HALSensorik::getInstance();
	signalChid = HALs->getSignalChid();

}

Initialisation::~Initialisation() {
	delete instance;
	instance = NULL;

}
Initialisation* Initialisation::getInstance() {
	Initialisation_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Initialisation();
	}
	Initialisation_mutex->unlock();

	return instance;
}
void Initialisation::shutdown() {
	cout << "shutdown Initialisation..." << endl;

}
void Initialisation::execute(void* arg) {
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
void Initialisation::setSensorChanges(int code, int val) {
	MachineState* MState = MachineState::getInstance();
	Timer* timr = Timer::getInstance();
	HALAktorik* HALa = HALAktorik::getInstance();
	if (code == SENSORS) {
		if (!(val & BIT_0) && !MState->sensEntry) {
#ifdef DEBUG_MESSAGE
			cout << "Werkstueck in Einlauf" << endl;
#endif
			MState->sensEntry = true;
			HALa->engine_rigth();
			HALa->engine_start();
		} else if ((val & BIT_0) && MState->sensEntry) {
			testzeitD = timr->testzeit;
#ifdef DEBUG_MESSAGE
			cout << "Werkstueck nicht mehr in Einlauf" << endl;
#endif
			MState->sensEntry = false;
		}
		if (!(val & BIT_1) && !MState->sensHeight) {
			printf("hohe: %d\n", HALSensorik::getInstance()->getHeight());
			MState->entryToHeight_f = timr->testzeit - testzeitD;
#ifdef SIMULATION
			MState->entryToHeight_f += 200;
#endif
			MState->entryToHeight_f += 300;
			testzeitD = timr->testzeit;
			if (MState->initRound) {
				HALa->engine_slow(ON);
			}
#ifdef DEBUG_MESSAGE
			cout << "Werkstueck in Hoehenmessung" << endl;
#endif
			MState->sensHeight = true;
		} else if ((val & BIT_1) && MState->sensHeight) {
			if (!MState->initRound) {
				MState->heightFast = timr->testzeit - testzeitD;
			} else {
				MState->heightSlow = timr->testzeit - testzeitD;
				MState->inHeigthTime = timr->testzeit - testzeitD;
#ifdef SIMULATION
				MState->inHeigthTime += 200;
#endif
				MState->inHeigthTime += 300;
				MState->slowTimeAdd = MState->heightSlow - MState->heightFast;
			}
			HALa->engine_slow(OFF);
			testzeitD = timr->testzeit;

#ifdef DEBUG_MESSAGE
			cout << "Werkstueck nicht mehr in Hoehenmessung" << endl;
#endif
			MState->sensHeight = false;
		}
		if (!(val & BIT_3) && !MState->sensSwitch) {
			MState->heightToSwitch_f = timr->testzeit - testzeitD;
#ifdef SIMULATION
			MState->heightToSwitch_f += 200;
#endif
			MState->heightToSwitch_f += 300;
			HALa->switchOnOff(ON);
#ifdef DEBUG_MESSAGE
			cout << "Werkstueck in Weiche" << endl;
#endif
			MState->sensSwitch = true;
		} else if ((val & BIT_3) && MState->sensSwitch) {
			testzeitD = timr->testzeit;
#ifdef DEBUG_MESSAGE
			cout << "Werkstueck nicht mehr in Weiche" << endl;
#endif
			MState->sensSwitch = false;
		}
		if (!(val & BIT_7) && !MState->sensExit) {

			MState->switchToExit_f = timr->testzeit - testzeitD;

			HALa->switchOnOff(OFF);
#ifdef SIMULATION
			MState->switchToExit_f += 200;
#endif
			MState->switchToExit_f += 300;
#ifdef DEBUG_MESSAGE
			MState->showTimes();
			cout << "Werkstueck in Auslauf" << endl;
#endif

			HALa->engine_stop();
			MState->sensExit = true;
			MState->showTimes();
		} else if ((val & BIT_7) && MState->sensExit) {
#ifdef DEBUG_MESSAGE
			cout << "Werkstueck nicht mehr in Auslauf" << endl;
#endif
			if (MState->initRound) {
				MState->dispatcherGo = false;
				Initialisation::getInstance()->stop();
			}
			MState->sensExit = false;
			MState->initRound = true;

		}
	}
	MState->goingBackTimer = MState->entryToHeight_f + MState->heightToSwitch_f;
}
