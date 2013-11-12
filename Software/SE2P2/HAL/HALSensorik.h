/*
 * HALSensorik.h
 *
 *  Created on: 24.10.2013
 *      Author: Jannik
 */

#ifndef HALSENSORIK_H_
#define HALSENSORIK_H_

#include <stdint.h>
#include "../HAW/HAWThread.h"
#include "../Mutex/Mutex.h"
#include "../HAW/HWaccess.h"
#include "Addresses.h"
#include <unistd.h>
#include <stdint.h>
#include <time.h>

const struct sigevent * ISR(void *arg, int id);

namespace hal {

class HALSensorik: public thread::HAWThread {
private:
	static HALSensorik* instance;
	static Mutex* HAL_Smutex;

	int isrChid;
	int interruptId;
	struct sigevent event;

	int signalChid;
	int signalCoid;

	uint8_t portBstatus;
	uint8_t portCstatus;

	bool portB_0;
	bool portB_1;
	bool portB_2;
	bool portB_3;
	bool portB_4;
	bool portB_5;
	bool portB_6;
	bool portB_7;

	bool portC_4;
	bool portC_5;
	bool portC_6;
	bool portC_7;

	HALSensorik();

public:
	~HALSensorik();
	static HALSensorik* getInstance();
	int getHeight();

	void stop(); // HAWThread: stop -> virtual

protected:
	virtual void execute(void* arg);
	virtual void shutdown();

private:
	void initInterrupts();
	void printSensorChanges(int code, int val);
	void hal::HALSensorik::set_timer();
};
}

#endif /* HALSENSORIK_H_ */
