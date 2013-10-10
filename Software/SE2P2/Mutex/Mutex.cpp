/*
 * Mutex.cpp
 *
 *  Created on: 10.10.2013
 *      Author: Jannik
 */

#include "Mutex.h"
#include "HWaccess.h"
#include "Addresses.h"
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex;

Mutex::Mutex() {
	if(pthread_mutex_init(&mutex, NULL) != 0){
		perror("Mutex init failed!\n");
		exit(EXIT_FAILURE);
	}
}

Mutex::~Mutex() {
	if(pthread_mutex_destroy(&mutex) != 0){
		perror("Mutex destroy failed!\n");
		exit(EXIT_FAILURE);
	}
}

void Mutex::lock(){
	if(pthread_mutex_lock(&mutex) != 0){
		perror("Mutex lock failed!\n");
		exit(EXIT_FAILURE);
	}

}
void Mutex::unlock(){
	if(pthread_mutex_unlock(&mutex) != 0){
		perror("Mutex unlock failed!\n");
		exit(EXIT_FAILURE);
	}

}
