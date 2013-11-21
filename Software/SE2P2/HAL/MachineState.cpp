/*
 * MachineState.cpp
 *
 *  Created on: 20.11.2013
 *      Author: Jannik
 */

#include "MachineState.h"

Mutex* MachineState::Machine_Mutex = new Mutex();
MachineState* MachineState::instance = NULL;

MachineState::MachineState() {

/*
	instance->SensEntry = false;
	instance->SensExit = false;

	instance->SensHeight = false;
	instance->SensMetall = false;
	instance->SensSlip = false;
	instance->SensSwitch = false;
	instance->SwitchOpen = false;
	instance->height = 0;
	*/

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
		   instance->height);
}
