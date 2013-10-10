/*
 * Switch.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef SWITCH_H_
#define SWITCH_H_

class Switch {
private:
	static Switch* instance;

	Switch();
	~Switch();
public:

	static Switch* getInstance();

	void switchOpen();
	void switchClose();
};

#endif /* SWITCH_H_ */
