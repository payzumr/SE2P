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

void hal::Blinki::execute(void* arg) {

	while (!isStopped()) {
		switch ((int) arg) {
		case REDFAST:
			flashRedFast();
		case REDSLOW:
			flashRedSlow();
		case YELLOW:
			flashYellow();
		case GREEN:
			flashGreenIni();
		}
	}
}

void hal::Blinki::shutdown() {
	HALAktorik::getInstance()->yellowLigths(OFF);
	HALAktorik::getInstance()->redLigths(OFF);
	cout << "shutdown Blinki..." << endl;

}

void hal::Blinki::flashYellow() {
	HALAktorik::getInstance()->yellowLigths(ON);
	sleep(1);
	HALAktorik::getInstance()->yellowLigths(OFF);
	sleep(1);
}

void hal::Blinki::flashRedSlow() {
	HALAktorik::getInstance()->redLigths(ON);
	sleep(2);
	HALAktorik::getInstance()->redLigths(OFF);
	sleep(2);
}

void hal::Blinki::flashRedFast() {
	HALAktorik::getInstance()->redLigths(ON);
	sleep(1);
	HALAktorik::getInstance()->redLigths(OFF);
	sleep(1);
}

void hal::Blinki::flashGreenIni(){
	HALAktorik::getInstance()->greenLigths(ON);
	sleep(1);
	HALAktorik::getInstance()->greenLigths(OFF);
	sleep(1);
}

