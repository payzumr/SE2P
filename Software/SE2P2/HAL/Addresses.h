/*
 * Adresses.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef ADDRESSES_H_
#define ADDRESSES_H_


/**
 * Base Adressen and Offsets for the Ports
 */
#define DIO_BASE 0x300          // Base address
#define DIO_OFFS_A 0x00         // Port A offset
#define DIO_OFFS_B 0x01         // Port B offset
#define DIO_OFFS_C 0x02         // Port C offset
#define DIO_OFFS_CTRL 0x03      // Control register offset

/**
 * Code to start the HeightMessure
 */
#define CTRL_CODE 0x10

// BIT_X is set to one
#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80

#define RESET_AKT 0x0

/**
 * Defines for Dispatcher
 */
#define SENSORS 2
#define BUTTONS 8
#define RESET 0x40
#define E_STOP 0x80
#define STOP 0x20
#define START 0x10

#define ESTOPBUTTON 0x20

#define ON true
#define OFF false


/**
 * Defines to able or disable Debug Messages
 */
//#define DEBUG_MESSAGE
//#define DEBUG_TIMER


//Serial
#define WRITE 1
#define READ 2

/*
 * interrupt specific
 */

#define OFFS_INT_STATUS 0x0F
#define PB_STATUS       0x02
#define PC_STATUS       0x08

#define OFFS_INT_CTRL   0x0B
#define PB_CTRL         0x02
#define PC_CTRL         0x04

//AIO Card
#define AIO_BASE 	0x320
#define AIO_OFFS_A 	0x02
#define AIO_GET_VAL 0x10
#define AIO_PORT_A 	(AIO_BASE + AIO_OFFS_A)

//LightsControl

#define STOPLIGHTS 1
#define REDFAST 2
#define REDSLOW 3
#define YELLOW 4
#define GREEN 5

//Timer

#define SLIDE_TIME 1000
#define TURN_TIME 10000
#define SWITCH_OPEN_TIME 1000
#define END_TIMER 1500
#define QUITTIERT_TIME 500;
#define HANDOVER_TIME 3000;

#endif /* ADDRESSES_H_ */
