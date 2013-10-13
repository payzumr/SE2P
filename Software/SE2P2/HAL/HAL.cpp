/*
 * HAL.cpp
 *
 *  Created on: 10.10.2013
 *      Author: Jannik
 */

#include "HAL.h"
#include "HWaccess.h"
#include "Addresses.h"
#include <unistd.h>
#include <stdint.h>

using namespace std;

Mutex* HAL::HALmutex = new Mutex();

HAL* HAL::instance = NULL;

HAL::HAL() {
	// Initialisierung der Digitalen IO Karte
	out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);
	out8(DIO_BASE + DIO_OFFS_A, 0x00);
	out8(DIO_BASE + DIO_OFFS_C, 0x00);
}

HAL::~HAL() {
	delete instance;
	instance = NULL;
}

HAL* HAL::getInstance() {

	// Zugriffsrechte fuer den Zugriff auf die HW holen
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return NULL;
	}
	if (instance == NULL) {
		instance = new HAL();
	}
	return instance;

}

void HAL::switchOpen() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_4);
	HALmutex->unlock();

}
void HAL::switchClose() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_4);
	HALmutex->unlock();

}

void HAL::redOn() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_7);
	HALmutex->unlock();
}

void HAL::redOff() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_7);

	HALmutex->unlock();
}

void HAL::yellowOn() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_6);

	HALmutex->unlock();
}
void HAL::yellowOff() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_6);

	HALmutex->unlock();
}
void HAL::greenOn() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_5);
	HALmutex->unlock();

}
void HAL::greenOff() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_5);

	HALmutex->unlock();
}

void HAL::engine_rigth() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);

	//Stop off
	val = val & ~BIT_3;
	//left off
	val = val & ~BIT_1;
	//rigth on
	val = val | BIT_0;
	out8(DIO_BASE + DIO_OFFS_A, val);

	//out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_3);
	//val = in8(DIO_BASE + DIO_OFFS_A);
	//out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_1);
	//val = in8(DIO_BASE + DIO_OFFS_A);
	//out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_1);
	//val = in8(DIO_BASE + DIO_OFFS_A);
	//out8(DIO_BASE + DIO_OFFS_A, val | BIT_0);
	HALmutex->unlock();
}
void HAL::engine_left() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);

	//Stop off
	val = val & ~BIT_3;
	//rigth off
	val = val & ~BIT_0;
	//left on
	val = val | BIT_1;

	out8(DIO_BASE + DIO_OFFS_A, val);
	/*
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_3);
	val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_0);
	val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_0);
	//val = in8(DIO_BASE + DIO_OFFS_A);

	out8(DIO_BASE + DIO_OFFS_A, val | BIT_1);
	*/
	HALmutex->unlock();
}
void HAL::engine_slowON() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_2);
	HALmutex->unlock();
}
void HAL::engine_slowOFF() {
	if(slowIsOn()){
		cout << "Slow switched off" << endl;
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_2);
		HALmutex->unlock();
	}
}
void HAL::engine_stop() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_3);
	HALmutex->unlock();
}
void HAL::engine_start() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_3);
	HALmutex->unlock();
}

bool  HAL::slowIsOn(){
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	HALmutex->unlock();

	return (uint8_t)BIT_2 == (val & BIT_2);
}



