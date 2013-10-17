/*
 * ComponentTest.h
 *
 *  Created on: 17.10.2013
 *      Author: Rutkay
 */

#ifndef COMPONENTTEST_H_
#define COMPONENTTEST_H_

#include "HAL.h"

class ComponentTest {
public:
	ComponentTest();
	virtual ~ComponentTest();


	virtual void StartTest(HAL* hal);
};

#endif /* COMPONENTTEST_H_ */
