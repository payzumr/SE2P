/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 Philipp Kloth (2081738)
 Rutkay Kuepelikilinc (2081831)
 Natalia Duske (2063265)
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h> //fuer sleep() und usleep
#include "HWaccess.h"
#include "ioaccess.h"
#include "HALSensorik.h"
#include <string>
#include "MachineState.h"
#include "Initialisation.h"
#include "Timer.h"
#include "Serial.h"
#include "Blinki.h"

#include "Thread.h"
#define TEST_TIME 25

using namespace thread;
using namespace hal;
using namespace std;

int main(int argc, char *argv[]) {
#ifdef SIMULATION
	cout << "Simulation aktiv" << endl;
	cout << "Zum Aufbau der Verbindung muss Die Festo Simulation schon laufen."
			<< endl;
	IOaccess_open(); // Baue die Verbindung zur Simulation auf
#endif
	// Initialisierung der Digitalen IO Karte
	out8(DIO_BASE + DIO_OFFS_CTRL, 0x8A);
	out8(DIO_BASE + DIO_OFFS_A, 0x00);
	out8(DIO_BASE + DIO_OFFS_C, 0x00);

	// Zugriffsrechte fuer den Zugriff auf die HW holen
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return -1;
	}

	/*
	 //Neues Objekt der Klasse Thread anlegen
	 Thread thread;
	 //Thread starten (void execute() wird aufgerufen)
	 thread.start(NULL);

	 string quit;
	 do {
	 cin >> quit;
	 } while (quit != "q");
	 thread.stop();
	 thread.join();
	 */

	/*
	 * Sensorik und Initalisation starten für den Initialisierungslauf
	 */
	Blinki* blink = new Blinki();
	blink->start((void*)GREEN);
	HALSensorik::getInstance()->start(NULL);
	Timer::getInstance()->start(NULL);
	Initialisation::getInstance()->start(NULL);
	//Warten auf den Abschluss der Initialisierung
	while (MachineState::getInstance()->dispatcherGo) {
		sleep(1);
	}
	//Threads für die Initalisierung beenden
	Initialisation::getInstance()->stop();
	blink->stop();
	blink->join();
	Initialisation::getInstance()->join();

	//Dispatcher und somit das eigentliche Programm starten
	Dispatcher::getInstance()->start(NULL);
	Serial::getInstance()->start(NULL);
	string quit;
	do {
		cin >> quit;
	} while (quit != "q");

	//Threads beenden (void shutdown() wird aufgerufen)
	HALSensorik::getInstance()->stop();
	Serial::getInstance()->stop();
	Timer::getInstance()->stop();

	//Threads joinen (auf Beendigung warten)
	HALSensorik::getInstance()->join();
	Serial::getInstance()->join();
	Timer::getInstance()->join();

#ifdef SIMULATION
	IOaccess_close(); // Schliesse die Verbindung zur Simulation
#endif
	return EXIT_SUCCESS;
}
