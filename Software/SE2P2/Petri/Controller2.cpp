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
}
void Controller2::reset() {
}
void Controller2::entryStartSens() {
}
void Controller2::exitStartSens() {
}
void Controller2::entryHeightMessure() {
}
void Controller2::exitHeightMessure() {
}
void Controller2::metalFound() {
}
void Controller2::entrySlide() {
}
void Controller2::exitSlide() {
}
void Controller2::entrySwitch() {
}
void Controller2::exitSwitch() {
}
void Controller2::entryFinishSens() {
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
