/*
 * Controller1.h
 *
 *  Created on: 22.11.2013
 *      Author: Jannik
 */

#ifndef CONTROLLER1_H_
#define CONTROLLER1_H_

#include "Mutex.h"
#include "PukType.h"
#include "Addresses.h"
#include "PetriDefines.h"
#include "Blinki.h"



class Controller1 {

private:

	static Controller1* instance;
	static Mutex* controller1_mutex;

	void errorFound();


	int pukPointer;
	int numOfPuks;
	int pukIdentifier;

	Controller1();
public:
	bool errorFlag;
	struct puk{
		int pukIdentifier;
		pukType type;
		int place;
		int height1;
		int height2;
		bool metall;
	} pukArr[N_PUKS];

    static Controller1* getInstance();

	~Controller1();

	bool ack;

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

	void handover(int puk);

	//tasten
	void stopPressed();
	void startPressed();
	void EStopPressed();
	void EStopReleased();

	void resetPuk(int puk);
	void stopConveyer();
	void startConveyer();
	void printPuk(int puk);
};

#endif /* CONTROLLER1_H_ */
