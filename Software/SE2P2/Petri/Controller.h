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
#include "Blinki.h"
#include "Timer.h"



class Controller {

private:

	static Controller* instance;
	static Mutex* controller_mutex;

	void errorFound();
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
public:

    static Controller* getInstance();

	~Controller();
	void init();
	void reset();
	//sensoren
	void entryStartSens();
	void exitStartSens();
	void entryHeightMessure();
	void exitHeightMessure();
	void metalFound();
	void entrySlide();
	void exitSlide();
	void entrySwitch();
	void exitSwitch();
	void entryFinishSens();
	void exitFinishSens();

	//tasten
	void stopPressed();
	void startPressed();
	void EStopPressed();
	void EStopReleased();

	void printPuk(int puk);
};

#endif /* CONTROLLER_H_ */
