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
#include "Test.h"
#include "Serial.h"

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
	//HAL* hal = HAL::getInstance();
	Serial* serial = Serial::getInstance();
	cout << "Shutting down..." << endl;
	//hal->engine_stop();
	serial->close_serial();

}

void Thread::execute(void *arg) {
	//HAL* hal = HAL::getInstance();
	Serial* serial = Serial::getInstance();
	serial->open_serial("/dev/ser1");
	Test test;

	while(!isStopped()){
		//test.componentTest(hal);
		test.serialTest(serial);
		sleep(2);
	}

}

}
