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
/**
 * This function turns the switch on or off.
 * @param: true = ON
 * 		   false = OFF
 */

void HAL::switchOnOff(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_4);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_4);
		HALmutex->unlock();
	}
}
/**
 * This function turns the redligths on or off
 * @param true = ON
 * 		  false = OFF
 */
void HAL::redLigths(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_7);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_7);
		HALmutex->unlock();
	}
}
/**
 * This function turns the yellowligths on or off
 * @param true = ON
 * 		  false = OFF
 */
void HAL::yellowLigths(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_6);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_6);
		HALmutex->unlock();
	}
}

/**
 * This function turns the greenligths on or off
 * @param true = ON
 * 		  false = OFF
 */
void HAL::greenLigths(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_5);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_5);
		HALmutex->unlock();
	}
}
/**
 * This function starts the engine in rigth direction
 */
void HAL::engine_rigth() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	val = val | (BIT_0 & ~BIT_1);
	out8(DIO_BASE + DIO_OFFS_A, val);
	HALmutex->unlock();
}
/**
 * This function starts the engine in left direction
 */
void HAL::engine_left() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	val = val | (BIT_1 & ~BIT_0);
	out8(DIO_BASE + DIO_OFFS_A, val);
	HALmutex->unlock();
}
/**
 * This function turns the engine in slow mode
 * @param true = ON
 * 		  false = OFF
 */
void HAL::engine_slow(bool state) {
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val | BIT_2);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
		out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_2);
		HALmutex->unlock();
	}
}
/**
 * This function stops the engine
 */
void HAL::engine_stop() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_3);
	HALmutex->unlock();
}
/**
 * This function starts the engine
 */
void HAL::engine_start() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_3);
	HALmutex->unlock();
}