/*
 * HAL.h
 *
 *  Created on: 10.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include <iostream>

#ifndef HAL_H_
#define HAL_H_

#include "Mutex.h"

class HAL {
private:
	static HAL* instance;
	static Mutex* HALmutex;
	HAL();
	~HAL();
public:

	static HAL* getInstance();

	//lights
	void redOn();
	void redOff();
	void yellowOn();
	void yellowOff();
	void greenOn();
	void greenOff();

	//conveyer
    void engine_left();
    void engine_rigth();
    void engine_slowON();
    void engine_slowOFF();
    void engine_stop();
    void engine_start();

    //Switch
	void switchOpen();
	void switchClose();
};

#endif /* HAL_H_ */
