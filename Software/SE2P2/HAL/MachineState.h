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

	int height;

	bool machineIsOn;
	bool sensEntry;
	bool sensExit;
	bool sensHeight;
	bool sensSwitch;
	bool sensMetall;
	bool sensSlip;
	bool switchOpen;
	bool running;
	bool turnAround;
	//Band 2
	bool bandBelegt;
	bool goingBack;
	bool isMetal;
	bool sortOut;

	//initilisation
	bool dispatcherGo;
	bool initRound;

	//Lights
	bool redFast;
	bool redSlow;
	bool yellow;
	bool green;
	bool stopLigth;

	bool rutscheVoll;

	//times
	int entryToHeight_f;
	int heightToSwitch_f;
	int switchToExit_f;
	int goingBackTimer;

	int inHeigthTime;

	int slideTime;
	int turnaroundTime;

	int slowTimeAdd;
	int heightFast;
	int heightSlow;
	void showMachineState();
	void showTimes();
	void machineStateReset();

	bool quittiert;
};

#endif /* MACHINESTATE_H_ */
