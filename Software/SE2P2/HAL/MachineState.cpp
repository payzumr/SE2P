/*
 * MachineState.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "MachineState.h"

Mutex* MachineState::Machine_Mutex = new Mutex();
MachineState* MachineState::instance = NULL;

MachineState::MachineState() {
	machineStateReset();

}

MachineState::~MachineState() {
	delete instance;
	instance = NULL;
}

void MachineState::machineStateReset(){

	slideTime = SLIDE_TIME;
	turnaroundTime = TURN_TIME;



	/**
	 * Sensor States default is false
	 */
	machineIsOn = false;
	sensEntry = false;
	sensExit = false;
	sensHeight = false;
	sensMetall = false;
	sensSlip = false;
	sensSwitch = false;
	switchOpen = false;
	initRound = false;
	quittiert = false;

	bandBelegt = false;
	goingBack = false;
	isMetal = false;
	sortOut = false;

	redSlow = false;
	redFast = false;
	stopLigth = true;
	yellow = false;
	green = false;


	running = false;
	dispatcherGo = true;
	turnAround = false;

	height = 0;

}
MachineState* MachineState::getInstance(){
	Machine_Mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new MachineState();
	}
	Machine_Mutex->unlock();
	return instance;
}

void MachineState::showMachineState(){

	printf("Werkstueck in Einlauf: %d \n"
		   "Werkstueck in Auslauf: %d \n"
		   "Werkstueck in Hoehenmessung: %d \n"
		   "Werkstueck in Weiche: %d \n"
		   "Werkstueck Metall: %d \n"
		   "Rutsche voll: %d \n"
		   "Weiche offen: %d  \n"
		   "E Stop gedrückt: %d \n"
		   "Hoehe: %d \n",
		   instance->sensEntry,
		   instance->sensExit,
		   instance->sensHeight,
		   instance->sensSwitch,
		   instance->sensMetall,
		   instance->sensSlip,
		   instance->switchOpen,
		   //instance->E_Stop,
		   instance->height);
}

void MachineState::showTimes(){
	printf("entry to height fast: %d \n"
			   "height to switch fast: %d \n"
			   "switch to exit fast: %d \n"
				"in height time: %d \n"
			"Slow Time Add: %d \n",
			   entryToHeight_f,
			   heightToSwitch_f,
			   switchToExit_f,
			   inHeigthTime,
			   slowTimeAdd);
}
