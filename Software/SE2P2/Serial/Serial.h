/*
 * Serial.h
 *
 *  Created on: 17.10.2013
 *      Author: Jannik
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "../HAW/HWaccess.h"
#include "../HAL/Addresses.h"
#include "HAWThread.h"
#include <HALSensorik.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../Mutex/Mutex.h"
//#ifdef BAND_1
#include "Controller1.h"
//#endif

#define MSG_LENGTH 20

class Serial: public thread::HAWThread {
private:
	static Serial* instance;
	static Mutex* Serialmutex;
	int fd;//filedescriptor->schnittstelle
	Serial();
	~Serial();
public:
	enum STATUS{ Stop, Data };

	struct packet{
		uint8_t status; //1=Stop;2=Data;3=Ack
		uint8_t pukId;
		uint16_t height1;
		uint8_t type; //1=metall;2=ohneMetall;3=undefined
		uint8_t acktype; //1 ready 2 not ready
	};

	static Serial* getInstance();


	int open_serial(char* device);
	void close_serial();
//#ifdef BAND_1
	ssize_t write_serial_puk(struct Controller1::puk* p, uint8_t s);
//#endif
	ssize_t write_serial_stop();
	ssize_t write_serial_ack(uint8_t ack);
	int read_serial(struct packet p);
	void printPacket(struct packet* p);

protected:
	virtual void execute(void* arg);
	virtual void shutdown();


};

#endif /* SERIAL_H_ */
