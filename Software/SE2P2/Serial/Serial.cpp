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
		struct packet p;
		instance->read_serial(p);
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

ssize_t Serial::write_serial_puk(struct Controller1::puk* puk) {
	ssize_t returnV;
	struct packet p;
	p.pukId = puk->pukIdentifier;
	p.height1 = puk->height1;
	p.status = 2;
	p.type = puk->type;

	returnV = write(fd, &p, sizeof(p));// s. open, Sendepuffer, Anzahl der zu schreibenden Bytes
	if (returnV < 0) {
		perror("write failed");
	}
	usleep(200000); // min. Zeit zwischen 2 Nachrichten
	return returnV;

}
/**
 * Status = 1
 *
 */

ssize_t Serial::write_serial_stop() {
	ssize_t returnVal;

	struct packet p;
	p.status = 1;
	p.height1 = 0;
	p.type = 0;
	p.pukId = 0;
	p.acktype = 0;

	returnVal = write(fd, &p, sizeof(p));
	if (returnVal < 0) {
		perror("write failed");
	}
	usleep(200000); // min. Zeit zwischen 2 Nachrichten
	return returnVal;
}
/*
 * Ack Types
 * Status 3
 * 1 = Band 2 ready
 * 2 = Band 2 not ready
 */

ssize_t Serial::write_serial_ack(uint8_t ack) {
	ssize_t returnVal;
	struct packet p;

	p.status = 3;
	p.height1 = 0;
	p.type = 0;
	p.pukId = 0;

	switch (ack) {
	case (1):
		p.acktype = 1;
		break;
	case (2):
		p.acktype = 2;
	}

	returnVal = write(fd, &p, sizeof(p));
	if (returnVal < 0) {
		perror("write failed");
	}
	usleep(200000); // min. Zeit zwischen 2 Nachrichten
	return returnVal;
}

/**
 * Reads data to a Buffer buf with the length "length" from the serial port.
 * @param 	buf: buffer for incoming data
 * 			lentgh: number of bytes
 */
int Serial::read_serial(struct packet p) {
#ifdef BAND_1
	Controller1* con = Controller1::getInstance();
#endif

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
	/*
	 * 1= E-Stop 2=Data 3=Ack
	 */
	switch (p.status) {
	case (1):
		MsgSendPulse(i_coid, SIGEV_PULSE_PRIO_INHERIT, BUTTONS, ESTOPBUTTON);
	case (2):
		if (con->pukArr[0].pukIdentifier == -1) {
			write_serial_ack(2);
		}else{
			con->pukArr[0].pukIdentifier = p.pukId;
			con->pukArr[0].height1 = p.height1;
			(p.type == 1)? con->pukArr[0].type = tall:(p.type == 2)? con->pukArr[0].type = withHole : con->pukArr[0].type = withMetal;
			write_serial_ack(1);
			//starten
		}
	case (3):
			if(p.acktype == 1){
				con->ack = false;
			}else{
				usleep(5000);
			}

	}
	return returnV;
}

void Serial::printPacket(struct packet* p) {

	printf("Status: %d\n PukId: %d\n Puktype: %d\n Hoehe: %d\n ACKtype: %d\n", p->status,
			p->pukId, p->type, p->height1, p->acktype);
}

