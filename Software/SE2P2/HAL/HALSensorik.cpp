/*
 * HALSensorik.cpp
 *
 *  Created on: 24.10.2013
 *      Author: Jannik
 */

#include "HALSensorik.h"

static int isr_coid;
hal::HALSensorik* hal::HALSensorik::instance = NULL;
Mutex* hal::HALSensorik::HAL_Smutex = new Mutex();

const struct sigevent* ISR(void* arg, int id) {
	struct sigevent *event = (struct sigevent *) arg;
	uint16_t val;
	uint8_t state;
	state = in8(DIO_BASE + OFFS_INT_STATUS) & (BIT_1 | BIT_3);//lesen
	out8(DIO_BASE + OFFS_INT_STATUS, 0);//zuruecksetzen

	switch (state) {
	case (BIT_1 | BIT_3):
		val = (in8(DIO_BASE + DIO_OFFS_B) << 8) | (in8(DIO_BASE + DIO_OFFS_C));
		SIGEV_PULSE_INIT(event,isr_coid,SIGEV_PULSE_PRIO_INHERIT, PB_STATUS | PC_STATUS, val);
		break;
	case BIT_1:
		val = in8(DIO_BASE + DIO_OFFS_B);
		SIGEV_PULSE_INIT(event,isr_coid,SIGEV_PULSE_PRIO_INHERIT, PB_STATUS, val);
		break;
	case BIT_3:
		val = in8(DIO_BASE + DIO_OFFS_C);
		SIGEV_PULSE_INIT(event,isr_coid,SIGEV_PULSE_PRIO_INHERIT, PC_STATUS, val);
		break;
	default:
		event = NULL;
	}
	return event;
}

hal::HALSensorik::HALSensorik() {
	//make sure HAL object is already created
	hal::HALSensorik::getInstance();
	initInterrupts();

	//create channel for dispatcher
	signalChid = ChannelCreate(0);
	if (signalChid == -1) {
		perror("Dispatcher: ChannelCreate signalChid failed");
		exit(EXIT_FAILURE);
	}

	signalCoid = ConnectAttach(0, 0, signalChid, _NTO_SIDE_CHANNEL, 0);
	if (signalCoid == -1) {
		perror("Dispatcher: ConnectAttach signalCoid failed");
		exit(EXIT_FAILURE);
	}
}

hal::HALSensorik* hal::HALSensorik::getInstance() {
	HAL_Smutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new HALSensorik();
	}
	HAL_Smutex->unlock();
	return instance;
}

hal::HALSensorik::~HALSensorik() {
	// TODO Auto-generated destructor stub
}

void hal::HALSensorik::initInterrupts() {
	// create channel to recive pulse message from the ISR
	isrChid = ChannelCreate(0);
	if (isrChid == -1) {
		perror("SensorikIntro: ChannelCreate isrChid failed");
		exit(EXIT_FAILURE);
	}
	isr_coid = ConnectAttach(0, 0, isrChid, _NTO_SIDE_CHANNEL, 0);
	if (isr_coid == -1) {
		perror("SensorikIntro: ConnectAttach isr_coid failed");
		exit(EXIT_FAILURE);
	}

	// attach ISRF
	SIGEV_PULSE_INIT(&event, isr_coid, SIGEV_PULSE_PRIO_INHERIT,0,0);
	interruptId = InterruptAttach(11, ISR, &event, sizeof(event), 0);
	if (interruptId == -1) {
		perror("SensorikIntro: ConnectAttach failed");
		exit(EXIT_FAILURE);
	}

	// configure interrupts

	//reset irq status register
	out8(DIO_BASE + OFFS_INT_STATUS, 0);
	// disable interrupts for all ports (Bit 0-5)
	uint8_t int_ctrl = in8(DIO_BASE + OFFS_INT_CTRL);
	out8(DIO_BASE + OFFS_INT_CTRL, int_ctrl | 0b00111111);
	// enable interrupt for portB und portC (Bit 1 und 2) (S. 18)
	int_ctrl = in8(DIO_BASE + OFFS_INT_CTRL);
	out8(DIO_BASE + OFFS_INT_CTRL, int_ctrl & ~(PB_CTRL | PC_CTRL));

	// read out port B and C values
	portBstatus = in8(DIO_BASE + DIO_OFFS_B);
	portCstatus = in8(DIO_BASE + DIO_OFFS_B);

}

void hal::HALSensorik::stop() {
	HAWThread::stop(); // super.stop();

	if (ConnectDetach(isr_coid)) {
		perror("SensorCtrl: ConnectDetatch isr_coid failed");
	}
	if (ChannelDestroy(isrChid) == -1) {
		perror("SensorCtrl: ChannelDestroy isrChid failed");
	}
	//in Simulation: bleibt hier haengen
	if (InterruptDetach(interruptId) == -1) {
		perror("SensorCtrl: InterruptDetach failed");
	}

}

void hal::HALSensorik::shutdown() {
}

void hal::HALSensorik::execute(void *arg) {
	cout << "| code  |  " << "value |" << endl;
	cout << "|----------------|" << endl;

	struct _pulse pulse;
	while (!isStopped()) {
		if (-1 == MsgReceivePulse(isrChid, &pulse, sizeof(pulse), NULL)) {
			if (isStopped()) {
				break; // channel destroyed, Thread ending
			}
			perror("SensorCtrl: MsgReceivePulse");
			exit(EXIT_FAILURE);
		}

		printf("|   %X   |   %2X   |", pulse.code, pulse.value.sival_int);
		cout << endl;
	}
}

