/*
 * Test.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef TEST_H_
#define TEST_H_

#include "HAL.h"
#include "Serial.h"

class Test {
public:
	Test();
	virtual ~Test();


	virtual void componentTest(HAL* hal);
	virtual void serialTest(Serial* serial);
};

#endif /* TEST_H_ */
