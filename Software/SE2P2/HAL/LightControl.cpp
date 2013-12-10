/*
 * LightControl.cpp
 *
 *  Created on: 06.12.2013
 *      Author: Jannik
 */

#include "LightControl.h"
#include "HALAktorik.h"
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "HWaccess.h"
#include "MachineState.h"

LightControl* LightControl::instance = NULL;
Mutex* LightControl::lightMutex = new Mutex();

LightControl::LightControl() {
	// TODO Auto-generated constructor stub

}

LightControl::~LightControl() {
	delete instance;
	instance = NULL;
}
LightControl* LightControl::getInstance() {
	lightMutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new LightControl();
	}
	lightMutex->unlock();

	return instance;
}

void LightControl::execute(void* arg) {
	MachineState* machineS = MachineState::getInstance();
	while (!isStopped()) {

		if (machineS->stopLigth) {
			stopLights();

		} else if (machineS->redFast) {
			flashRedFast();

		} else if (machineS->redSlow) {
			flashRedSlow();
		} else if (machineS->yellow) {
			flashYellow();
		} else if (machineS->green) {
			flashGreenIni();
		} else {
			usleep(10000);
		}
	}
}
void LightControl::shutdown() {
	cout << "shutdown LightControl ..." << endl;
}

void LightControl::flashYellow() {
	HALAktorik::getInstance()->yellowLigths(ON);
	sleep(1);
	HALAktorik::getInstance()->yellowLigths(OFF);
	sleep(1);
}

void LightControl::flashRedSlow() {
	HALAktorik::getInstance()->redLigths(ON);
	sleep(2);
	HALAktorik::getInstance()->redLigths(OFF);
	sleep(2);
}

void LightControl::flashRedFast() {
	HALAktorik::getInstance()->redLigths(ON);
	sleep(1);
	HALAktorik::getInstance()->redLigths(OFF);
	sleep(1);
}

void LightControl::flashGreenIni() {
	HALAktorik::getInstance()->greenLigths(ON);
	sleep(1);
	HALAktorik::getInstance()->greenLigths(OFF);
	sleep(1);
}
void LightControl::stopLights() {

	MachineState::getInstance()->redFast = false;
	MachineState::getInstance()->redSlow = false;
	MachineState::getInstance()->yellow = false;
	MachineState::getInstance()->green = false;
	if (!MachineState::getInstance()->machineIsOn) {
		HALAktorik::getInstance()->greenLigths(OFF);
	}
	HALAktorik::getInstance()->yellowLigths(OFF);
	cout << "mache aus " << endl;

//	HALAktorik::getInstance()->redLigths(OFF);
	MachineState::getInstance()->stopLigth = false;
}
