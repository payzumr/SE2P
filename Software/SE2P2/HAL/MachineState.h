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

#define SLIDE_TIME 1500
#define TURN_TIME 10000

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
	bool running;
	bool turnAround;
	int height;

	bool DispatcherGo;
	bool InitRound;

	//times
	int entryToHeight_f;
	int heightToSwitch_f;
	int switchToExit_f;

	int inHeigthTime;

	int slideTime;
	int turnaroundTime;

	int slowTimeAdd;
	int heightFast;
	int heightSlow;
	void showMachineState();
	void showTimes();
};

#endif /* MACHINESTATE_H_ */
