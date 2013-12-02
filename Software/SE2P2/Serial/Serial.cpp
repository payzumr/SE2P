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

int fd;

Mutex* Serial::Serialmutex = new Mutex();
Serial* Serial::instance = NULL;

Serial::Serial() {

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

void Serial::execute(void* arg){


	if (instance->open_serial("/dev/ser1") < 0) {
			perror("open call failed");
	}

		char* msgIn[MSG_LENGTH];
		struct packet p;
		instance->read_serial(p);
		printf("Nachricht: %s\n", msgIn);
}

void Serial::shutdown(){

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
ssize_t Serial::write_serial(struct Controller::puk* puk, STATUS s ) {
	ssize_t returnV;
	struct packet* p;
	p->PukId = puk->pukIdentifier;
	p->height1 = puk->height1;
	p->st = s;
	p->type = puk->type;


	returnV = write(fd, &p , sizeof(p));// s. open, Sendepuffer, Anzahl der zu schreibenden Bytes
	if (returnV < 0) {
		perror("write failed");
	}
	usleep(200000); // min. Zeit zwischen 2 Nachrichten
	return returnV;

}
/**
 * Reads data to a Buffer buf with the length "length" from the serial port.
 * @param 	buf: buffer for incoming data
 * 			lentgh: number of bytes
 */
int Serial::read_serial(struct packet p) {
	int returnV = 0;
	while(returnV < sizeof(p)){
	returnV += read(fd, &p + returnV, sizeof(p)-returnV); //blockieren, bis etwas gesendet wurde
	printf("gelesen: %d\n", returnV);
	}
	if (returnV < 0) {
		perror("read failed");
	}
	printPacket(&p);

	return returnV;
}

void Serial::printPacket(struct packet* p){
	printf("Status: ", (p->st == Stop)? "Stop" : "Data", "\n",
		   "PukId: %d\n" , p->PukId,
		   "Puktype: ", (p->type == tall) ? "tall\n" : (p->type == withHole) ? "withHole\n" : "withMetal\n",
		   "Hoehe: %d", p->height1);
}

