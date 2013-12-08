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
#include "Controller1.h"
#include "Timer.h"

class Dispatcher: public thread::HAWThread {
private:

	static Dispatcher* instance;
	static Mutex* dispatcher_mutex;

	int signalChid;

	bool e_stop;

	void setSensorChanges(int code, int val);
	Dispatcher();
public:
	static Dispatcher* getInstance();
	virtual ~Dispatcher();

	//testwert:
	long testzeitD;

protected:
	virtual void execute(void* arg);
	virtual void shutdown();

};

#endif /* DISPATCHER_H_ */
