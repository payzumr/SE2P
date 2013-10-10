/*
 * Lights.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef LIGHTS_H_
#define LIGHTS_H_

class Lights {
private:
	static Lights* instance;

	Lights();
	~Lights();
public:

	static Lights* getInstance();

	void redOn();
	void redOff();
	void yellowOn();
	void yellowOff();
	void greenOn();
	void greenOff();
};

#endif /* LIGHTS_H_ */
