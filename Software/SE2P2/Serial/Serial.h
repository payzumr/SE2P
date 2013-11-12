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
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../Mutex/Mutex.h"

class Serial {
private:
	static Serial* instance;
	static Mutex* Serialmutex;
	int fd;//filedescriptor->schnittstelle
	Serial();
	~Serial();
public:
	static Serial* getInstance();

	int open_serial(char* device);
	void close_serial();
	ssize_t write_serial(void* buf, size_t nbytes);
	int read_serial(void* buf, int length);

};

#endif /* SERIAL_H_ */
