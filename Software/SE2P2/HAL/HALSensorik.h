/*
 * HALSensorik.h
 *
 *  Created on: 24.10.2013
 *      Author: Jannik
 */

#ifndef HALSENSORIK_H_
#define HALSENSORIK_H_


#include <stdint.h>
#include "HAWThread.h"
#include "Mutex.h"
#include "HWaccess.h"
#include "Addresses.h"
#include <unistd.h>
#include <stdint.h>

const struct sigevent * ISR (void *arg, int id);

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

    public:
        ~HALSensorik();
        static HALSensorik* getInstance();

        void stop(); // HAWThread: stop -> virtual

    protected:
        virtual void execute(void* arg);
        virtual void shutdown();

    private:
        void initInterrupts();
    };
}

#endif /* HALSENSORIK_H_ */
