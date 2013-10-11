/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Thread.h"
#include <iostream>
#include <unistd.h>
#include "HWaccess.h"
#include "HAL.h"

using namespace std;

namespace thread {
HAL* hal = HAL::getInstance();

//Default Konstruktor der Klasse Thread
Thread::Thread() {
	// TODO Auto-generated constructor stub

}

// Default Destruktor Klasse Thread
Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void Thread::shutdown() {

	cout << "Shutting down..." << endl;
	hal->engine_stop();
	hal->greenOn();

}

void Thread::execute(void *arg) {

	while (!isStopped()) {
		hal->greenOn();
		sleep(2);
		hal->greenOff();
		sleep(2);
		hal->yellowOn();
		sleep(2);
		hal->yellowOff();
		sleep(2);
		hal->redOn();
		sleep(2);
		hal->redOff();
		sleep(2);
		hal->switchOpen();
		sleep(2);
		hal->switchClose();
		sleep(2);
		hal->engine_rigth();
		sleep(2);
		hal->engine_stop();
		sleep(2);
		hal->engine_left();
		sleep(2);
		hal->engine_start();
		sleep(2);
		hal->engine_slowON();
		sleep(2);
		hal->engine_slowOFF();
		sleep(2);
	}
}

}
