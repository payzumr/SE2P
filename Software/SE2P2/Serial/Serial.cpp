/*
 * Serial.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Serial.h"
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <pthread.h>
#include <unistd.h>

using namespace hal;

int fd;

Mutex* Serial::Serialmutex = new Mutex();
Serial* Serial::instance = NULL;

Serial::Serial() {
	/*
	 fd = open_serial("dev/Ser1");
	 struct termios ts;
	 tcflush(fd, TCIOFLUSH);
	 tcgetattr(fd, &ts);
	 cfsetispeed(&ts, B19200); // input baut rate
	 cfsetospeed(&ts, B19200); // output baut rate
	 ts.c_cflag &= ~CSIZE; // clear number of data bits
	 ts.c_cflag &= ~CSTOPB;
	 ts.c_cflag &= ~PARENB; // clear number of data bits
	 ts.c_cflag |= CS8; // clear number of data bits
	 ts.c_cflag |= CREAD;
	 ts.c_cflag |= CLOCAL;
	 tcsetattr(fd, TCSANOW, &ts); // TCSANOW : The change is made immediately.
	 */
}

Serial::~Serial() {
	delete instance;
	instance = NULL;
	// TODO Auto-generated destructor stub
}

Serial* Serial::getInstance() {

	Serialmutex->lock();
	if (instance == NULL) {
		// Zugriffsrechte fuer den Zugriff auf die HW holen
		if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
			perror("ThreadCtl access failed\n");
			return NULL;
		}
		instance = new Serial();
	}
	Serialmutex->unlock();
	return instance;
}

void Serial::execute(void* arg) {

	if (instance->open_serial("/dev/ser1") < 0) {
		perror("open call failed");
	}
	while (!isStopped()) {
		char* msgIn[MSG_LENGTH];
		struct packet p;
		instance->read_serial(p);
		printf("Nachricht: %s\n", msgIn);
	}
}

void Serial::shutdown() {

	close_serial();
	cout << "shutdown Serial..." << endl;

}

/**
 * This method opens a connection via a serial port.
 * @param device: serial port that is connected to an other device
 * @return Filedeskriptor
 */
int Serial::open_serial(char* device) {
	// O_NOCTTY:
	// If set, and path identifies a terminal device, the open() function
	// doesn't cause the terminal device to become the controlling terminal for
	// the process.

	fd = open(device, O_RDWR | O_NOCTTY); // O_NOBLOCK => nicht blockierend

	return fd;
}
/**
 * Closes the connection to a device
 */
void Serial::close_serial() {
	if (close(fd) < 0) {
		perror("close failed");
	}
	fd = 0;
}
/**
 * Writes data from a Buffer buf with the length nbytes to the serial port.
 * @param 	buf: data
 * 			nbytes: number of bytes
 */
ssize_t Serial::write_serial_puk(struct Controller::puk* puk, STATUS s) {
	ssize_t returnV;
	struct packet p;
	p.PukId = puk->pukIdentifier;
	p.height1 = puk->height1;
	p.status = 1;
	p.type = puk->type;

	printf("Groesse:  : %d \n", sizeof(p));
	//printPacket(p);
	returnV = write(fd, &p, sizeof(p));// s. open, Sendepuffer, Anzahl der zu schreibenden Bytes
	printf("returnv %d\n", returnV);
	if (returnV < 0) {
		perror("write failed");
	}
	usleep(200000); // min. Zeit zwischen 2 Nachrichten
	return returnV;

}

ssize_t Serial::write_serial_stop() {
	ssize_t returnVal;

	struct packet p;
	p.status = 1;
	p.height1 = 0;
	p.type = 0;
	p.PukId = 0;

	returnVal = write(fd, &p, sizeof(p));
	if (returnVal < 0) {
		perror("write failed");
	}
	return returnVal;
}

ssize_t Serial::write_serial_ack(uint8_t ack) {
	ssize_t returnVal;

	return returnVal;
}

/**
 * Reads data to a Buffer buf with the length "length" from the serial port.
 * @param 	buf: buffer for incoming data
 * 			lentgh: number of bytes
 */
int Serial::read_serial(struct packet p) {
	HALSensorik* hal_S = HALSensorik::getInstance();

	int i_coid;
	i_coid = hal_S->getSignalCoid();

	int returnV = 0;
	while (returnV < sizeof(p)) {
		returnV += read(fd, &p + returnV, sizeof(p) - returnV); //blockieren, bis etwas gesendet wurde
		if (returnV < 0) {
			perror("read failed");
		}

		printPacket(&p);

	}
		switch (p.status) {
		case (1):
			MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, BUTTONS, ESTOPBUTTON);
			//e-stopp betätigen
		case (2):
			break;//puk daten eintragen
		case (3):
			break;//ack bearbeiten
		}
		return returnV;
}

void Serial::printPacket(struct packet* p) {

	printf("Status: %d\n PukId: %d\n Puktype: %d\n Hoehe: %d\n", p->status,
			p->PukId, p->type, p->height1);
}

