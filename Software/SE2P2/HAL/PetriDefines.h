/*
 * PetriDefines.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef PETRIDEFINES_H_
#define PETRIDEFINES_H_

#define S0 0
#define S1 1 /* Eingangsbereich */
#define S2 2 /* Anfang Band1 */
#define S3 3 /* in Hoehenmessung */
#define S4 4 /* Ausgang Hoehenmessung mit Bohrung */
#define S5 5 /* Augang Hoehenmessung ohne Bohrung */
#define S6 6 /* Ausgang Hoehenmessung zu flach */
#define S7 7 /* Weiche mit Metall */
#define S8 8 /* Weiche ohne Metall */
#define S9 9 /* Ausgang Weiche */
#define S10 10 /*Ausgang Weiche */
#define S11 11 /* Umdrehplatz */
#define S12 12 /* Auslauf */
#define S13 13 /* warten auf Band 2 */

#define E0 0 /*keine Ereignis*/
#define E1 1 /*Lichtschranke nicht aktiv*/
#define E2 2 /*Lichtschranke aktiv*/
#define E3 3 /*Transport zur Hoehenmessung*/
#define E4 4 /*Werkstueck mit Bohrung*/
#define E5 5 /*Werkstueck ohne Bohrung*/
#define E6 6 /*Werkstueck zu flach*/
#define E7 7 /*Werkstueck ohne Metall*/
#define E8 8 /*Werkstueck mit Metall*/
#define E9 9 /*Werkstueck kein Metall sichtbar*/
#define E10 10 /*Aussortieren in Weiche*/
#define E11 11 /*Bandstop*/
#define E12 12 /*Durchlaufen*/
#define E13 13 /*Umgedreht*/
#define E14 14 /*Timeout*/
#define E15 15 /*Band2besetzt*/
#define E16 16 /*Uebergabe 1*/
#define E17 17 /*Uebergabe 2*/
#define E18 18 /*Ueberlauf*/
#define E19 19 /*Rot ausloesen*/
#define E20 20 /*Gelb ausloesen*/
#define E21 21 /*Gruen ausloesen*/

#define A0 NULL /*keine Aktion*/
#define A1 engine_left
#define A2 engine_rigth
#define A3 engine_slow
#define A4 engine_stop
#define A5 engine_start
#define A6 led_Q1
#define A7 led_Q2
#define A8 led_Start
#define A9 led_Reset
#define A10 switchOnOff

#endif /* PETRIDEFINES_H_ */
