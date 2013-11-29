/*
 * Controller.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Controller.h"

using namespace hal;

HALAktorik* HALa = HALAktorik::getInstance();
MachineState* Mstat = MachineState::getInstance();
thread::Timer* timer = thread::Timer::getInstance();


//Mutex* Controller::Controller_mutex = new Mutex();
//Controller* Controller::instance = NULL;
Controller::Controller() {
	// TODO Auto-generated constructor stub

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}
//Controller* Controller::getInstance() {
//
//	Controller_mutex->lock();
//	if (instance == NULL) {
//		// Zugriffsrechte fuer den Zugriff auf die HW holen
//		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
//			perror("ThreadCtl access failed\n");
//			return NULL;
//		}
//		instance = new Controller();
//	}
//	Controller_mutex->unlock();
//	return instance;
//
//}

void Controller::init() {
	errorFlag = false;
	numOfPuks = 0;
	pukPointer = 0;
	pukIdentifier = 0;
	int i = 0;
	for (; i < N_PUKS; i++) {
		pukArr[i].pukIdentifier = 0;
		pukArr[i].type = undefined;
		pukArr[i].place = S0;
		pukArr[i].height1 = 0;
		pukArr[i].height2 = 0;
	}
}

void Controller::reset() {
	init();
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	HALa->redLigths(OFF);
	HALa->engine_slow(OFF);
	HALa->switchOnOff(OFF);
}

void Controller::entryStartSens() {
	if (++numOfPuks == N_PUKS + 1) {

		printf("Fehler in entryStartSens\n");
		errorFound();
	} else {
		pukArr[pukPointer].place = S1;
		pukPointer = (pukPointer + 1) % N_PUKS;
		pukArr[pukPointer].pukIdentifier = pukIdentifier;
		pukIdentifier++;

		HALa->greenLigths(ON);
		HALa->engine_rigth();
		HALa->engine_start();
	}
}
void Controller::exitStartSens() {
	int puk = 0;
	while (pukArr[puk].place != S1) {
		puk++;
	}
	pukArr[puk].place = S2;
	timer->timerArr[puk] = Mstat->entryToHeight_f;
	//	printPuk(puk);
}

void Controller::entryHeightMessure() {
	HALa->engine_slow(ON);
	int puk = 0;
	while (pukArr[puk].place != S2 && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}

	if (!errorFlag) {
	timer->timerArr[puk] = -1;
		pukArr[puk].place = S3;
		if (Mstat->height >= 3400 && Mstat->height <= 3800) {//flacher puk
			pukArr[puk].place = S4;
			pukArr[puk].type = withHole;
			pukArr[puk].height1 = Mstat->height;

		}
		if (Mstat->height >= 2700 && Mstat->height <= 2850) {//hoher puk
			pukArr[puk].place = S6;
			pukArr[puk].type = flat;
			pukArr[puk].height1 = Mstat->height;

		}
		if (Mstat->height >= 2000 && Mstat->height <= 2600) {//puk mit loch
			pukArr[puk].place = S5;
			pukArr[puk].type = tall;
			pukArr[puk].height1 = Mstat->height;
		}
	} else {

		printf("Fehler in entryHeightMessure\n");
		errorFound();
	}
	printPuk(puk);
}

void Controller::exitHeightMessure() {
	HALa->engine_slow(OFF);
#ifdef SIMULATION
	//HALa->switchOnOff(ON);
#endif
}

void Controller::metalFound() {
	int puk = 0;
	while (pukArr[puk].place != S4 && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		pukArr[puk].metall = true;
		pukArr[puk].type = withMetal;
	} else {

		printf("Fehler in metalFound\n");
		errorFound();
	}
}

void Controller::entrySlide() {
	int puk = 0;
	printPuk(puk);
	while (pukArr[puk].place != S6 && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		pukArr[puk].pukIdentifier = 0;
		pukArr[puk].type = undefined;
		pukArr[puk].place = S0;
		pukArr[puk].height1 = 0;
		pukArr[puk].height2 = 0;
		pukArr[puk].metall = false;
	} else {
		printf("Fehler in entrySlide\n");
		errorFound();
	}
}
void Controller::exitSlide() {
	bool conveyerEmpty = false;
	int puk = 0;
		while (pukArr[puk].place == S0 && puk <= N_PUKS) {
			puk++;
			if (puk == N_PUKS) {
				conveyerEmpty = true;
			}
		}
		if (conveyerEmpty) {
			reset();
			HALa->engine_stop();
		}
}

void Controller::entrySwitch() {
	int puk = 0;
	while ((pukArr[puk].place != S4 && pukArr[puk].place != S5
			&& pukArr[puk].place != S6) && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		if (pukArr[puk].place == S4 && pukArr[puk].type == withMetal) {
			pukArr[puk].place = S8;
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].place == S4) {
			pukArr[puk].place = S7;//auf 7 zurucksetzen!!!!!!!!!!!!!!!!!!!!!!!!!
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].place == S5) {
			pukArr[puk].place = S8;
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].place == S6) {

		}
		printPuk(puk);

	} else {
		printf("Fehler in entrySwitch\n");
		errorFound();
	}
}
void Controller::exitSwitch() {
	int puk = 0;
	while ((pukArr[puk].place != S7 && pukArr[puk].place != S8
			&& pukArr[puk].place != S6) && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		if (pukArr[puk].place == S7) {
			pukArr[puk].place = S9;
		} else if(pukArr[puk].place == S8){
			pukArr[puk].place = S10;
		}
	} else {
		printf("Fehler in exitSwitch\n");
		errorFound();
	}
	HALa->switchOnOff(OFF);
}
void Controller::entryFinishSens() {
Blinki* blink = Blinki::getInstance();
	int puk = 0;
	while ((pukArr[puk].place != S9 && pukArr[puk].place != S10) && puk
			<= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		if (pukArr[puk].place == S9) {
			pukArr[puk].place = S12;
			HALa->engine_stop();
			blink->start(NULL);
						sleep(10);
						blink->stop();
						blink->join();

		} else {
			HALa->engine_stop();
			HALa->greenLigths(OFF);
			blink->start(NULL);
			sleep(10);
			blink->stop();
			blink->join();

			pukArr[puk].place = S11;
		}
	} else {
		printf("Fehler in entryFinishSensor\n");
		errorFound();
	}
}

//tasten
void Controller::EStopPressed() {
	HALa->engine_stop();
	HALa->redLigths(ON);
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	init();
	printf("Not-Aus gedrueckt! Band muss abgeraumt werden!\n");

}

void Controller::errorFound() {
	HALa->engine_stop();
	HALa->redLigths(ON);
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	init();
	printf("Fehler Aufgetreten! Band muss abgeraumt werden!\n");
}

void Controller::printPuk(int puk) {
	printf("Puk:\n");
	printf("PukID: %d\n", pukArr[puk].pukIdentifier);
	printf("Stelle: %d\n", pukArr[puk].place);
	printf("Metall: %d\n", pukArr[puk].metall);
	printf("Hohe1: %d\n", pukArr[puk].height1);
}
