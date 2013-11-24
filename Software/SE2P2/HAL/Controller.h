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
	void fehlerAufgetreten();
public:
	struct puk{
		int pukIdentifier;
		pukType type;
		int stelle;
		int hohe1;
		int hohe2;
		bool metall;
	} pukArr[N_PUKS];

	int pukZeiger;
	int anzahlPuks;
	int pukIdentifier;
	bool fehlerFlag;

	Controller();
	virtual ~Controller();
	void init();
	void neustart();
	//sensoren
	void eintrittEinlauf();
	void austrittEinlauf();
	void eintrittHohenmessung();
	void austrittHohenmessung();
	void metallSetzen();
	void eintrittWeiche();
	void austrittWeiche();
	void eintrittAuslauf();
	void austrittAuslauf();

	//tasten
	void notAusGedruckt();
	void notAusNichtGedruckt();

	void printPuk(int puk);
};

#endif /* CONTROLLER_H_ */
