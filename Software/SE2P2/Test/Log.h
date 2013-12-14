/*
 * Log.h
 *
 *  Created on: 14.12.2013
 *      Author: Rutkay
 */

#include "HAWThread.h"
#include "Mutex.h"

#ifndef LOG_H_
#define LOG_H_

class Log: public thread::HAWThread{
private:
	static Mutex* logMutex;
public:
	virtual ~Log();
	Log();
	void LogMe(string);
	void ShowLog();
	void StopLog();

protected:
	virtual void execute(void* arg);
	virtual void shutdown();
};

#endif /* LOG_H_ */
