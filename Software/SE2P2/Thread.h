/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick
                Philipp Kloth
                Rutkay Küpelikilinc
                Natalia Duske
 */

#ifndef THREAD_H_
#define THREAD_H_


#include "HAWThread.h"

namespace thread {

class Thread: public HAWThread {
public:
	Thread();
	virtual ~Thread();

    virtual void execute(void*arg);
    virtual void shutdown();

};

}

#endif /* THREAD_H_ */
