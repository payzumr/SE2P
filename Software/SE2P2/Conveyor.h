/*
 * Conveyor.h
 *
 *  Created on: 02.10.2013
 *      Author: Jannik Schick (2063265)
 *              Philipp Kloth (2081738)
 *              Rutkay Kuepelikilinc (2081831)
 *              Natalia Duske (2063265)
 */

#ifndef CONVEYOR_H_
#define CONVEYOR_H_

/*
 *
 */
class Conveyor {

private:
    static Conveyor* instance;

    Conveyor();
    ~Conveyor();

public:
    static Conveyor* getInstance();

    void move_left();
    void move_rigth();
    void move_slowON();
    void move_slowOFF();
    void stop();
};

#endif /* CONVEYOR_H_ */
