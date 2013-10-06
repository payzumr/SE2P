/*
 * Thread.cpp
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick
                Philipp Kloth
                Rutkay Küpelikilinc
                Natalia Duske
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include "IOMakros.h"
#include "HWaccess.h"

#include "Thread.h"

using namespace thread;

int main(int argc, char *argv[]) {

#ifdef SIMULATION
	cout << "Simulation aktiv" << endl;
	cout << "Zum Aufbau der Verbindung muss Die Festo Simulation schon laufen." << endl;
	IOaccess_open(); // Baue die Verbindung zur Simulation auf
#endif

	// Simpler Test des HW Zugriffs ohne Klassen, Pattern etc.
	// Reicht nicht fuer den ersten Meilenstein aus

	// Zugriffsrechte fuer den Zugriff auf die HW holen
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return -1;
	}
	// Initialisierung der Digitalen IO Karte
	out8(DIGITAL_CARD_CONTROL, 0x8A);

	Thread thread;
	thread.start(NULL);

	sleep(10);

	thread.stop();
	thread.join();

#ifdef SIMULATION
	IOaccess_close(); // Schließe die Verbindung zur Simulation
#endif
	return EXIT_SUCCESS;
}
