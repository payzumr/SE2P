/*
 * Initialisation.h
 *
 *  Created on: 28.11.2013
 *      Author: Jannik
 */

#ifndef INITIALISATION_H_
#define INITIALISATION_H_

#include "../HAW/HAWThread.h"
#include "../HAW/HWaccess.h"
#include "HALSensorik.h"
#include "MachineState.h"
#include "Addresses.h"
#include "Mutex.h"

class Initialisation: public thread::HAWThread {
private:

	static Initialisation* instance;
	static Mutex* Initialisation_mutex;

	int signalChid;

	void setSensorChanges(int code, int val);
	Initialisation();
public:
	static Initialisation* getInstance();
	virtual ~Initialisation();

	//testwert:
	long testzeitD;

protected:
	virtual void execute(void* arg);
	virtual void shutdown();

};

#endif /* INITIALISATION_H_ */
