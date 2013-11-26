/*
 * MachineState.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef MACHINESTATE_H_
#define MACHINESTATE_H_

#include "Mutex.h"
#include "HWaccess.h"
#include "Addresses.h"
#include <unistd.h>
#include <stdint.h>
#include <iostream>

class MachineState {
private:
	static MachineState* instance;
	static Mutex* Machine_Mutex;

	MachineState();
public:
	virtual ~MachineState();
	static MachineState* getInstance();

	bool SensEntry;
	bool SensExit;
	bool SensHeight;
	bool SensSwitch;
	bool SensMetall;
	bool SensSlip;
	bool SwitchOpen;
	int height;

	void showMachineState();
};

#endif /* MACHINESTATE_H_ */
