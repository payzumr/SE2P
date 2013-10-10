/*
 * Lights.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */
#include "Lights.h"
#include "HWaccess.h"
#include "Addresses.h"

#include <stdint.h>

Lights* Lights::instance = NULL;

Lights::Lights() {
	// Initialisierung der Digitalen IO Karte
	out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);
	out8(DIO_BASE + DIO_OFFS_A, 0x00);
	out8(DIO_BASE + DIO_OFFS_C, 0x00);
}

Lights::~Lights() {
	delete instance;
	instance = NULL;
}

Lights* Lights::getInstance() {
	// Zugriffsrechte fuer den Zugriff auf die HW holen
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return NULL;
	}

	if (instance == NULL) {
		instance = new Lights();
	}
	return instance;

}

void Lights::redOn() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_7);
}

void Lights::redOff() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_7);
}

void Lights::yellowOn() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_6);

}
void Lights::yellowOff() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_6);

}
void Lights::greenOn() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_5);

}
void Lights::greenOff() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_5);

}
