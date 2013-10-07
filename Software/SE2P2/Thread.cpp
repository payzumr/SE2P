/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick
                Philipp Kloth
                Rutkay Küpelikilinc
                Natalia Duske
 */

#include "Thread.h"
#include <iostream>
#include <unistd.h>
#include "HWaccess.h"
#include "IOMakros.h"

using namespace std;

namespace thread {

//Default Konstruktor der Klasse Thread
Thread::Thread() {
	// TODO Auto-generated constructor stub

}

// Default Destruktor Klasse Thread
Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void Thread::shutdown() {
    cout << "Shutting down..." << endl;
}

void Thread::execute(void *arg) {
    while(!isStopped()) {

    	GREEN(); // Gruen lechtet auf - geht aus (im Wechsel)
        sleep(1); // 1 Sekunde warten
    	YELLOW(); // Gelb leuchtet auf - geht aus (im Wechsel)
        usleep(500000); // 0,5 Sekunden warten
    	RED(); // Rot leuchtet auf - geht aus (im Wechsel) 
        usleep(500000); // 0,5 Sekunden warten
    }
}

}
