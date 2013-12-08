/*
 * Test.cpp
 *
 *  Created on: 17.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */


#include "../Thread.h"
#include "Test.h"
#include <iostream>
#include <unistd.h>
#include "../HAW/HWaccess.h"
#include "../HAL/Addresses.h"
#include "Controller1.h"

#define MSG_LENGTH 20
#define SLEEP_ONE_SEC 1

#define TEST_MESSAGES
#define READ
//#define WRITE

//PulseMessage Bitmasken defines; Neutral(Alles aus = 11001111)
#define EINLAUF 		0xCE   		//1100 1110
#define AUSLAUF 		0x4F 		//0100 1111
#define HOEHENMESSUNG 	0xCD		//1100 1101
#define W_IN_WEICHE		0xC7		//1100 0111
#define WEICHEOFFEN		0xEF		//1110 1111
#define METALLSENSOR	0xDF		//1101 1111
#define RUTSCHEVOLL		0x8F		//1000 1111
//Neutral = 10100000
#define STARTTASTE		0xB0		//1011 0000
#define STOPTASTE		0x80		//1000 0000
#define RESETTASTE		0xE0		//1110 0000
#define ESTOPTASTE		0x20		//0010 0000

using namespace hal;


Test::Test() {
	// TODO Auto-generated constructor stub

}

Test::~Test() {
	// TODO Auto-generated destructor stub
}
/**
 *  testing all the aktoric Components, waits 1sec after each test
 */

void Test::componentTest(HALAktorik* hal) {
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
	struct Controller1::puk p;


	p.height1 = 2500;
	p.height2 = 0;
	p.metall = true;
	p.place = 1;
	p.pukIdentifier = 99;
	p.type = withMetal;


	//serial->write_serial(&p, Serial::Data );


}

void Test::sensorikPulseTest(HALSensorik* halS){
	uint8_t PortA = 2;
	uint8_t PortB = 8;

	MachineState* M = MachineState::getInstance();

	int i_coid;
	i_coid = halS->getSignalCoid();

	cout << "Sensorik Pulse Test Start!" << endl;
	cout << "Machine State vor Test" << endl;
	M->showMachineState();
	sleep(1);

	//Port A
	cout << "Einlauf Werkstueck" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA , EINLAUF); //0,1,3,4,5,6,7
	M->showMachineState();
	if(M->sensEntry == true && M->sensHeight == false && M->sensSwitch == false && M->sensMetall == false&& M->switchOpen == false && M->sensSlip == false && M->sensExit == false){
		cout << "Testergebniss Einlauf: OK" << endl;
	}else{
		cout << "Testergebniss Einlauf: Failed" << endl;
	}
	sleep(1);


	cout << "Werkstueck in Hoehenmessung" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA, HOEHENMESSUNG);
	M->showMachineState();
	if(M->sensEntry == false && M->sensHeight == true && M->sensSwitch == false && M->sensMetall == false && M->switchOpen == false && M->sensSlip == false && M->sensExit == false){
		cout << "Testergebniss Hoehenmessung: OK" << endl;
	}else{
		cout << "Testergebniss Hoehenmessung: Failed" << endl;
	}
	sleep(1);


	cout << "Werkstueck in Weiche" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA, W_IN_WEICHE);
	M->showMachineState();
	if(M->sensEntry == false && M->sensHeight == false && M->sensSwitch == true && M->sensMetall == false && M->switchOpen == false && M->sensSlip == false && M->sensExit == false){
		cout << "Testergebniss Weiche: OK" << endl;
	}else{
		cout << "Testergebniss Weiche: Failed" << endl;
	}
	sleep(1);


	cout << "Werkstueck Metallsensor" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA, METALLSENSOR);
	M->showMachineState();
	if(M->sensEntry == false && M->sensHeight == false && M->sensSwitch == false && M->sensMetall == true && M->switchOpen == false && M->sensSlip == false && M->sensExit == false){
		cout << "Testergebniss Metallsensor: OK" << endl;
	}else{
		cout << "Testergebniss Metallsensor: Failed" << endl;
	}
	sleep(1);


	cout << "Weiche offen" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA, WEICHEOFFEN);
	M->showMachineState();
	if(M->sensEntry == false && M->sensHeight == false && M->sensSwitch == false && M->sensMetall == false && M->switchOpen == true && M->sensSlip == false && M->sensExit == false){
		cout << "Testergebniss Weiche offen: OK" << endl;
	}else{
		cout << "Testergebniss Weiche offen: Failed" << endl;
	}
	sleep(1);


	cout << "Rutsche voll" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA, RUTSCHEVOLL);
	M->showMachineState();
	if(M->sensEntry == false && M->sensHeight == false && M->sensSwitch == false && M->sensMetall == false && M->switchOpen == false && M->sensSlip == true && M->sensExit == false){
		cout << "Testergebniss Rutsche: OK" << endl;
	}else{
		cout << "Testergebniss Rutsche: Failed" << endl;
	}
	sleep(1);


	cout << "Auslauf Werkstueck" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortA, AUSLAUF);
	M->showMachineState();
	if(M->sensEntry == false && M->sensHeight == false && M->sensSwitch == false && M->sensMetall == false && M->switchOpen == false && M->sensSlip == false && M->sensExit == true){
		cout << "Testergebniss Auslauf: OK" << endl;
	}else{
		cout << "Testergebniss Auslauf: Failed" << endl;
	}
	sleep(1);
	//Port B
	cout << "Starttaste" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortB, STARTTASTE); //4,5,6,7
	M->showMachineState();

	sleep(1);


	cout << "Stoptaste" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortB, STOPTASTE);
	M->showMachineState();
	sleep(1);


	cout << "Resettaste" << endl;
	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortB, RESETTASTE);
	M->showMachineState();
	sleep(1);


//	cout << "E-Stoptaste" << endl;
//	MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, PortB, ESTOPTASTE);
//	M->showMachineState();
//	if(M->SensEntry == false && M->SensHeight == false && M->SensSwitch == false && M->SensMetall == false && M->SwitchOpen == false && M->SensSlip == false && M->SensExit == false && M->E_Stop == true){
//		cout << "Testergebniss E-Stop: OK" << endl;
//	}else{
//		cout << "Testergebniss E-Stop: Failed" << endl;
//	}
//	sleep(2);
}

void Test::controllerTest(){
	//eventuell den Controller durchgehen und am ende gucken ob der puk die richtigen eigenschaften hat??
	//also nur die Controller Funktionen nacheinander aufrufen und vor der HeigthMeasure den richtigen höhenwert setzen
}

