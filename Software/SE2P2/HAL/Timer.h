/*
 * Timer.h
 *
 *  Created on: 28.11.2013
 *      Author: Jannik
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>
#include "../HAW/HAWThread.h"
#include "../Mutex/Mutex.h"
#include "../HAW/HWaccess.h"
#include "MachineState.h"
#include "Addresses.h"
#include "Dispatcher.h"
#include <unistd.h>
#include <stdint.h>
#include <sys/siginfo.h>
#include <sys/neutrino.h>
#include <time.h>


namespace thread{

class Timer: public HAWThread {
private:
	static Timer* instance;
	static Mutex* Timer_mutex;
	Timer();

	int connectID;
	int channelID;

	void initTimer();


public:
	static Timer* getInstance();
	virtual ~Timer();
protected:

	virtual void execute(void* args);
	virtual void shutdown();

};
}

#endif /* TIMER_H_ */
