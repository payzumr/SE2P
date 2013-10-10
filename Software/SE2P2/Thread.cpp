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
#include "IOMakros.h"
#include "Lights.h"
#include "Switch.h"

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
	cout << "Shutting down..." << endl;
}

void Thread::execute(void *arg) {
	Lights* lights = Lights::getInstance();
	Switch* swi = Switch::getInstance();
	while (!isStopped()) {
		swi->switchOpen();
		lights->greenOn();
		sleep(1);
		lights->yellowOn();
		sleep(1);
		lights->redOn();
		sleep(1);
		lights->greenOff();
		sleep(1);
		lights->yellowOff();
		sleep(1);
		lights->redOff();
		sleep(1);

		swi->switchClose();
		sleep(1);
	}
}

}
