/*
 * HAL.h
 *
 *  Created on: 10.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "HALAktorik.h"


using namespace std;

Mutex* HALAktorik::HALmutex = new Mutex();

HALAktorik* HALAktorik::instance = NULL;

HALAktorik::HALAktorik() {
	// Initialisierung der Digitalen IO Karte
	out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);
	out8(DIO_BASE + DIO_OFFS_A, 0x00);
	out8(DIO_BASE + DIO_OFFS_C, 0x00);
}

HALAktorik::~HALAktorik() {
	delete instance;
	instance = NULL;
}

HALAktorik* HALAktorik::getInstance() {

	HALmutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new HALAktorik();
	}
	HALmutex->unlock();
	return instance;
}
/**
 * This function turns the switch on or off.
 * @param: true = ON
 * 		   false = OFF
 */

void HALAktorik::switchOnOff(bool state){
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
void HALAktorik::redLigths(bool state){
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
void HALAktorik::yellowLigths(bool state){
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
void HALAktorik::greenLigths(bool state){
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
 * This function turns the LED Q1 on or off
 * @param true = ON
 * 		  false = OFF
 */
void HALAktorik::led_Q1(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val | BIT_2);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val & ~BIT_2);
		HALmutex->unlock();
	}
}
/**
 * This function turns the LED Q2 on or off
 * @param true = ON
 * 		  false = OFF
 */
void HALAktorik::led_Q2(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val | BIT_3);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val & ~BIT_3);
		HALmutex->unlock();
	}
}
/**
 * This function turns the LED Start on or off
 * @param true = ON
 * 		  false = OFF
 */
void HALAktorik::led_Start(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val | BIT_0);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val & ~BIT_0);
		HALmutex->unlock();
	}
}
/**
 * This function turns the LED Reset on or off
 * @param true = ON
 * 		  false = OFF
 */
void HALAktorik::led_Reset(bool state){
	if(state){
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val | BIT_1);
		HALmutex->unlock();
	}else{
		HALmutex->lock();
		uint8_t val = in8(DIO_BASE + DIO_OFFS_C);
		out8(DIO_BASE + DIO_OFFS_C, val & ~BIT_1);
		HALmutex->unlock();
	}
}
/**
 * This function starts the engine in rigth direction
 */
void HALAktorik::engine_rigth() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	val = val | (BIT_0 & ~BIT_1);
	out8(DIO_BASE + DIO_OFFS_A, val);
	HALmutex->unlock();
}
/**
 * This function starts the engine in left direction
 */
void HALAktorik::engine_left() {
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
void HALAktorik::engine_slow(bool state) {
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
void HALAktorik::engine_stop() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_3);
	HALmutex->unlock();
}
/**
 * This function starts the engine
 */
void HALAktorik::engine_start() {
	HALmutex->lock();
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_3);
	HALmutex->unlock();
}
/**
 * This function resets all Bits
 */

void HALAktorik::resetAktorik(){
	HALmutex->lock();
	out8(DIO_BASE + DIO_OFFS_A,RESET_AKT);
	HALmutex->unlock();
}
