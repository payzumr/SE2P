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
#include "PetriDefines.h"


namespace thread{
#define N_TIMER N_PUKS
#define SWITCH_OPEN_TIME 1000;
#define END_TIMER 3000;


class Timer: public HAWThread {
private:
	static Timer* instance;
	static Mutex* Timer_mutex;
	Timer();

	int connectID;
	int channelID;

	void initTimer();
	void countDownTimer();

public:
	static Timer* getInstance();
	virtual ~Timer();

	void setTimer(int timer, int value);
	void resetTimer();
	void addSlowTime(int puk);
	void showTimeArray();

	int timerArr[N_TIMER];
	int slideTimer;
	int turnaroundTimer;
	int switchTimer;
	int slowTimer;
	int endTimer;

	long testzeit;
protected:

	virtual void execute(void* args);
	virtual void shutdown();

};
}

#endif /* TIMER_H_ */
