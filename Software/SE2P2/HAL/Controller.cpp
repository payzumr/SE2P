/*
 * Controller.cpp
 *
 *  Created on: 22.11.2013
 *      Author: Jannik
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
	fehlerFlag = false;
	anzahlPuks = 0;
	pukZeiger = 0;
	pukIdentifier = 0;
	int i = 0;
	for (; i < N_PUKS; i++) {
		pukArr[i].pukIdentifier = 0;
		pukArr[i].type = undefiniert;
		pukArr[i].stelle = S0;
		pukArr[i].hohe1 = 0;
		pukArr[i].hohe2 = 0;
	}
}

void Controller::neustart(){
	init();
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	HALa->redLigths(OFF);
	HALa->engine_slow(OFF);
	HALa->engine_start();
	HALa->switchOnOff(OFF);
}

void Controller::eintrittEinlauf() {
	if (++anzahlPuks == N_PUKS + 1) {
		fehlerAufgetreten();
	} else {
		pukArr[pukZeiger].stelle = S1;
		pukZeiger = (pukZeiger + 1) % N_PUKS;
		pukArr[pukZeiger].pukIdentifier = pukIdentifier;
		pukIdentifier++;

		HALa->greenLigths(ON);
		HALa->engine_rigth();
	}
}
void Controller::austrittEinlauf() {
	int puk = 0;
	while (pukArr[puk].stelle != S1) {
		puk++;
	}
	pukArr[puk].stelle = S2;
	//	printPuk(puk);
}

void Controller::eintrittHohenmessung() {
	HALa->engine_slow(ON);
	int puk = 0;
	while (pukArr[puk].stelle != S2 && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			fehlerFlag = true;
		}
	}

	if (!fehlerFlag) {
		pukArr[puk].stelle = S3;
		//	printPuk(puk);
#ifdef SIMULATION
		if (Mstat->height >= 60000 && Mstat->height <= 70000) {
			pukArr[puk].stelle = S4;
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
		fehlerAufgetreten();
	}
}

void Controller::austrittHohenmessung() {
	HALa->engine_slow(OFF);
#ifdef SIMULATION
	//HALa->switchOnOff(ON);
#endif
}

void Controller::metallSetzen() {
	int puk = 0;
	while (pukArr[puk].stelle != S4 && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			fehlerFlag = true;
		}
	}
	if (!fehlerFlag) {
		pukArr[puk].metall = true;
		pukArr[puk].type = mitMetall;
	} else {
		fehlerAufgetreten();
	}
}

void Controller::eintrittWeiche() {
	int puk = 0;
	printPuk(puk);
	while ((pukArr[puk].stelle != S4 && pukArr[puk].stelle != S5
			&& pukArr[puk].stelle != S6) && puk <= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			fehlerFlag = true;
		}
	}
	if (!fehlerFlag) {
		if (pukArr[puk].stelle == S4 && pukArr[puk].type == mitMetall) {
			pukArr[puk].stelle = S8;
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].stelle == S4) {
			pukArr[puk].stelle = S8;//auf 7 zurucksetzen!!!!!!!!!!!!!!!!!!!!!!!!!
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].stelle == S5) {
			pukArr[puk].stelle = S8;
			HALa->switchOnOff(ON);
		} else if (pukArr[puk].stelle == S6) {

		}
		printPuk(puk);

	} else {
		fehlerAufgetreten();
	}
}
void Controller::austrittWeiche() {
	int puk = 0;
	while ((pukArr[puk].stelle != S7 && pukArr[puk].stelle != S8) && puk
			<= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			fehlerFlag = true;
		}
	}
	if (!fehlerFlag) {
		if (pukArr[puk].stelle == S7) {
			pukArr[puk].stelle = S9;
		} else {
			pukArr[puk].stelle = S10;
		}
	} else {
		fehlerAufgetreten();
	}
	HALa->switchOnOff(OFF);
}
void Controller::eintrittAuslauf() {
	int puk = 0;
	while ((pukArr[puk].stelle != S9 && pukArr[puk].stelle != S10) && puk
			<= N_PUKS) {
		puk++;
		if (puk == N_PUKS) {
			fehlerFlag = true;
		}
	}
	if (!fehlerFlag) {
		if (pukArr[puk].stelle == S9) {
			pukArr[puk].stelle = S12;
		} else {
			HALa->engine_stop();
			HALa->greenLigths(OFF);
			HALa->yellowLigths(ON);
			pukArr[puk].stelle = S11;
		}
	} else {
		fehlerAufgetreten();
	}
}

//tasten
void Controller::notAusGedruckt() {
	HALa->engine_stop();
	HALa->redLigths(ON);
	HALa->greenLigths(OFF);
	HALa->yellowLigths(OFF);
	init();
	printf("Not-Aus gedrueckt! Band muss abgeraumt werden!\n");

}

void Controller::fehlerAufgetreten() {
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
	printf("Stelle: %d\n", pukArr[puk].stelle);
	printf("Metall: %d\n", pukArr[puk].metall);
	printf("Hohe1: %d\n", pukArr[puk].hohe1);
}
