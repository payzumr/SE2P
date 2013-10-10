/*
 * Conveyor.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#include "Conveyor.h"
#include "HWaccess.h"
#include "Addresses.h"

#include <stdint.h>

Conveyor* Conveyor::instance = NULL;

Conveyor::Conveyor() {
    // Initialisierung der Digitalen IO Karte
    out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);
    out8(DIO_BASE + DIO_OFFS_A, 0x00);
    out8(DIO_BASE + DIO_OFFS_C, 0x00);
}
Conveyor::~Conveyor() {
	delete instance;
	instance = NULL;

}

Conveyor* Conveyor::getInstance(){

	// Zugriffsrechte fuer den Zugriff auf die HW holen
    if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
        perror("ThreadCtl access failed\n");
        return NULL;
    }
    if(instance == NULL)
    {
    	instance = new Conveyor();
    }
    return instance;


}
void Conveyor::move_rigth(){
	    uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_1);
	    val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val | BIT_0);
}
void Conveyor::move_left(){
	    uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_0);
	    val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val | BIT_1);
}
void Conveyor::move_slowON(){
	    uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val | BIT_2);
}
void Conveyor::move_slowOFF(){
	    uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val & ~BIT_2);
}
void Conveyor::stop(){
	    uint8_t val = in8(DIO_BASE + DIO_OFFS_A);
	    out8(DIO_BASE + DIO_OFFS_A, val | BIT_3);
}

