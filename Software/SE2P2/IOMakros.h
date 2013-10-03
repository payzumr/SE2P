/*
 * IOMakros.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik
 */


#include "HWaccess.h"
#ifndef IOMAKROS_H_
#define IOMAKROS_H_

// Port der Digitalen IO Karte (s. Datenblatt)

#define D_IOBASE 0x300 								// Anfangsadresse Ports
#define DIGITAL_CARD_CONTROL (D_IOBASE + 0x03) 		// Adresse von Card Control
#define DIGITAL_CARD_CROUP0_PORTA (D_IOBASE + 0x00) // Adresse von Port A


/**
 * PORT A
 */
#define GREEN() out8(D_IOBASE, 0x20)
#define YELLOW() out8(D_IOBASE, 0x40)
#define RED() out8(D_IOBASE, 0x80)

#endif /* IOMAKROS_H_ */
