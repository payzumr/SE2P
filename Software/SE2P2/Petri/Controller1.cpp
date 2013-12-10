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
//Mutex* Controller1::controller1_mutex = new Mutex();
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
	//	controller1_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Controller1();
	}
	//	controller1_mutex->unlock();

	return instance;
}

void Controller1::init() {
	//	numOfPuks = 0;
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
	init();
	MachineState::getInstance()->sensEntry = false;
	HALAktorik::getInstance()->resetAktorik();
	HALAktorik::getInstance()->greenLigths(ON);

	cout << "Reset pusht! Machine is ready" << endl;
}

void Controller1::entryStartSens() {
	//	controller1_mutex->lock();
	timer->endTimer = -1;
	cout << "pukpointer " << pukPointer << endl;
	if (pukPointer == N_PUKS) {

#ifdef DEBUG_MESSAGE
		cout << "Error found in entryStartSens()" << endl;
#endif
		errorFound();
	} else {
		pukArr[pukPointer].place = CONVEYOREMPTY;
		pukArr[pukPointer].pukIdentifier = pukIdentifier;
		pukIdentifier++;
		pukPointer++;
		if (pukPointer == 1) {
			HALa->greenLigths(ON);
			HALa->engine_rigth();
			startConveyer();
		}
	}
	//	controller1_mutex->unlock();
}
void Controller1::exitStartSens() {
	//	controller1_mutex->lock();
	int puk = 0;
	while (pukArr[puk].place != CONVEYOREMPTY) {
		puk++;
	}
	pukArr[puk].place = CONVEYORBEGINNING;
	timer->setTimer(puk, (Mstat->entryToHeight_f + timer->slowTimer));//timerArr[puk] = Mstat->entryToHeight_f;
	printPuk(puk);
	//	controller1_mutex->unlock();
}

void Controller1::entryHeightMessure() {
	//	controller1_mutex->lock();
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
		if (Mstat->height >= 3200 && Mstat->height <= 3800) {
			pukArr[puk].place = HOLEPUKHM;
			pukArr[puk].type = withHole;
			pukArr[puk].height1 = Mstat->height;
		}
		if (Mstat->height >= 2800 && Mstat->height <= 3200) {
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
	//	controller1_mutex->unlock();
}

void Controller1::exitHeightMessure() {
	//	controller1_mutex->lock();
	int puk = N_PUKS - 1;
	while ((pukArr[puk].place != HOLEPUKHM && pukArr[puk].place != NONHOLEPUKHM
			&& pukArr[puk].place != FLATPUKHM) && puk >= -1) {
		puk--;
	}
	HALa->engine_slow(OFF);
	timer->setTimer(puk, Mstat->heightToSwitch_f);
	printPuk(puk);
	//	controller1_mutex->unlock();
}

void Controller1::metalFound() {
	//	controller1_mutex->lock();
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
	printPuk(puk);
	//	controller1_mutex->unlock();
}

void Controller1::entrySlide() {
	//	controller1_mutex->lock();
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
	if (!errorFlag) {
		cout << "puknummenr: " << puk << endl;
		resetPuk(puk);
		timer->slideTimer = SLIDE_TIME;
	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in entrySlide" << endl;
#endif
		errorFound();
	}
	printPuk(puk);
	//	controller1_mutex->unlock();
}
void Controller1::exitSlide() {
	//	controller1_mutex->lock();
	timer->slideTimer = -1;
	bool conveyerEmpty = false;
	int puk = 0;

	// hier fragen wir nicht nach dem flachen puk es kann also jeder rausgeschmissen werden und dann machen wir print puk und dann reset mit ner zahl die nicht geht

	while (pukArr[puk].place == CONVEYOROCCUPIED && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			conveyerEmpty = true;
		}
	}
	//resetPuk(puk);
	if (conveyerEmpty) {
		//reset();
		stopConveyer();
	}
	//printPuk(puk);
	//	controller1_mutex->unlock();
}

void Controller1::entrySwitch() {
	//	controller1_mutex->lock();
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
			HALa->switchOnOff(ON);
			usleep(80000);
			HALa->switchOnOff(OFF);
		}

	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in entrySwitch" << endl;
#endif
		errorFound();
	}
	printPuk(puk);
	//	controller1_mutex->unlock();
}
void Controller1::exitSwitch() {
	//	controller1_mutex->lock();
	int puk = N_PUKS - 1;

	//Hier wird als einziges der flache puk nicht bearbeitet
	while ((pukArr[puk].place != INSWITCHNONMETAL && pukArr[puk].place
			!= INSWITCHNONHOLEMETAL) && puk >= -1) {
		puk--;
	}
	if (puk == -1) {
		puk = 0;
		while (pukArr[puk].place != FLATPUKHM && puk <= N_PUKS) {
			puk++;
			if (puk == N_PUKS) {
				errorFlag = true;
#ifdef DEBUG_MESSAGE
				cout << "Error found in exitSwitch()" << endl;
#endif
			}
		}
	}

	// hier kann der timer auch auf den flachen puk draufgerechnet werden soll das so?

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
	printPuk(puk);
	//	controller1_mutex->unlock();
}
void Controller1::entryFinishSens() {
	//	controller1_mutex->lock();
	cout << "---------------------------------------------------------" << endl;
	for (int i = 0; i < N_PUKS; i++) {
		printPuk(i);
	}
	cout << "---------------------------------------------------------" << endl;
	stopConveyer();
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
	if (!errorFlag) {
		timer->setTimer(puk, -1);//timerArr[puk] = -1;
		if (pukArr[puk].place == STARTEXITPARTHOLE) {
			pukArr[puk].place = EXIT;
			//			stopConveyer();

			handover(puk);

			startConveyer();

		} else if (pukArr[puk].place == TURNPLACECE) {
			pukArr[puk].place = EXIT;
			//			stopConveyer();
			timer->turnaroundTimer = -1;
			Mstat->turnAround = false;
			Mstat->stopLigth = true;
			HALa->greenLigths(ON);

			handover(puk);

			startConveyer();
		} else if (pukArr[puk].place == STARTEXITNONPARTHOLEMETAL) {
			//			stopConveyer();
			timer->turnaroundTimer = TURN_TIME;
			Mstat->turnAround = true;
			Mstat->yellow = true;
			HALa->greenLigths(OFF);
			pukArr[puk].place = TURNPLACECE;
		}
	} else {
		errorFound();
		cout << "Error in entry Finsh Sens" << endl;
	}
	printPuk(puk);
	//	controller1_mutex->unlock();
}
void Controller1::exitFinishSens() {
	//	controller1_mutex->lock();
	if (!Mstat->turnAround) {
		int puk = 0;
		while (pukArr[puk].place != WAITFORCONVEYOR2 && puk <= N_PUKS) {
			puk++;
			if (puk == N_PUKS) {
				errorFlag = true;
				cout << "Error in entry Finsh Sens" << endl;
			}
		}
		if (!errorFlag) {

			resetPuk(puk);

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
		printPuk(puk);
	}
	//	controller1_mutex->unlock();
}

void Controller1::handover(int puk) {
	pukArr[puk].place = WAITFORCONVEYOR2;
	ack = true;
	while (ack) {
		Serial::getInstance()->write_serial_puk(&pukArr[puk]);
		usleep(500000);
	}
}
//tasten
void Controller1::EStopPressed() {
	//	controller1_mutex->lock();
	Serial::getInstance()->write_serial_stop();
	stopConveyer();
	errorFlag = true;
	MachineState::getInstance()->redFast = true;
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	init();
	printf("Not-Aus gedrueckt! Band muss abgeraumt werden!\n");
	//	controller1_mutex->unlock();
}

void Controller1::errorFound() {
	errorFlag = true;
	stopConveyer();
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	MachineState::getInstance()->redFast = true;
	init();
	//ordentliche Fehlerausgaben
	cout << "Fehler Aufgetreten! Band muss abgeraumt werden!" << endl;
}

void Controller1::printPuk(int puk) {
	printf("Puk:\n");
	printf("PukID: %d\n", pukArr[puk].pukIdentifier);
	printf("Stelle: %d\n", pukArr[puk].place);
	printf("Metall: %d\n", pukArr[puk].metall);
	printf("Hohe1: %d\n", pukArr[puk].height1);
	//printf("Pointer :%d\n", pukPointer);
	//printf("Größe: %d\n", sizeof(pukArr) / sizeof(struct puk));
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

	if (pukPointer > 0) {
		int i = puk;
		for (; i < pukPointer; i++) {
			pukArr[i] = pukArr[i + 1];
			timer->timerArr[i] = timer->timerArr[i + 1];
			if (i == pukPointer - 1) {
				timer->setTimer(i + 1, -1);
				pukArr[i + 1].pukIdentifier = 0;
				pukArr[i + 1].type = undefined;
				pukArr[i + 1].place = CONVEYOROCCUPIED;
				pukArr[i + 1].height1 = 0;
				pukArr[i + 1].height2 = 0;
				pukArr[i + 1].metall = false;
			}
			//letzten löschen????????????
		}
		pukPointer--;
	}

}

void Controller1::printPlace(int place) {
	switch (place) {
	case (0):
		cout << "ConveyerEmpty" << endl;
		break;
	case (1):
		cout << "ConveyorBeginnind" << endl;
		break;
	case (2):
		cout << "InHeightMeasure" << endl;
		break;
	case (3):
		cout << "HolePukHM" << endl;
		break;
	case (4):
		cout << "NonHolePukHM" << endl;
		break;
	case (5):
		cout << "FlatPukHM" << endl;
		break;
	case (6):
		cout << "InSwitchNonMetal" << endl;
		break;
	case (7):
		cout << "inSwitchNonHoleMetal" << endl;
		break;
	case (8):
		cout << "StartExitPartHole" << endl;
		break;
	case (9):
		cout << "StartExitNonPartHoleMetal" << endl;
		break;
	case (10):
		cout << "Turnplace 10" << endl;
		break;
	case (11):
		cout << "Exit" << endl;
		break;
	case (12):
		cout << "WaitForConveyor2" << endl;
		break;
	case (13):
		cout << "SlidePlace" << endl;
		break;
	case (14):
		cout << "HeigthMEasure" << endl;
		break;
	case (15):
		cout << "InSwitchHole" << endl;
		break;
	case (16):
		cout << "MetallNotOk" << endl;
		break;
	case (17):
		cout << "ErrorMarker" << endl;
		break;
	case (18):
		cout << "StartExitPart" << endl;
		break;
	case (19):
		cout << "ConveyorOccupied" << endl;
		break;
	case (20):
		cout << "InSwitchHole" << endl;
		break;
	default:
		cout << "No Place found" << endl;
	}
}
