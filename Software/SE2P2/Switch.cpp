/*
 * Switch.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Switch.h"
#include "HWaccess.h"
#include "Addresses.h"

#include <stdint.h>

Switch* Switch::instance = NULL;

Switch::Switch() {
	// Initialisierung der Digitalen IO Karte
	out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);
	out8(DIO_BASE + DIO_OFFS_A, 0x00);
	out8(DIO_BASE + DIO_OFFS_C, 0x00);
}

Switch::~Switch() {
	delete instance;
	instance = NULL;
}

Switch* Switch::getInstance() {
	// Zugriffsrechte fuer den Zugriff auf die HW holen
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return NULL;
	}

	if (instance == NULL) {
		instance = new Switch();
	}
	return instance;

}

void Switch::switchOpen() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val | BIT_4);

}
void Switch::switchClose() {
	uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_4);

}
