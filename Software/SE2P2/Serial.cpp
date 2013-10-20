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

Serial::Serial() {
	// TODO Auto-generated constructor stub

}

Serial::~Serial() {
	// TODO Auto-generated destructor stub
}
	int Serial::open_serial(char* device) {
		// O_NOCTTY:
		// If set, and path identifies a terminal device, the open() function
		// doesn't cause the terminal device to become the controlling terminal for
		// the process.

		fd = open(device, O_RDWR | O_NOCTTY); // O_NOBLOCK => nicht blockierend
		if (fd < 0) {
			perror(device);
			return 0;
		}
		return 1;
	}
	void Serial::close_serial() {
		if(close(fd)<0){
			perror("close failed");
		}
		fd = 0;
	}
	ssize_t Serial::write_serial(void* buf, size_t nbytes) {
		ssize_t returnV;
		returnV = write(fd, buf, nbytes);// s. open, Sendepuffer, Anzahl der zu schreibenden Bytes
		if(returnV<0){
			perror("write failed");
		}
		usleep(200000); // min. Zeit zwischen 2 Nachrichten
		return returnV;

	}
	int Serial::read_serial(void* buf, int length) {
		int returnV;
		returnV = readcond(fd, buf, length, length, // kein min/max
				1, // max. 0.1 Sekunden zwischen 2 zusammengehörigen Bytes
				0); // ohne Timeout blockieren, bis etwas gesendet wurde
		if(returnV<0){
					perror("read failed");
				}
		return returnV;
	}

