//    FourButtons library. Configured in the Controller class and 
//    used for capturing user input from four dedicated buttons. 
//
//    The library provides an  example on how to configure callbacks, including routing callbacks to a class member
//    via a static trampoline function. It contains two callbacks - one is where one button is pressed and 
//    another one when two buttons are pressed simultaneously. 
/*
  =================================================================
  (C)2022 IFH
  This file is part of Arduino/STM32 MVC prototype.
  Arduino/STM32 MVC prototype is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  Arduino MVC prototype is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.
*/

#ifndef _FourButtons_H_
#define _FourButtons_H_
// these are milliseconds
#define FOURBTN_DEFAULT_PRESSDUR_MS 100  //min duration to recognise a pressed button 
#define FOURBTN_DEBOUNCE_TIMEOUT_MS 25   //time to ignore fluctuations of signal from a button (debounce) 
#define FOURBTN_TRIGGER_LEVEL HIGH       //a logical level that a pressed button generates  (HIGH or LOW)

// statuses of a button 
//OFF->PENDING->ON->PRESSED->OFF
#define FOURBTN_OFF 1     //off
#define FOURBTN_PENDING 2 //just pressed 
#define FOURBTN_ON 3      //pressed with more than debounce timeout
#define FOURBTN_PRESSED 4 //pressed with more than specified pressed duration

#include "Arduino.h"

class FourButtons {
	public:
		// any pin number less than 3 will be discarded
		FourButtons(int p1, int p2, int p3, int p4, unsigned long pressDur, unsigned int lvl);
		FourButtons(int p1, int p2, int p3, int p4);

		// sets pin mode & pull-up resistors
		// call this in setup
		void setup();

//There are two setCallback methods per each case (one button pressed and two buttons pressed) available in this library. 
//The first setCallback method is for registering a C function callback 
//and the other is for registering a classes pseudo-callback for an object.
//The actual call function will use the callback that isn't NULL.
// Described in details here http://www.cplusplus.com/forum/general/136410/
		//single button pressed callback 
		void setPressCallback(void (*cb)(int));
	    void setPressCallback(void (*cb)(void*,int),void* obj);
		
		//two buttons pressed callback
		void setTwoPressCallback(void (*cb)(int,int));
	    void setTwoPressCallback(void (*cb)(void*,int,int),void* obj);
		
		
		//checks if a button is pressed and returns an appropriate callback
		//call this in your sketch loop 
		void update();
		
		//checks if a button is pressed and returns yes (1) or no (0) 
		int isBtnPressed(int p);
		
		
	private:
	    //single button pressed callback 
		void (*callback)(int);
	    void (*callback_obj)(void*,int);
		void* object;
		
		//two buttons pressed callback
		void (*callback2)(int,int);
	    void (*callback2_obj)(void*,int,int);		
		void* object2;
	    
	
		
	protected:
		unsigned long pressDuration;
		unsigned int triggerLevel;
		
		
		int pinNum[4];
		int state[4];
		unsigned long pressTime[4]; //in milliseconds 
		
		int8_t isOtherButtonPressed(int skip); //Determine if any other button has been pressed 
};

#endif //_FourButtons_H_
