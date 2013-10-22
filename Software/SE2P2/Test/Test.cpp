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

#define MSG_LENGTH 20
#define SLEEP_ONE_SEC 1

#define TEST_MESSAGES
//#define READ
#define WRITE

Test::Test() {
	// TODO Auto-generated constructor stub

}

Test::~Test() {
	// TODO Auto-generated destructor stub
}
/**
 *  testing all the aktoric Components, waits 1sec after each test
 */

void Test::componentTest(HAL* hal) {
	hal->led_Q1(ON);
#ifdef TEST_MESSAGES
	cout << "LED Q1 on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Q1(OFF);
#ifdef TEST_MESSAGES
	cout << "LED Q1 off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Q2(ON);
#ifdef TEST_MESSAGES
	cout << "LED Q2 on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Q2(OFF);
#ifdef TEST_MESSAGES
	cout << "LED Q2 off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Start(ON);
#ifdef TEST_MESSAGES
	cout << "LED Start on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Start(OFF);
#ifdef TEST_MESSAGES
	cout << "LED Start off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Reset(ON);
#ifdef TEST_MESSAGES
	cout << "LED Reset on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->led_Reset(OFF);
#ifdef TEST_MESSAGES
	cout << "LED Reset off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->greenLigths(ON);
#ifdef TEST_MESSAGES
	cout << "green on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->greenLigths(OFF);
#ifdef TEST_MESSAGES
	cout << "green off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->yellowLigths(ON);
#ifdef TEST_MESSAGES
	cout << "yellow on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->yellowLigths(OFF);
#ifdef TEST_MESSAGES
	cout << "yellow off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->redLigths(ON);
#ifdef TEST_MESSAGES
	cout << "red on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->redLigths(OFF);
#ifdef TEST_MESSAGES
	cout << "red off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->switchOnOff(ON);
#ifdef TEST_MESSAGES
	cout << "switch open" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->switchOnOff(OFF);
#ifdef TEST_MESSAGES
	cout << "switch close" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_rigth();
#ifdef TEST_MESSAGES
	cout << "engine right" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_stop();
#ifdef TEST_MESSAGES
	cout << "engine stop" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_start();
#ifdef TEST_MESSAGES
	cout << "engine start" << endl;
#endif
	hal->engine_left();
#ifdef TEST_MESSAGES
	cout << "engine left" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_stop();
#ifdef TEST_MESSAGES
	cout << "engine stop" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_start();
#ifdef TEST_MESSAGES
	cout << "engine start" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_slow(ON);
#ifdef TEST_MESSAGES
	cout << "engine slow on" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
	hal->engine_slow(OFF);
#ifdef TEST_MESSAGES
	cout << "engine slow off" << endl;
#endif
	sleep(SLEEP_ONE_SEC);
}
/**
 * testing to send a char to the other device
 *
 */
void Test::serialTest(Serial* serial){

#ifdef READ
	char* msgIn[MSG_LENGTH];
	serial->read_serial(msgIn, MSG_LENGTH);
	printf("Nachricht: %s", msgIn);
#endif

#ifdef WRITE
	char* msgOut = "Hallo Maschine\n";
	serial->write_serial(msgOut, MSG_LENGTH);
#ifdef TEST_MESSAGES
	cout << "send Message" << endl;
#endif
#endif

}


