/*
 * Log.cpp
 *
 *  Created on: 14.12.2013
 *      Author: Rutkay
 */
#include <string>
#include <stdio.h>
#include "Log.h"

#define ARRSIZE 100

Mutex* Log::logMutex = new Mutex();
bool LogRunning = false;
bool show = false;
int writepointer = 0;

string msgs[ARRSIZE];


Log::Log() {
	for(int i = 0; i < ARRSIZE; i++){
		msgs[i] = "0";
	}
}

Log::~Log() {
	// TODO Auto-generated destructor stub
}


void Log::LogMe(string string){
	logMutex->lock();
	msgs[writepointer] = string;
	writepointer = (writepointer+1)%ARRSIZE;
	logMutex->unlock();


}

void Log::ShowLog(){

	while(show){
		for(int i = writepointer+1; i < ARRSIZE + writepointer+1; i++){
			string tmpmsg = msgs[i%ARRSIZE];
			if(tmpmsg.compare("0") != 0){
			cout<<tmpmsg<<endl;
			}
		}
	}
}

void Log::StopLog(){
	show = false;
}

void Log::execute(void* arg){

	while (1) {
			char charIn = getchar();
			if(charIn == 'l' && LogRunning == false){
				LogRunning = true;
				printf("Log ausgabe");
				ShowLog();

			}else if(charIn == 'l' && LogRunning == true){
				StopLog();
				LogRunning = false;
			}


	}

}

void Log::shutdown(){

}






