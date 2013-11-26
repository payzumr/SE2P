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


	SensEntry = false;
	SensExit = false;

	SensHeight = false;
	SensMetall = false;
	SensSlip = false;
	SensSwitch = false;
	SwitchOpen = false;
	height = 0;


}

MachineState::~MachineState() {
	delete instance;
	instance = NULL;
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
		   "Hoehe: %d \n",
		   instance->SensEntry,
		   instance->SensExit,
		   instance->SensHeight,
		   instance->SensSwitch,
		   instance->SensMetall,
		   instance->SensSlip,
		   instance->SwitchOpen,
		   //instance->E_Stop,
		   instance->height);
}
