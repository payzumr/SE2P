/*
 * Controller.h
 *
 *  Created on: 22.11.2013
 *      Author: Jannik
 */

#ifndef CONTROLLER_H_
#define CONTROLLER_H_
#include "../HAW/HAWThread.h"
#include "../HAW/HWaccess.h"
#include "HALSensorik.h"
#include "HALAktorik.h"
#include "MachineState.h"
#include "Addresses.h"
#include "PukType.h"
#include "PetriDefines.h"

#define N_PUKS 2

class Controller {

private:
	void errorFound();
public:
	struct puk{
		int pukIdentifier;
		pukType type;
		int place;
		int height1;
		int height2;
		bool metall;
	} pukArr[N_PUKS];

	int pukPointer;
	int numOfPuks;
	int pukIdentifier;
	bool errorFlag;

	Controller();
	virtual ~Controller();
	void init();
	void reset();
	//sensoren
	void entryStartSens();
	void exitStartSens();
	void entryHeightMessure();
	void exitHeightMessure();
	void metalFound();
	void entrySwitch();
	void exitSwitch();
	void entryFinishSens();
	void exitFinishSens();

	//tasten
	void EStopPressed();
	void EStopReleased();

	void printPuk(int puk);
};

#endif /* CONTROLLER_H_ */
