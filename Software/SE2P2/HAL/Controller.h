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

#define N_PUKS 10

class Controller {
public:
	struct puk{
		int stelle;
		int hohe;
		bool metall;
	} pukArr[N_PUKS];

	int pukZeiger;

	Controller();
	virtual ~Controller();
	void init();
	void inEinlauf();
	void einlaufVerlassen();
	void inHohenmessung();
	void metallSetzen();

	void printPuk(int puk);
};

#endif /* CONTROLLER_H_ */
