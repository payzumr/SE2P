/*
 * Timer.cpp
 *
 *  Created on: 28.11.2013
 *      Author: Jannik
 */

#include "Timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

using namespace thread;

Mutex* Timer::Timer_mutex = new Mutex();
Timer* Timer::instance = NULL;


Timer::Timer() {
	for (int i = 0; i < N_TIMER; i++) {
		timerArr[i] = -1;
	}

	channelID = ChannelCreate(0);
	if (channelID == -1) {
		perror("Dispatcher: ChannelCreate signalChid failed");
		exit(EXIT_FAILURE);
	}
	//creats connection between PC Channel and Prozess
	connectID = ConnectAttach(0, 0, channelID, _NTO_SIDE_CHANNEL, 0);
	if (connectID == -1) {
		perror("Dispatcher: ConnectAttach signalCoid failed");
		exit(EXIT_FAILURE);
	}

}

Timer::~Timer() {
	// TODO Auto-generated destructor stub
	delete instance;
	instance = NULL;
}

void Timer::execute(void* args) {
	testzeit = 0;
	initTimer();
	struct _pulse pulse;
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(channelID, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}
		testzeit += 1;
		countDownTimer();

	}
}
void Timer::shutdown() {

}

void Timer::countDownTimer() {
HALAktorik* HALak = HALAktorik::getInstance();
	for (int i = 0; i < N_TIMER; i++) {
		if (timerArr[i] > 0) {
			timerArr[i] -= 1;
		}
		if(timerArr[i] == 0){
			HALak->engine_stop();
			HALak->redLigths(ON);
		}
	}
}

Timer* Timer::getInstance() {

	Timer_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Timer();
	}
	Timer_mutex->unlock();
	return instance;

}

void Timer::initTimer() {

	timer_t timerid;
	struct sigevent event;
	struct itimerspec timer;

	// Definiere Pulse Event
	SIGEV_PULSE_INIT (&event, connectID, SIGEV_PULSE_PRIO_INHERIT, 1, 0);
	// Erzeuge den Timer
	timer_create(CLOCK_REALTIME, &event, &timerid); // Fehlerbehandlung fehlt
	// Setup und Start eines periodischen Timers
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_nsec = 1000000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_nsec = 1000000;
	timer_settime(timerid, 0, &timer, NULL);
}

