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
	void redLigths(bool state);
	void yellowLigths(bool state);
	void greenLigths(bool state);

	//conveyer
    void engine_left();
    void engine_rigth();
    void engine_slow(bool state);
    void engine_stop();
    void engine_start();

    //Switch
	void switchOnOff(bool state);
};
#endif /* HAL_H_ */
