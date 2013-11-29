/*
 * HAL.h
 *
 *  Created on: 10.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */


#ifndef HAL_H_
#define HAL_H_

#include "../Mutex/Mutex.h"
#include "../HAW/HWaccess.h"
#include "Addresses.h"
#include <unistd.h>
#include <stdint.h>
#include <iostream>//?????

class HALAktorik {
private:
	static HALAktorik* instance;
	static Mutex* HALmutex;
	HALAktorik();
	~HALAktorik();
public:

	static HALAktorik* getInstance();

	//lights
	void redLigths(bool state);
	void yellowLigths(bool state);
	void greenLigths(bool state);

	//conveyer
    void engine_left();
    void engine_rigth();
    void engine_slow(bool state);
    void engine_stop();
    void engine_start();

    //LED's
    void led_Q1(bool state);
    void led_Q2(bool state);
    void led_Start(bool state);
    void led_Reset(bool state);
    //Switch
	void switchOnOff(bool state);
};
#endif /* HAL_H_ */
