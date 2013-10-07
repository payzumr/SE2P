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

//Klasse Thread abgeleitet von HAWThread + Definitionen

class Thread: public HAWThread {
public:
	Thread();
	virtual ~Thread();

	//Die Methoden werden von HAWThread gefordert
    virtual void execute(void*arg); // Was soll gemacht werden während der Thread läuft?
    virtual void shutdown(); // Was soll gemacht werden wenn der Thread beendet wird?

};

}

#endif /* THREAD_H_ */
