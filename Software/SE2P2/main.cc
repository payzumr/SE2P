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

#include "Thread.h"

using namespace thread;

int main(int argc, char *argv[]) {

#ifdef SIMULATION
	cout << "Simulation aktiv" << endl;
	cout << "Zum Aufbau der Verbindung muss Die Festo Simulation schon laufen." << endl;
	IOaccess_open(); // Baue die Verbindung zur Simulation auf
#endif


	// Zugriffsrechte fuer den Zugriff auf die HW holen
	if (-1 == ThreadCtl(_NTO_TCTL_IO, 0)) {
		perror("ThreadCtl access failed\n");
		return -1;
	}

	
	//Neues Objekt der Klasse Thread anlegen
	Thread thread;
	//Thread starten (void execute() wird aufgerufen)
	thread.start(NULL);

	sleep(15);

	//Thread beenden (void shutdown() wird aufgerufen)
	thread.stop();
	thread.join();

#ifdef SIMULATION
	IOaccess_close(); // Schließe die Verbindung zur Simulation
#endif
	return EXIT_SUCCESS;
}
