/*
 * Controller2.cpp
 *
 *  Created on: 04.12.2013
 *      Author: Jannik
 */

#include "Controller2.h"
#include "../HAW/HAWThread.h"
#include "../HAW/HWaccess.h"
#include "HALSensorik.h"
#include "HALAktorik.h"
#include "MachineState.h"
#include "Timer.h"
#include "Serial.h"

using namespace hal;
Controller2* Controller2::instance = NULL;
Mutex* Controller2::controller2_mutex = new Mutex();
HALAktorik* HAL_A = HALAktorik::getInstance();
MachineState* MachineS = MachineState::getInstance();
thread::Timer* timerC2 = thread::Timer::getInstance();

#define PUK 0

Controller2::Controller2() {
	ack = true;
}

Controller2::~Controller2() {
	delete instance;
	instance = NULL;
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
	resetPuk(PUK);
}
void Controller2::reset() {
	//	MachineS->machineIsOn = true;
	timerC2->resetTimer();
	init();
	errorFlag = false;
	MachineState::getInstance()->stopLigth = true;
	HALAktorik::getInstance()->resetAktorik();
	HALAktorik::getInstance()->greenLigths(ON);
	cout << "Reset pusht! Machine is ready" << endl;
}
void Controller2::entryStartSens() {
	static int nummer = 1;
	cout << "Werkstuecknummer: " << nummer << endl;
	nummer++;
	timerC2->setTimer(PUK, -1);

	if (pukArr[PUK].type == withMetal) {
		pukArr[PUK].metall = true;
	}
	if (pukArr[PUK].place == CONVEYOREMPTY) {
		HAL_A->greenLigths(ON);

	} else if (pukArr[PUK].place == METALLNOTOK) {
		if (!MachineS->turnAround) {
			stopConveyer();
			MachineS->yellow = true;
			MachineS->turnAround = true;
			timerC2->turnaroundTimer = TURN_TIME;
		} else {
			timerC2->turnaroundTimer = -1;
			pukArr[PUK].place = CONVEYOREMPTY;
			MachineS->goingBack = false;
			MachineS->stopLigth = true;
			MachineS->turnAround = false;
			HAL_A->engine_rigth();
			startConveyer();
		}
	} else {
#ifdef DEBUG_MESSAGE
		cout << "Error found in entryStartSens()" << endl;
#endif
		errorFound();
	}
	printPlace(pukArr[PUK].place);
}
void Controller2::exitStartSens() {
	if (!MachineS->turnAround) {

		pukArr[PUK].place = CONVEYORBEGINNING;
		timerC2->setTimer(PUK, MachineS->entryToHeight_f);
	}
	printPlace(pukArr[PUK].place);
}

void Controller2::entryHeightMessure() {
	if (!MachineS->goingBack) {

		HAL_A->engine_slow(ON);

		if (pukArr[PUK].place == CONVEYORBEGINNING) {
			timerC2->setTimer(PUK, MachineS->inHeigthTime);

			pukArr[PUK].place = HEIGTHMEASURE;

#ifdef SIMULATION
			if (MachineS->height >= 60000 && MachineS->height <= 70000) {
				pukArr[PUK].place = S4; //<-- anpassen manuell
			}
#endif
			if (MachineS->height >= 3000 && MachineS->height <= 3800) {
				pukArr[PUK].place = HOLEPUKHM;
				pukArr[PUK].type = withHole;
				pukArr[PUK].height2 = MachineS->height;

			}
			if (MachineS->height >= 2000 && MachineS->height <= 2800) {
				pukArr[PUK].place = NONHOLEPUKHM;
				pukArr[PUK].type = tall;
				pukArr[PUK].height2 = MachineS->height;
			}
		} else {
#ifdef DEBUG_MESSAGE
			cout << "Fehler in entryHeightMessure" << endl;
#endif
			errorFound();
		}
		//		printPuk(PUK);
	} else {
		//ignore
	}
	printPlace(pukArr[PUK].place);
}
void Controller2::exitHeightMessure() {
	if (!MachineS->goingBack) {

		timerC2->setTimer(PUK, MachineS->heightToSwitch_f);
		HAL_A->engine_slow(OFF);
	} else {
		//ignore
	}
}
void Controller2::metalFound() {
	if (!MachineS->isMetal) { //????????? Wieso fragst du metall ab wenn du es nirgendswo anders gebrauchst?
		stopConveyer();
		//			HALAktorik::getInstance()->engine_left();
		usleep(200000);
		//Wei§t du noch welche ausgaben kamen?
		//wir kšnnen am anfang nochmal den componenten test laufen lassen ob links auch funktioniert
		if (pukArr[PUK].place == HOLEPUKHM) {
			pukArr[PUK].metall = true;
			pukArr[PUK].type = withMetal;
			pukArr[PUK].place = METALLNOTOK;
			MachineS->goingBack = true;
			MachineS->isMetal = true;
			HALAktorik::getInstance()->engine_left();
			startConveyer();
			timerC2->setTimer(PUK, MachineS->goingBackTimer);
		} else {
#ifdef DEBUG_MESSAGE
			cout << "Fehler in metalFound" << endl;
#endif
			errorFound();
		}
	} else {
		MachineS->sortOut = true;

		//aussortieren;

	}
	printPlace(pukArr[PUK].place);
}
void Controller2::entrySlide() {
	if (MachineS->sortOut) {
		resetPuk(PUK);
		timerC2->slideTimer = SLIDE_TIME;
		MachineS->sortOut = false;
	} else {
#ifdef DEBUG_MESSAGE
		cout << "Fehler in entrySlide" << endl;
#endif
		errorFound();
	}

}
void Controller2::exitSlide() {
	timerC2->slideTimer = -1;
	stopConveyer();
	printPlace(pukArr[PUK].place);
}
void Controller2::entrySwitch() {
	timerC2->setTimer(PUK, -1);
	if (!MachineS->sortOut) {
		//		if (!MachineS->isMetal) {

		if (pukArr[PUK].place == NONHOLEPUKHM) {
			pukArr[PUK].place = INSWITCHNONHOLE;
			HAL_A->switchOnOff(ON);
			timerC2->switchTimer = SWITCH_OPEN_TIME;
		} else if (pukArr[PUK].place == HOLEPUKHM) {
			pukArr[PUK].place = INSWITCHHOLE;
			HAL_A->switchOnOff(ON);
			timerC2->switchTimer = SWITCH_OPEN_TIME;
		} else {

#ifdef DEBUG_MESSAGE
			cout << "Fehler in entrySwitch" << endl;
#endif
			errorFound();
		}
		if (pukArr[PUK].metall == true) {
			pukArr[PUK].type = withMetal;
		}
	} else {
		HALAktorik::getInstance()->switchOnOff(ON);
		usleep(100000);
		HALAktorik::getInstance()->switchOnOff(OFF);
	}
	printPlace(pukArr[PUK].place);
}
void Controller2::exitSwitch() {
	if (!MachineS->sortOut) {
		timerC2->setTimer(PUK, MachineS->switchToExit_f);
		if (pukArr[PUK].place == INSWITCHHOLE || pukArr[PUK].place
				== INSWITCHNONHOLE) {
			pukArr[PUK].place = STARTEXITPART;
		} else {
#ifdef DEBUG_MESSAGE
			cout << "Error found in exitSwitch()" << endl;
#endif
			errorFound();
		}
		//		}
	} else {
		//ignore
	}
	printPlace(pukArr[PUK].place);
}
void Controller2::entryFinishSens() {
	timerC2->setTimer(PUK, -1);
	if (pukArr[PUK].place == STARTEXITPART) {
		pukArr[PUK].place = EXIT;
		stopConveyer();
		printPuk(PUK);

	} else {
#ifdef DEBUG_MESSAGE
		cout << "Error found in entryFinishSens()" << endl;
#endif
		errorFound();
	}
	printPlace(pukArr[PUK].place);
}

void Controller2::exitFinishSens() {
	resetPuk(PUK);
	printPlace(pukArr[PUK].place);
}

//tasten
void Controller2::EStopPressed() {
	Serial::getInstance()->write_serial_stop();
	stopConveyer();
	errorFlag = true;
	MachineState::getInstance()->redFast = true;
	HAL_A->greenLigths(OFF);
	HAL_A->yellowLigths(OFF);
	init();
	printf("Not-Aus gedrueckt! Band muss abgeraumt werden!\n");
}

void Controller2::errorFound() {
	errorFlag = true;
	stopConveyer();
	HAL_A->greenLigths(OFF);
	HAL_A->yellowLigths(OFF);
	MachineState::getInstance()->redFast = true;
	init();
	printf("Fehler Aufgetreten! Band muss abgeraumt werden!\n");
}

void Controller2::printPuk(int puk) {
	printf("Puk:\n");
	printf("PukID: %d\n", pukArr[puk].pukIdentifier);
	printf("Hohe1: %d\n", pukArr[puk].height1);
	printf("Hohe2: %d\n", pukArr[puk].height2);
	printf("Typ: ");
	(pukArr[puk].type == withHole) ? printf("with Hole\n") :(pukArr[puk].type == tall)?  printf("tall\n"):printf("with Metal\n") ;
}

void Controller2::stopConveyer() {
	HALAktorik::getInstance()->engine_stop();
	MachineS->running = false;
}
void Controller2::startConveyer() {
	HALAktorik::getInstance()->engine_start();
	MachineS->running = true;
}

void Controller2::resetPuk(int puk) {
	//	MachineS->machineIsOn = false;
	timerC2->setTimer(puk, -1);
	pukArr[puk].pukIdentifier = -1;
	pukArr[puk].type = undefined;
	pukArr[puk].place = CONVEYOREMPTY;
	pukArr[puk].height1 = 0;
	pukArr[puk].height2 = 0;
	pukArr[puk].metall = false;

	MachineS->isMetal = false;
	MachineS->goingBack = false;
	MachineS->turnAround = false;
}

void Controller2::printPlace(int place) {
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
