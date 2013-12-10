/*
 * LightControl.h
 *
 *  Created on: 06.12.2013
 *      Author: Jannik
 */
#include "HAWThread.h"
#include "Mutex.h"
#include "Addresses.h"

#ifndef LIGHTCONTROL_H_
#define LIGHTCONTROL_H_

class LightControl: public thread::HAWThread {
private:
	static LightControl* instance;
	static Mutex* lightMutex;
	int chid;
	void initPulseMessage();
	LightControl();
public:
	static LightControl* getInstance();
	virtual ~LightControl();
	void flashYellow();
	void flashRedSlow();
	void flashRedFast();
	void flashGreenIni();
	void stopLights();
protected:
	virtual void execute(void* arg);
	virtual void shutdown();
};

#endif /* LIGHTCONTROL_H_ */
