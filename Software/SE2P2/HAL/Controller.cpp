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
	pukZeiger = 0;
	int i = 0;
	for (; i < N_PUKS; i++) {
		pukArr[i].stelle = 0;
		pukArr[i].hohe = 0;
	}
}

void Controller::inEinlauf() {
	pukArr[pukZeiger].stelle = 1;
	pukZeiger = (pukZeiger + 1) % N_PUKS;

	HALa->greenLigths(ON);
	HALa->engine_rigth();
}
void Controller::einlaufVerlassen() {
	int puk = 0;
	while (pukArr[puk].stelle != 1) {
		puk++;
	}
	pukArr[puk].stelle = 2;
}

void Controller::inHohenmessung() {
	int puk = 0;
	while (pukArr[puk].stelle != 2) {
		puk++;
	}
#ifdef SIMULATION
	if (Mstat->height >= 60000 && Mstat->height <= 70000) {
		pukArr[puk].stelle = 3;
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
}

void Controller::metallSetzen() {
	int puk = 0;
	while (pukArr[puk].stelle != 3) {
		puk++;
	}

	pukArr[puk].metall = true;
	pukArr[puk].stelle = 7;
}

void Controller::printPuk(int puk){
	printf("Puk:\n");
	printf("Stelle: %d\n", pukArr[puk].stelle);
	printf("Metall: %d\n", pukArr[puk].metall);
	printf("Hohe: %d\n", pukArr[puk].hohe);
}
