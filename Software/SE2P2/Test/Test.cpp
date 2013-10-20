/*
 * Test.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */


#include "Thread.h"
#include "Test.h"
#include <iostream>
#include <unistd.h>
#include "HWaccess.h"
#include "Addresses.h"

#define TEST_MESSAGES
//#define READ
#define WRITE

Test::Test() {
	// TODO Auto-generated constructor stub

}

Test::~Test() {
	// TODO Auto-generated destructor stub
}


void Test::componentTest(HAL* hal) {
		hal->greenLigths(ON);
		#ifdef TEST_MESSAGES
		cout<<"green on"<<endl;
		#endif
		sleep(1);
		hal->greenLigths(OFF);
		#ifdef TEST_MESSAGES
		cout<<"green off"<<endl;
		#endif
		sleep(1);
		hal->yellowLigths(ON);
		#ifdef TEST_MESSAGES
		cout<<"yellow on"<<endl;
		#endif
		sleep(1);
		hal->yellowLigths(OFF);
		#ifdef TEST_MESSAGES
		cout<<"yellow off"<<endl;
		#endif
		sleep(1);
		hal->redLigths(ON);
		#ifdef TEST_MESSAGES
		cout<<"red on"<<endl;
		#endif
		sleep(1);
		hal->redLigths(OFF);
		#ifdef TEST_MESSAGES
		cout<<"red off"<<endl;
		#endif
		sleep(1);
		hal->switchOnOff(ON);
		#ifdef TEST_MESSAGES
		cout<<"switch open"<<endl;
		#endif
		sleep(1);
		hal->switchOnOff(OFF);
		#ifdef TEST_MESSAGES
		cout<<"switch close"<<endl;
		#endif
		sleep(1);
		hal->engine_rigth();
		#ifdef TEST_MESSAGES
		cout<<"engine right"<<endl;
		#endif
		sleep(1);
		sleep(1);
		hal->engine_left();
		#ifdef TEST_MESSAGES
		cout<<"engine left"<<endl;
		#endif
		sleep(1);
		hal->engine_stop();
		#ifdef TEST_MESSAGES
		cout<<"engine stop"<<endl;
		#endif
		sleep(1);
		hal->engine_start();
		#ifdef TEST_MESSAGES
		cout<<"engine start"<<endl;
		#endif
		sleep(1);
		hal->engine_slow(ON);
		#ifdef TEST_MESSAGES
		cout<<"engine slow on"<<endl;
		#endif
		sleep(1);
		hal->engine_slow(OFF);
		#ifdef TEST_MESSAGES
		cout<<"engine slow off"<<endl;
		#endif
		sleep(1);
	}

void Test::serialTest(Serial* serial){

	serial->open_serial("dev/Serial1");
#ifdef READ
	char* msgIn[20];
	serial->read_serial(msgIn, sizeof(msgIn));
	printf("Nachricht: %s", msgIn);
#endif

#ifdef WRITE
	char* msgOut = "Hallo Maschine";
	serial->write_serial(msgOut, sizeof(msgOut));
#endif

	serial->close_serial();
}


