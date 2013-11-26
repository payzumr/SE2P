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
HALAktorik* HALa = HALAktorik::getInstance();
MachineState* Mstat = MachineState::getInstance();
Controller::Controller() {
	// TODO Auto-generated constructor stub

}

Controller::~Controller() {
	// TODO Auto-generated destructor stub
}

void Controller::init() {
	errorFlag = false;
	numOfPuks = 0;
	pukPointer = 0;
	pukIdentifier = 0;
	int i = 0;
	for (; i < N_PUKS; i++) {
		pukArr[i].pukIdentifier = 0;
		pukArr[i].type = undefiniert;
		pukArr[i].place = S0;
		pukArr[i].height1 = 0;
		pukArr[i].height2 = 0;
	}
}

void Controller::reset(){
	init();
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	HALa->redLigths(OFF);
	HALa->engine_slow(OFF);
	HALa->engine_start();
	HALa->switchOnOff(OFF);
}

void Controller::entryStartSens() {
	if (++numOfPuks == N_PUKS + 1) {
		errorFound();
	} else {
		pukArr[pukPointer].place = S1;
		pukPointer = (pukPointer + 1) % N_PUKS;
		pukArr[pukPointer].pukIdentifier = pukIdentifier;
		pukIdentifier++;

		HALa->greenLigths(ON);
		HALa->engine_rigth();
	}
}
void Controller::exitStartSens() {
	int puk = 0;
	while (pukArr[puk].place != S1) {
		puk++;
	}
	pukArr[puk].place = S2;
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
		pukArr[puk].place = S3;
		//	printPuk(puk);
#ifdef SIMULATION
		if (Mstat->height >= 60000 && Mstat->height <= 70000) {
			pukArr[puk].place = S4;
		}
#endif
		//	if (Mstat->height >= 60000 && Mstat->height <= 70000) {//hoher puk
		//
		//	}
		//	if (Mstat->height >= 60000 && Mstat->height <= 70000) {//niedriger puk
		//
		//	}
		//	if (Mstat->height >= 60000 && Mstat->height <= 70000) {//puk mit loch
		//
		//	}
	} else {
		errorFound();
	}
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
		pukArr[puk].type = mitMetall;
	} else {
		errorFound();
	}
}

void Controller::entrySwitch() {
	int puk = 0;
	printPuk(puk);
	while ((pukArr[puk].place != S4 && pukArr[puk].place != S5
			&& pukArr[puk].place != S6) && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		if (pukArr[puk].place == S4 && pukArr[puk].type == mitMetall) {
			pukArr[puk].place = S8;
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].place == S4) {
			pukArr[puk].place = S8;//auf 7 zurucksetzen!!!!!!!!!!!!!!!!!!!!!!!!!
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].place == S5) {
			pukArr[puk].place = S8;
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].place == S6) {

		}
		printPuk(puk);

	} else {
		errorFound();
	}
}
void Controller::exitSwitch() {
	int puk = 0;
	while ((pukArr[puk].place != S7 && pukArr[puk].place != S8) && puk
			<= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			errorFlag = true;
		}
	}
	if (!errorFlag) {
		if (pukArr[puk].place == S7) {
			pukArr[puk].place = S9;
		} else {
			pukArr[puk].place = S10;
		}
	} else {
		errorFound();
	}
	HALa->switchOnOff(OFF);
}
void Controller::entryFinishSens() {
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
		} else {
			HALa->engine_stop();
			HALa->greenLigths(OFF);
			HALa->yellowLigths(ON);
			pukArr[puk].place = S11;
		}
	} else {
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
