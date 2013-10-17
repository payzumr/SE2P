/*
 * ComponentTest.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Rutkay
 */


#include "Thread.h"
#include "ComponentTest.h"
#include <iostream>
#include <unistd.h>
#include "HWaccess.h"


ComponentTest::ComponentTest() {
	// TODO Auto-generated constructor stub

}

ComponentTest::~ComponentTest() {
	// TODO Auto-generated destructor stub
}


void ComponentTest::StartTest(HAL* hal) {
		hal->greenOn();
		#ifdef TEST_MESSAGES
		cout<<"green on"<<endl;
		#endif
		sleep(1);
		hal->greenOff();
		#ifdef TEST_MESSAGES
		cout<<"green off"<<endl;
		#endif
		sleep(1);
		hal->yellowOn();
		#ifdef TEST_MESSAGES
		cout<<"yellow on"<<endl;
		#endif
		sleep(1);
		hal->yellowOff();
		#ifdef TEST_MESSAGES
		cout<<"yellow off"<<endl;
		#endif
		sleep(1);
		hal->redOn();
		#ifdef TEST_MESSAGES
		cout<<"red on"<<endl;
		#endif
		sleep(1);
		hal->redOff();
		#ifdef TEST_MESSAGES
		cout<<"red off"<<endl;
		#endif
		sleep(1);
		hal->switchOpen();
		#ifdef TEST_MESSAGES
		cout<<"switch open"<<endl;
		#endif
		sleep(1);
		hal->switchClose();
		#ifdef TEST_MESSAGES
		cout<<"switch close"<<endl;
		#endif
		sleep(1);
		hal->engine_rigth();
		#ifdef TEST_MESSAGES
		cout<<"engine right"<<endl;
		#endif
		sleep(1);
		hal->engine_stop();
		#ifdef TEST_MESSAGES
		cout<<"engine stop"<<endl;
		#endif
		sleep(1);
		hal->engine_left();
		#ifdef TEST_MESSAGES
		cout<<"engine left"<<endl;
		#endif
		sleep(1);
		hal->engine_start();
		#ifdef TEST_MESSAGES
		cout<<"engine start"<<endl;
		#endif
		sleep(1);
		hal->engine_slowON();
		#ifdef TEST_MESSAGES
		cout<<"engine slow on"<<endl;
		#endif
		sleep(1);
		hal->engine_slowOFF();
		#ifdef TEST_MESSAGES
		cout<<"engine slow off"<<endl;
		#endif
		sleep(1);
		hal->engine_slowOFF();
		#ifdef TEST_MESSAGES
		cout<<"engine slow off"<<endl;
		#endif
		sleep(1);
	}

