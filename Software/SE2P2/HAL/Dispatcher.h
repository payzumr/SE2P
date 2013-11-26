/*
 * Dispatcher.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "../HAW/HAWThread.h"
#include "../HAW/HWaccess.h"
#include "HALSensorik.h"
#include "MachineState.h"
#include "Addresses.h"
#include "Mutex.h"
#include "Controller.h"

class Dispatcher: public thread::HAWThread {
private:

	static Dispatcher* instance;
	static Mutex* dispatcher_mutex;

	int signalChid;

	bool portB_0;
	bool portB_1;
//	bool portB_2;
	bool portB_3;
	bool portB_4;
	bool portB_5;
	bool portB_6;
	bool portB_7;

//	bool portC_4;
//	bool portC_5;
	bool portC_6;
	bool portC_7;

	bool not_aus_reset;

	void setSensorChanges(int code, int val);
public:
	Dispatcher();
	static Dispatcher* getInstance();
	virtual ~Dispatcher();

protected:
	virtual void execute(void* arg);
	virtual void shutdown();

};

#endif /* DISPATCHER_H_ */
