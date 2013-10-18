/*
 * HAL.h
 *
 *  Created on: 10.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
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
	if(!isON(BIT_4)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_4);
	}
	HALmutex->unlock();
}
void HAL::switchClose() {
	HALmutex->lock();
	if(isON(BIT_4)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_4);
	}
	HALmutex->unlock();
}

void HAL::redOn() {
	HALmutex->lock();
	if(!isON(BIT_7)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_7);
	}
	HALmutex->unlock();
}

void HAL::redOff() {
	HALmutex->lock();
	if(isON(BIT_7)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_7);
	}
	HALmutex->unlock();
}

void HAL::yellowOn() {
	HALmutex->lock();
	if(!isON(BIT_6)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_6);
	}
	HALmutex->unlock();
}
void HAL::yellowOff() {
	HALmutex->lock();
	if(isON(BIT_6)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_6);
	}
	HALmutex->unlock();
}
void HAL::greenOn() {
	HALmutex->lock();
	if(!isON(BIT_5)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_5);
	}
	HALmutex->unlock();
}
void HAL::greenOff() {
	HALmutex->lock();
	if(IsOn(BIT_5)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_5);
	}
	HALmutex->unlock();
}

void HAL::engine_rigth() {
	HALmutex->lock();
	if(!isOn(BIT_0) || isON(BIT_1)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		//left off
		val = val & ~BIT_1;
		//rigth on
		val = val | BIT_0;
		out8(DIO_BASE + DIO_OFFS_A, val);
	}
	HALmutex->unlock();
}
void HAL::engine_left() {
	HALmutex->lock();
	if(!isOn(BIT_1) || isON(BIT_0)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		//rigth off
		val = val & ~BIT_0;
		//left on
		val = val | BIT_1;
		out8(DIO_BASE + DIO_OFFS_A, val);
	}
	HALmutex->unlock();
}
void HAL::engine_slowON() {
	HALmutex->lock();
	if(!isON(BIT_2)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_2);
	}
	HALmutex->unlock();
}
void HAL::engine_slowOFF() {
	HALmutex->lock();
	if(isON(BIT_2)){
		cout << "Slow switched off" << endl;
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_2);
	}
	HALmutex->unlock();
}
void HAL::engine_stop() {
	HALmutex->lock();
	if(!isON(BIT_3)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_3);
	}
	HALmutex->unlock();
}
void HAL::engine_start() {
	HALmutex->lock();
	if(isON(BIT_3)){
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_3);
	}
	HALmutex->unlock();
}
bool  HAL::isON(int bit){
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	HALmutex->unlock();
	return (uint8_t)bit == (val & bit);
}



