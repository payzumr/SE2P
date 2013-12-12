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
#include "HALAktorik.h"

using namespace thread;

#define TIMERCNT(a) (a!=-1)?a-=1; // ??

Mutex* Timer::Timer_mutex = new Mutex();
Timer* Timer::instance = NULL;

Timer::Timer() {

	for (int i = 0; i < N_TIMER; i++) {
		timerArr[i] = -1;
	}
	slideTimer = -1;
	turnaroundTimer = -1;
	endTimer = -1;
	quittiertTimer = -1;

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

void Timer::execute(void* args) {
	MachineState* Mst = MachineState::getInstance();
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
		testzeit += 1; // Nur für die Initialisierung??

		if (Mst->running) {
			countDownTimer();
		}
		if (turnaroundTimer != -1) {
			turnaroundTimer -= 1;
		}
		if (turnaroundTimer == 0) {
			printf("Timeout: Werkstueck umdrehen!\n");
#ifdef BAND_1
			Controller1::getInstance()->errorFlag = true;
			Controller1::getInstance()->errorFound();
#endif
#ifdef BAND_2
			Controller2::getInstance()->errorFlag = true;
			Controller2::getInstance()->errorFound();
#endif
			turnaroundTimer = -1;
		}

		if (quittiertTimer != -1) {
			quittiertTimer -= 1;
		}
		if (quittiertTimer == 0) {
			HALAktorik::getInstance()->redLigths(ON);
			Mst->quittiert = true;
			quittiertTimer = -1;
		}
	}
}
void Timer::shutdown() {
	cout << "shutdown Timer..." << endl;
}

void Timer::countDownTimer() {
	HALAktorik* HALak = HALAktorik::getInstance();
	for (int i = 0; i < N_TIMER; i++) {
		if (timerArr[i] > 0) {
			timerArr[i] -= 1;
		}
		if (timerArr[i] == 0) {
			cout << "Timeout: Fehlerzustand Puk! Nr: " << endl;
#ifdef BAND_1
			Controller1* control = Controller1::getInstance();
#endif
#ifdef BAND_2
			Controller2* control = Controller2::getInstance();
#endif
			control->errorFlag = true;
			control->errorFound();
			timerArr[i] = -1;
		}
	}
	if (slideTimer != -1) {
		slideTimer -= 1;
	}
	if (slideTimer == 0) {
		cout << "Timeout: Fehlerzustand Rutsche voll" << endl;
#ifdef BAND_1
		Controller1::getInstance()->errorFound();
#endif
#ifdef BAND_2
		Controller2::getInstance()->errorFound();
#endif
		slideTimer = -1;
	}

	if (switchTimer != -1) {
		switchTimer -= 1;
	}
	if (switchTimer == 0) {
		HALak->switchOnOff(OFF);
		switchTimer = -1;
	}

	if (slowTimer != -1) {
		slowTimer -= 1;
	}

	if (endTimer == 0) {
		//cout << "Fehler im EndTimer" << endl;
#ifdef BAND_1
		Controller1::getInstance()->reset();
#endif
#ifdef BAND_2
		Controller2::getInstance()->reset();
#endif
		endTimer = -1;
	}
	if (endTimer != -1) {
		endTimer -= 1;
	}
}

void Timer::setTimer(int timer, int value) {
	timerArr[timer] = value;
}
void Timer::resetTimer() {
	for (int i = 0; i < N_TIMER; i++) {
		timerArr[i] = -1;
	}
	slideTimer = -1;
	turnaroundTimer = -1;

	//Endtimer auch reseten?
}

void Timer::addSlowTime(int puk) {
	for (int i = 0; i < N_TIMER; i++) {
		if (i != puk && timerArr[i] != -1) {
			timerArr[i] += MachineState::getInstance()->slowTimeAdd;
		}
	}
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

void Timer::showTimeArray() {
	int i = 0;
	for (; i < N_TIMER; i++) {
		printf("PukTimer %d = %d\n", i, timerArr[i]);
	}
}

