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

using namespace thread;
using namespace hal;

Initialisation* Initialisation::instance = NULL;
Mutex* Initialisation::Initialisation_mutex = new Mutex();

Initialisation::Initialisation() {
	HALSensorik* HALs = HALSensorik::getInstance();
	signalChid = HALs->getSignalChid();

}

Initialisation::~Initialisation() {

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
	HALSensorik* HALs = HALSensorik::getInstance();
	Timer* timr = Timer::getInstance();
	HALAktorik* HALa = HALAktorik::getInstance();
	if (code == SENSORS) {
		if (!(val & BIT_0) && !MState->SensEntry) {
#ifdef DEBUG
			cout << "Werkstueck in Einlauf" << endl;
#endif
			MState->SensEntry = true;
			HALa->engine_rigth();
			HALa->engine_start();
		} else if ((val & BIT_0) && MState->SensEntry) {
			testzeitD = timr->testzeit;
#ifdef DEBUG
			cout << "Werkstueck nicht mehr in Einlauf" << endl;
#endif
			MState->SensEntry = false;
		}
		if (!(val & BIT_1) && !MState->SensHeight) {
			MState->entryToHeight_f = timr->testzeit - testzeitD;
			testzeitD = timr->testzeit;

			HALa->engine_slow(ON);
#ifdef DEBUG
			cout << "Werkstueck in Hoehenmessung" << endl;
#endif
			MState->SensHeight = true;
		} else if ((val & BIT_1) && MState->SensHeight) {
			MState->inHeigthTime = timr->testzeit - testzeitD;
			HALa->engine_slow(OFF);
			testzeitD = timr->testzeit;

#ifdef DEBUG
			cout << "Werkstueck nicht mehr in Hoehenmessung" << endl;
#endif
			MState->SensHeight = false;
		}
		if (!(val & BIT_3) && !MState->SensSwitch) {
				MState->heightToSwitch_f = timr->testzeit - testzeitD;
			HALa->switchOnOff(ON);
#ifdef DEBUG
			cout << "Werkstueck in Weiche" << endl;
#endif
			MState->SensSwitch = true;
		} else if ((val & BIT_3) && MState->SensSwitch) {
			testzeitD = timr->testzeit;
#ifdef DEBUG
			cout << "Werkstueck nicht mehr in Weiche" << endl;
#endif
			MState->SensSwitch = false;
			HALa->switchOnOff(OFF);
		}
		if (!(val & BIT_7) && !MState->SensExit) {
			MState->switchToExit_f = timr->testzeit - testzeitD;
			MState->imLauf = false;
#ifdef DEBUG
			MState->showTimes();
			cout << "Werkstueck in Auslauf" << endl;
#endif
			HALa->engine_stop();
			MState->SensExit = true;
		} else if ((val & BIT_7) && MState->SensExit) {
#ifdef DEBUG
			cout << "Werkstueck nicht mehr in Auslauf" << endl;
#endif
			MState->SensExit = false;

		}
	}
}
