/*
 * Controller2.h
 *
 *  Created on: 22.11.2013
 *      Author: Jannik
 */

#ifndef CONTROLLER2_H_
#define CONTROLLER2_H_
#include "Mutex.h"
#include "PukType.h"
#include "Addresses.h"
#include "PetriDefines.h"



class Controller2 {

private:

	static Controller2* instance;
	static Mutex* controller2_mutex;



	int pukPointer;
	int numOfPuks;
	int pukIdentifier;

	Controller2();
public:
	struct puk{
		int pukIdentifier;
		pukType type;
		int place;
		int height1;
		int height2;
		bool metall;
	} pukArr[N_PUKS];

	bool ack;

    static Controller2* getInstance();
	bool errorFlag;
	void errorFound();

	~Controller2();
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


	void resetPuk(int puk);
	void stopConveyer();
	void startConveyer();
	void printPuk(int puk);
	void printPlace(int place);
};

#endif /* CONTROLLER1_H_ */
