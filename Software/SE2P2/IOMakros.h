/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
                Philipp Kloth (2081738)
                Rutkay Kuepelikilinc (2081831)
                Natalia Duske (2063265)
 */


#include "HWaccess.h"
#ifndef IOMAKROS_H_
#define IOMAKROS_H_

// Port der Digitalen IO Karte (s. Datenblatt)

#define D_IOBASE 0x300 								// Anfangsadresse Ports
#define DIGITAL_CARD_CONTROL (D_IOBASE + 0x03) 		// Adresse von Card Control
#define DIGITAL_CARD_CROUP0_PORTA (D_IOBASE + 0x00) // Adresse von Port A


/**
 * PORT A 				Bit	IO 	0 	1
 * Motor Rechtslauf 	0 	OUT 	Band läuft nach rechts
 * Motor Linkslauf 		1 	OUT 	Band läuft nach link
 * Motor langsam		2 	OUT  	+ Bit 0: Band läuft langsam nach rechts
 *									+ Bit 1: Band läuft langsam nach links
 * Motor Stop			3	OUT 	Band läuft nicht egal wie 1/2 stehen
 * Weiche Auf			4	OUT 	Weiche geht auf
 * Ampel gruen			5 	OUT 	Grüne Lampe geht an
 * Ampel gelb			6	OUT 	Gelbe Lampe geht an
 * Ampel rot 			7 	OUT 	Rote Lampe geht an
 */

#define GREEN() out8(D_IOBASE, 0x20) //Bit 5
#define YELLOW() out8(D_IOBASE, 0x40) // Bit 6
#define RED() out8(D_IOBASE, 0x80)  // Bit 7

#endif /* IOMAKROS_H_ */
