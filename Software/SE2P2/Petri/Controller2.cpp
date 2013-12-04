/*
 * Controller2.cpp
 *
 *  Created on: 04.12.2013
 *      Author: Jannik
 */

#include "Controller2.h"


Controller2* Controller2::instance = NULL;
Mutex* Controller2::controller2_mutex = new Mutex();
HALAktorik* HAL_A = HALAktorik::getInstance();
MachineState* MachineS = MachineState::getInstance();
thread::Timer* timerC2 = thread::Timer::getInstance();

Controller2::Controller2() {
	// TODO Auto-generated constructor stub

}

Controller2::~Controller2() {
	// TODO Auto-generated destructor stub
}


Controller2* Controller2::getInstance() {
	controller2_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Controller2();
	}
	controller2_mutex->unlock();

	return instance;
}


void Controller2::init() {
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

void Controller2::reset() {
	timerC2->resetTimer();
	init();
	HAL_A->greenLigths(OFF);
	HAL_A->yellowLigths(OFF);
	HAL_A->redLigths(OFF);
	HAL_A->engine_slow(OFF);
	HAL_A->switchOnOff(OFF);
}

void Controller2::entryStartSens() {
//	if (++numOfPuks == N_PUKS + 1) {
//
//#ifdef DEBUG_MESSAGE
//		cout << "Error found in entryStartSens()" << endl;
//#endif
//		errorFound();
//	} else {
//		pukArr[pukPointer].place = S1;
//		pukPointer = (pukPointer + 1) % N_PUKS;
//		pukArr[pukPointer].pukIdentifier = pukIdentifier;
//		pukIdentifier++;
//
//		HALa->greenLigths(ON);
//		HALa->engine_rigth();
//		HALa->engine_start();
//		Mstat->running = true;
//	}
}
void Controller2::exitStartSens() {
//	int puk = 0;
//	while (pukArr[puk].place != S1) {
//		puk++;
//	}
//	pukArr[puk].place = S2;
//	timer->setTimer(puk, (Mstat->entryToHeight_f + timer->slowTimer));//timerArr[puk] = Mstat->entryToHeight_f;
}

void Controller2::entryHeightMessure() {
//	HALa->engine_slow(ON);
//	int puk = 0;
//	while (pukArr[puk].place != S2 && puk <= N_PUKS) {
//		puk++;
//		if (puk == N_PUKS) {
//			errorFlag = true;
//#ifdef DEBUG_MESSAGE
//			cout << "Error found in entryHeigthMessure()" << endl;
//#endif
//		}
//	}
//
//	if (!errorFlag) {
//		timer->slowTimer = Mstat->inHeigthTime;
//		timer->setTimer(puk, Mstat->inHeigthTime);//timerArr[puk] = Mstat->inHeigthTime;
//		if(timer->switchTimer>=0){
//			timer->switchTimer += timer->slowTimer;
//		}
//		timer->addSlowTime(puk);
//		pukArr[puk].place = S3;
//
//#ifdef SIMULATION
//		if (Mstat->height >= 60000 && Mstat->height <= 70000) {
//			pukArr[puk].place = S4; //<-- anpassen manuell
//		}
//#endif
//
//		if (Mstat->height >= 3400 && Mstat->height <= 3800) {//flacher puk
//			pukArr[puk].place = S4;
//			pukArr[puk].type = withHole;
//			pukArr[puk].height1 = Mstat->height;
//
//		}
//		if (Mstat->height >= 2700 && Mstat->height <= 2850) {//hoher puk
//			pukArr[puk].place = S6;
//			pukArr[puk].type = flat;
//			pukArr[puk].height1 = Mstat->height;
//
//		}
//		if (Mstat->height >= 2000 && Mstat->height <= 2600) {//puk mit loch
//			pukArr[puk].place = S5;
//			pukArr[puk].type = tall;
//			pukArr[puk].height1 = Mstat->height;
//		}
//	} else {
//#ifdef DEBUG_MESSAGE
//		cout << "Fehler in entryHeightMessure" << endl;
//#endif
//		errorFound();
//	}
//	printPuk(puk);
}

void Controller2::exitHeightMessure() {
//	int puk = N_PUKS-1;
//	while ((pukArr[puk].place != S4 && pukArr[puk].place != S5
//			&& pukArr[puk].place != S6) && puk >= -1) {
//		puk--;
//	}
//	timer->setTimer(puk, Mstat->heightToSwitch_f);//timerArr[puk] = Mstat->heightToSwitch_f;
//	HALa->engine_slow(OFF);
}

void Controller2::metalFound() {
//	int puk = 0;
//	while (pukArr[puk].place != S4 && puk <= N_PUKS) {
//		puk++;
//		if (puk == N_PUKS) {
//			errorFlag = true;
//#ifdef DEBUG_MESSAGE
//			cout << "Error found in metalFound()" << endl;
//#endif
//		}
//	}
//	if (!errorFlag) {
//		pukArr[puk].metall = true;
//		pukArr[puk].type = withMetal;
//	} else {
//#ifdef DEBUG_MESSAGE
//		cout << "Fehler in metalFound" << endl;
//#endif
//		errorFound();
//	}
}

void Controller2::entrySlide() {
//	int puk = 0;
//	while (pukArr[puk].place != S6 && puk <= N_PUKS) {
//		puk++;
//		if (puk == N_PUKS) {
//			errorFlag = true;
//#ifdef DEBUG_MESSAGE
//			cout << "Error found in entrySlide()" << endl;
//#endif
//		}
//	}
//	printPuk(puk);
//	if (!errorFlag) {
//		timer->setTimer(puk, -1);
//		timer->slideTimer = SLIDE_TIME;
//		pukArr[puk].pukIdentifier = 0;
//		pukArr[puk].type = undefined;
//		pukArr[puk].place = S0;
//		pukArr[puk].height1 = 0;
//		pukArr[puk].height2 = 0;
//		pukArr[puk].metall = false;
//	} else {
//#ifdef DEBUG_MESSAGE
//		cout << "Fehler in entrySlide" << endl;
//#endif
//		errorFound();
//	}
}
void Controller2::exitSlide() {
//	timer->slideTimer = -1;
//	bool conveyerEmpty = false;
//	int puk = 0;
//	while (pukArr[puk].place == S0 && puk <= N_PUKS) {
//		puk++;
//		if (puk == N_PUKS) {
//			conveyerEmpty = true;
//		}
//	}
//	if (conveyerEmpty) {
//		reset();
//		HALa->engine_stop();
//		Mstat->running = false;
//	}
}

void Controller2::entrySwitch() {
//	int puk = 0;
//	while ((pukArr[puk].place != S4 && pukArr[puk].place != S5
//			&& pukArr[puk].place != S6) && puk <= N_PUKS) {
//		puk++;
//		if (puk == N_PUKS) {
//			errorFlag = true;
//#ifdef DEBUG_MESSAGE
//			cout << "Error found in entrySwitch()" << endl;
//#endif
//		}
//	}
//	if (!errorFlag) {
//		timer->setTimer(puk, -1);//timerArr[puk] = -1;
//		if (pukArr[puk].place == S4 && pukArr[puk].type == withMetal) {
//			pukArr[puk].place = S8;
//			HALa->switchOnOff(ON);
//			timer->switchTimer = SWITCH_OPEN_TIME;
//		} else if (pukArr[puk].place == S4) {
//			pukArr[puk].place = S7;//auf 7 zurucksetzen!!!!!!!!!!!!!!!!!!!!!!!!!
//			HALa->switchOnOff(ON);
//			timer->switchTimer = SWITCH_OPEN_TIME;
//		} else if (pukArr[puk].place == S5) {
//			pukArr[puk].place = S8;
//			HALa->switchOnOff(ON);
//			timer->switchTimer = SWITCH_OPEN_TIME;
//		} else if (pukArr[puk].place == S6) {
//
//		}
//		printPuk(puk);
//
//	} else {
//#ifdef DEBUG_MESSAGE
//		cout << "Fehler in entrySwitch" << endl;
//#endif
//		errorFound();
//	}
}
void Controller2::exitSwitch() {
//	int puk = N_PUKS-1;
//	while ((pukArr[puk].place != S7 && pukArr[puk].place != S8
//			&& pukArr[puk].place != S6) && puk >= -1) {
//		puk--;
//		if (puk == -1) {
//			errorFlag = true;
//#ifdef DEBUG_MESSAGE
//			cout << "Error found in exitSwitch()" << endl;
//#endif
//		}
//	}
//	timer->setTimer(puk, Mstat->switchToExit_f + timer->slowTimer);//timerArr[puk] = Mstat->switchToExit_f;
//	if (!errorFlag) {
//		if (pukArr[puk].place == S7) {
//			pukArr[puk].place = S9;
//		} else if (pukArr[puk].place == S8) {
//			pukArr[puk].place = S10;
//		}
//	} else {
//		errorFound();
//	}
}
void Controller2::entryFinishSens() {
//	Blinki* blink = Blinki::getInstance();
//	int puk = 0;
//	while ((pukArr[puk].place != S9 && pukArr[puk].place != S10) && puk
//			<= N_PUKS) {
//		puk++;
//		if (puk == N_PUKS) {
//			errorFlag = true;
//#ifdef DEBUG_MESSAGE
//			cout << "Error found in entryFinishSens()" << endl;
//#endif
//		}
//	}
//	if (!errorFlag) {
//		timer->setTimer(puk, -1);//timerArr[puk] = -1;
//		if (pukArr[puk].place == S9) {
//			pukArr[puk].place = S12;
//			HALa->engine_stop();
//			Mstat->running = false;
//			blink->start((void*)YELLOW);
//			sleep(10);
//			blink->stop();
//			blink->join();
//
//		} else {
//			HALa->engine_stop();
//			Mstat->running = false;
//			HALa->greenLigths(OFF);
//			pukArr[puk].place = S11;
//		}
//	} else {
//		errorFound();
//	}
}
//tasten
void Controller2::EStopPressed() {
	HAL_A->engine_stop();
	HAL_A->redLigths(ON);
	HAL_A->greenLigths(OFF);
	HAL_A->yellowLigths(OFF);
	init();
	printf("Not-Aus gedrueckt! Band muss abgeraumt werden!\n");
}

void Controller2::errorFound() {
	HAL_A->engine_stop();
	HAL_A->redLigths(ON);
	HAL_A->greenLigths(OFF);
	HAL_A->yellowLigths(OFF);
	init();
	printf("Fehler Aufgetreten! Band muss abgeraumt werden!\n");
}

void Controller2::printPuk(int puk) {
	printf("Puk:\n");
	printf("PukID: %d\n", pukArr[puk].pukIdentifier);
	printf("Stelle: %d\n", pukArr[puk].place);
	printf("Metall: %d\n", pukArr[puk].metall);
	printf("Hohe1: %d\n", pukArr[puk].height1);
}
