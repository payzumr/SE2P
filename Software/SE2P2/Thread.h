/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
                Philipp Kloth (2081738)
                Rutkay Kuepelikilinc (2081831)
                Natalia Duske (2063265)
 */

#ifndef THREAD_H_
#define THREAD_H_
#define ONE_SEC 1

#include "HAWThread.h"
#include "MachineState.h"

namespace thread {

//Klasse Thread abgeleitet von HAWThread + Definitionen

class Thread: public HAWThread {
public:
	Thread();
	virtual ~Thread();

	//Die Methoden werden von HAWThread gefordert
    virtual void execute(void*arg);
    virtual void shutdown(); 

};

}

#endif /* THREAD_H_ */
