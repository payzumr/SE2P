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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../Mutex/Mutex.h"
#include "Controller.h"

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
		STATUS st;
		int PukId;
		int height1;
		pukType type;

		uint32_t checksum;
	};

	static Serial* getInstance();


	int open_serial(char* device);
	void close_serial();
	ssize_t write_serial(struct Controller::puk* p, STATUS s);
	int read_serial(struct packet p);
	void printPacket(struct packet* p);

protected:
	virtual void execute(void* arg);
	virtual void shutdown();


};

#endif /* SERIAL_H_ */
