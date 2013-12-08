/*
 * Controller1.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Controller1.h"
#include "../HAW/HAWThread.h"
#include "../HAW/HWaccess.h"
#include "HALSensorik.h"
#include "HALAktorik.h"
#include "MachineState.h"
#include "Timer.h"
#include "Serial.h"
#include "LightControl.h"
using namespace hal;

Controller1* Controller1::instance = NULL;
Mutex* Controller1::controller1_mutex = new Mutex();
HALAktorik* HALa = HALAktorik::getInstance();
MachineState* Mstat = MachineState::getInstance();
thread::Timer* timer = thread::Timer::getInstance();

Controller1::Controller1() {
	ack = true;
}

Controller1::~Controller1() {
	delete instance;
	instance = NULL;
}

Controller1* Controller1::getInstance() {
	controller1_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Controller1();
	}
	controller1_mutex->unlock();

	return instance;
}

void Controller1::init() {
	numOfPuks = 0;
	pukPointer = 0;
	pukIdentifier = 0;
	int i = 0;
	for (; i < N_PUKS; i++) {
		pukArr[i].pukIdentifier = 0;
		pukArr[i].type = undefined;
		pukArr[i].place = CONVEYOROCCUPIED;
		pukArr[i].height1 = 0;
		pukArr[i].height2 = 0;
	}
}

/**
 * Reset Bitmask erstellen und diese in der HAL Aktorik implementieren!!!!
 *
 *
 */
void Controller1::reset() {
	timer->resetTimer();
	errorFlag = false;
	MachineState::getInstance()->stopLigth = true;
	HALAktorik::getInstance()->resetAktorik();
	HALAktorik::getInstance()->greenLigths(ON);
	cout << "Reset pusht! Machine is ready" << endl;
}

void Controller1::entryStartSens() {
	if (++numOfPuks == N_PUKS + 1) {

#ifdef DEBUG_MESSAGE
		cout << "Error found in entryStartSens()" << endl;
#endif
		errorFound();
	} else {
		pukArr[pukPointer].place = CONVEYOREMPTY;
		pukPointer = (pukPointer + 1) % N_PUKS;
		pukArr[pukPointer].pukIdentifier = pukIdentifier;
		pukIdentifier++;

		HALa->greenLigths(ON);
		HALa->engine_rigth();
		startConveyer();
	}
}
void Controller1::exitStartSens() {
	int puk = 0;
	while (pukArr[puk].place != CONVEYOREMPTY) {
		puk++;
	}
	pukArr[puk].place = CONVEYORBEGINNING;
	timer->setTimer(puk, (Mstat->entryToHeight_f + timer->slowTimer));//timerArr[puk] = Mstat->entryToHeight_f;
}

void Controller1::entryHeightMessure() {
	HALa->engine_slow(ON);
	int puk = 0;
	while (pukArr[puk].place != CONVEYORBEGINNING && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
#ifdef DEBUG_MESSAGE
			cout << "Error found in entryHeigthMessure()" << endl;
#endif
		}
	}

	if (!errorFlag) {
		timer->slowTimer = Mstat->inHeigthTime;
		timer->setTimer(puk, Mstat->inHeigthTime);//timerArr[puk] = Mstat->inHeigthTime;
		if (timer->switchTimer >= 0) {
			timer->switchTimer += timer->slowTimer;
		}
		timer->addSlowTime(puk);
		pukArr[puk].place = INHEIGHTMEASURE;

#ifdef SIMULATION
		if (Mstat->height >= 60000 && Mstat->height <= 70000) {
			pukArr[puk].place = HOLEPUKHM; //<-- anpassen manuell
		}
#endif
		if (Mstat->height >= 3400 && Mstat->height <= 3800) {
			pukArr[puk].place = HOLEPUKHM;
			pukArr[puk].type = withHole;
			pukArr[puk].height1 = Mstat->height;
		}
		if (Mstat->height >= 2800 && Mstat->height <= 3400) {
			pukArr[puk].place = FLATPUKHM;
			pukArr[puk].type = flat;
			pukArr[puk].height1 = Mstat->height;
		}
		if (Mstat->height >= 2000 && Mstat->height <= 2800) {
			pukArr[puk].place = NONHOLEPUKHM;
			pukArr[puk].type = tall;
			pukArr[puk].height1 = Mstat->height;
		}
	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in entryHeightMessure" << endl;
#endif
		errorFound();
	}
	printPuk(puk);
}

void Controller1::exitHeightMessure() {
	int puk = N_PUKS - 1;
	while ((pukArr[puk].place != HOLEPUKHM && pukArr[puk].place != NONHOLEPUKHM
			&& pukArr[puk].place != FLATPUKHM) && puk >= -1) {
		puk--;
	}
	timer->setTimer(puk, Mstat->heightToSwitch_f);
	HALa->engine_slow(OFF);
}

void Controller1::metalFound() {
	int puk = 0;
	while (pukArr[puk].place != HOLEPUKHM && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
#ifdef DEBUG_MESSAGE
			cout << "Error found in metalFound()" << endl;
#endif
		}
	}
	if (!errorFlag) {
		pukArr[puk].metall = true;
		pukArr[puk].type = withMetal;
	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in metalFound" << endl;
#endif
		errorFound();
	}
}

void Controller1::entrySlide() {
	int puk = 0;
	while (pukArr[puk].place != FLATPUKHM && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
#ifdef DEBUG_MESSAGE
			cout << "Error found in entrySlide()" << endl;
#endif
		}
	}
	printPuk(puk);
	if (!errorFlag) {
		resetPuk(puk);
		timer->slideTimer = SLIDE_TIME;
	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in entrySlide" << endl;
#endif
		errorFound();
	}
}
void Controller1::exitSlide() {
	timer->slideTimer = -1;
	bool conveyerEmpty = false;
	int puk = 0;
	while (pukArr[puk].place == CONVEYOROCCUPIED && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			conveyerEmpty = true;
		}
	}
	if (conveyerEmpty) {
		reset();
		stopConveyer();
	}
}

void Controller1::entrySwitch() {
	int puk = 0;
	while ((pukArr[puk].place != HOLEPUKHM && pukArr[puk].place != NONHOLEPUKHM
			&& pukArr[puk].place != FLATPUKHM) && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
#ifdef DEBUG_MESSAGE
			cout << "Error found in entrySwitch()" << endl;
#endif
		}
	}
	if (!errorFlag) {
		timer->setTimer(puk, -1);//timerArr[puk] = -1;
		if (pukArr[puk].place == HOLEPUKHM && pukArr[puk].type == withMetal) {
			pukArr[puk].place = INSWITCHNONHOLEMETAL;
			HALa->switchOnOff(ON);
			timer->switchTimer = SWITCH_OPEN_TIME;
		} else if (pukArr[puk].place == HOLEPUKHM) {
			pukArr[puk].place = INSWITCHNONMETAL;//auf 7 zurucksetzen!!!!!!!!!!!!!!!!!!!!!!!!!
			HALa->switchOnOff(ON);
			timer->switchTimer = SWITCH_OPEN_TIME;
		} else if (pukArr[puk].place == NONHOLEPUKHM) {
			pukArr[puk].place = INSWITCHNONHOLEMETAL;
			HALa->switchOnOff(ON);
			timer->switchTimer = SWITCH_OPEN_TIME;
		} else if (pukArr[puk].place == FLATPUKHM) {

		}
		printPuk(puk);

	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in entrySwitch" << endl;
#endif
		errorFound();
	}
}
void Controller1::exitSwitch() {
	int puk = N_PUKS - 1;
	while ((pukArr[puk].place != INSWITCHNONMETAL && pukArr[puk].place
			!= INSWITCHNONHOLEMETAL && pukArr[puk].place != FLATPUKHM) && puk
			>= -1) {
		puk--;
		if (puk == -1) {
			errorFlag = true;
#ifdef DEBUG_MESSAGE
			cout << "Error found in exitSwitch()" << endl;
#endif
		}
	}
	timer->setTimer(puk, Mstat->switchToExit_f + timer->slowTimer);//timerArr[puk] = Mstat->switchToExit_f;
	if (!errorFlag) {
		if (pukArr[puk].place == INSWITCHNONMETAL) {
			pukArr[puk].place = STARTEXITPARTHOLE;
		} else if (pukArr[puk].place == INSWITCHNONHOLEMETAL) {
			pukArr[puk].place = STARTEXITNONPARTHOLEMETAL;
		}
	} else {
		errorFound();
	}
}
void Controller1::entryFinishSens() {
	int puk = 0;
	while ((pukArr[puk].place != STARTEXITPARTHOLE && pukArr[puk].place
			!= STARTEXITNONPARTHOLEMETAL && pukArr[puk].place != TURNPLACECE)
			&& puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
#ifdef DEBUG_MESSAGE
			cout << "Error found in entryFinishSens()" << endl;
#endif
		}
	}
	printPuk(puk);
	if (!errorFlag) {
		timer->setTimer(puk, -1);//timerArr[puk] = -1;
		if (pukArr[puk].place == STARTEXITPARTHOLE) {
			pukArr[puk].place = EXIT;
			stopConveyer();

			handover(puk);

			startConveyer();

		} else if (pukArr[puk].place == TURNPLACECE) {
			timer->turnaroundTimer = -1;
			pukArr[puk].place = EXIT;
			stopConveyer();

			handover(puk);

			Mstat->turnAround = false;
			Mstat->stopLigth = true;
			HALa->greenLigths(ON);
			startConveyer();
		} else if (pukArr[puk].place == STARTEXITNONPARTHOLEMETAL) {
			stopConveyer();
			timer->turnaroundTimer = TURN_TIME;
			Mstat->turnAround = true;
			Mstat->yellow = true;
			HALa->greenLigths(OFF);
			pukArr[puk].place = TURNPLACECE;
		}
	} else {
		errorFound();
	}
}
void Controller1::exitFinishSens() {
	if (!Mstat->turnAround) {
		int puk = 0;
		while (pukArr[puk].place != WAITFORCONVEYOR2 && puk <= N_PUKS) {
			puk++;
			if (puk == N_PUKS) {
				errorFlag = true;
			}
		}
		printf("error is == %d\n", errorFlag);
		if (!errorFlag) {

			resetPuk(puk);
			cout << "huhu1" << endl;

			bool conveyerEmpty = false;
			int puk = 0;
			while (pukArr[puk].place == CONVEYOROCCUPIED && puk <= N_PUKS) {
				puk++;
				if (puk == N_PUKS) {
					conveyerEmpty = true;
				}
			}
			if (conveyerEmpty) {
				timer->endTimer = END_TIMER;
			}
		}
	}
}

void Controller1::handover(int puk) {
	pukArr[puk].place = WAITFORCONVEYOR2;
	ack = true;
	while (ack) {
		Serial::getInstance()->write_serial_puk(&pukArr[puk]);
	}
}
//tasten
void Controller1::EStopPressed() {
	Serial::getInstance()->write_serial_stop();
	stopConveyer();
	errorFlag = true;
	MachineState::getInstance()->redFast = true;
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	init();
	printf("Not-Aus gedrueckt! Band muss abgeraumt werden!\n");
}

void Controller1::errorFound() {
	stopConveyer();
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	MachineState::getInstance()->redFast = true;
	init();
	//ordentliche Fehlerausgaben
	printf("Fehler Aufgetreten! Band muss abgeraumt werden!\n");
}

void Controller1::printPuk(int puk) {
	printf("Puk:\n");
	printf("PukID: %d\n", pukArr[puk].pukIdentifier);
	printf("Stelle: %d\n", pukArr[puk].place);
	printf("Metall: %d\n", pukArr[puk].metall);
	printf("Hohe1: %d\n", pukArr[puk].height1);
}
void Controller1::stopConveyer() {
	HALa->engine_stop();
	Mstat->running = false;
}
void Controller1::startConveyer() {
	HALa->engine_start();
	Mstat->running = true;
}

void Controller1::resetPuk(int puk) {
	timer->setTimer(puk, -1);
	pukArr[puk].pukIdentifier = 0;
	pukArr[puk].type = undefined;
	pukArr[puk].place = CONVEYOROCCUPIED;
	pukArr[puk].height1 = 0;
	pukArr[puk].height2 = 0;
	pukArr[puk].metall = false;
}
