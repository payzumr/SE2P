/*
 * Blinki.cpp
 *
 *  Created on: 27.11.2013
 *      Author: Jannik
 */

#include "Blinki.h"
#include "HWaccess.h"
#include <stdio.h>
#include "HALAktorik.h"

hal::Blinki* hal::Blinki::instance = NULL;
Mutex* hal::Blinki::Blinki_mutex = new Mutex();


hal::Blinki::Blinki() {

}

hal::Blinki::~Blinki() {
	delete instance;
	instance = NULL;
}

hal::Blinki* hal::Blinki::getInstance() {
	Blinki_mutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Blinki();
	}
	Blinki_mutex->unlock();

	return instance;
}

void hal::Blinki::execute(void* arg){

	while(!isStopped()){

	blinkGelb();
	}
}

void hal::Blinki::shutdown(){}


void hal::Blinki::blinkGelb()
{
HALAktorik* Hala = HALAktorik::getInstance();
 Hala->yellowLigths(ON);
 usleep(500000);
 Hala->yellowLigths(OFF);
 usleep(500000);
}

