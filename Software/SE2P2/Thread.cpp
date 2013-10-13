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

//Default Konstruktor der Klasse Thread
Thread::Thread() {
	// TODO Auto-generated constructor stub

}

// Default Destruktor Klasse Thread
Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void Thread::shutdown() {
	HAL* hal = HAL::getInstance();
	cout << "Shutting down..." << endl;
	hal->engine_stop();

}

void Thread::execute(void *arg) {
	HAL* hal = HAL::getInstance();
	while (!isStopped()) {
		hal->greenOn();
		sleep(1);
		hal->greenOff();
		sleep(1);
		hal->yellowOn();
		sleep(1);
		hal->yellowOff();
		sleep(1);
		hal->redOn();
		sleep(1);
		hal->redOff();
		sleep(1);
		hal->switchOpen();
		sleep(1);
		hal->switchClose();
		sleep(1);
		hal->engine_rigth();
		sleep(1);
		hal->engine_stop();
		sleep(1);
		hal->engine_left();
		sleep(1);
		hal->engine_start();
		sleep(1);
		hal->engine_slowON();
		sleep(1);
		hal->engine_slowOFF();
		sleep(1);
		hal->engine_slowOFF();
		sleep(1);
	}
}

}
