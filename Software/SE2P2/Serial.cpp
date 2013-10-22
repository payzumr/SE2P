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

int fd;

Mutex* Serial::Serialmutex = new Mutex();
Serial* Serial::instance = NULL;

Serial::Serial() {
	// TODO Auto-generated constructor stub

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
ssize_t Serial::write_serial(void* buf, size_t nbytes) {
	ssize_t returnV;
	returnV = write(fd, buf, nbytes);// s. open, Sendepuffer, Anzahl der zu schreibenden Bytes
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
int Serial::read_serial(void* buf, int length) {
	int returnV = 0;
	while(returnV < length){
	returnV = returnV + read(fd, buf[returnV], length-returnV); //blockieren, bis etwas gesendet wurde

	}
	if (returnV < 0) {
		perror("read failed");
	}
	return returnV;
}

