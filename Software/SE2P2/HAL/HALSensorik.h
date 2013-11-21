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
#include "MachineState.h"
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

	HALSensorik();

	void initInterrupts();
public:
	~HALSensorik();
	static HALSensorik* getInstance();
	int getHeight();
	int getSignalChid();

	void stop(); // HAWThread: stop -> virtual

protected:
	virtual void execute(void* arg);
	virtual void shutdown();

};
}

#endif /* HALSENSORIK_H_ */
