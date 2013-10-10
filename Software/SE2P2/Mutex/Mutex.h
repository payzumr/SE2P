/*
 * Mutex.h
 *
 *  Created on: 10.10.2013
 *      Author: Jannik
 */
#include <pthread.h>

#ifndef MUTEX_H_
#define MUTEX_H_

class Mutex {

public:

	Mutex();
	~Mutex();
	void lock();
	void unlock();
};

#endif /* MUTEX_H_ */
