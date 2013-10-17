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
#include "ComponentTest.h"

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
	ComponentTest componenttest;

	while(!isStopped()){
		componenttest.StartTest(hal);
	}

}

}
