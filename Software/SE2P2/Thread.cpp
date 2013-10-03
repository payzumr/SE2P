/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik
 */

#include "Thread.h"
#include <iostream>
#include <unistd.h>
#include "HWaccess.h"
#include "IOMakros.h"

using namespace std;

namespace thread {

Thread::Thread() {
	// TODO Auto-generated constructor stub

}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void Thread::shutdown() {
    cout << "Shutting down..." << endl;
}

void Thread::execute(void *arg) {
    while(!isStopped()) {

    	GREEN();
        sleep(1);
    	YELLOW();
        usleep(500000);
    	RED();
        usleep(500000);
    }
}

}
