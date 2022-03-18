//    Implementation of FourButtons library. Configured in the Controller class and 
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
#include "FourButtons.h"

FourButtons::FourButtons(int p1, int p2, int p3, int p4, unsigned long pressDur, unsigned int lvl) {
	this->pinNum[0]=p1;
	this->pinNum[1]=p2;
	this->pinNum[2]=p3;
	this->pinNum[3]=p4;
	this->pressDuration = pressDur>0? pressDur: FOURBTN_DEFAULT_PRESSDUR_MS;
	this->triggerLevel = lvl!=0? lvl: FOURBTN_TRIGGER_LEVEL;
	
    this->callback = NULL;
	this->callback_obj = NULL;
	this->object = NULL;
	
	this->callback2 = NULL;
	this->callback2_obj = NULL;
	this->object2 = NULL;
	
	
}

FourButtons::FourButtons(int p1, int p2, int p3, int p4) {
	this->pinNum[0]=p1;
	this->pinNum[1]=p2;
	this->pinNum[2]=p3;
	this->pinNum[3]=p4;
	this->pressDuration = FOURBTN_DEFAULT_PRESSDUR_MS;
	this->triggerLevel =  FOURBTN_TRIGGER_LEVEL;
	
	
	this->callback = NULL;
	this->callback_obj = NULL;
	this->object = NULL;
	
	this->callback2 = NULL;
	this->callback2_obj = NULL;
	this->object2 = NULL;
}

//this is set up 
void FourButtons::setup() {

	for(int i=0; i<4; i++) {
		this->state[i]=FOURBTN_OFF;
		this->pressTime[i]=0;
	

   //Hardware pin setup 
		if (triggerLevel == HIGH)
			{
			#ifdef _BOARD_MAPLE_MINI_H_
			 //maple mini
				for(int i=0; i<4; i++) {
			
					// skip disconnected pins ( pin no is -1)
					if(pinNum[i]==-1)
						continue;  
					
					pinMode(pinNum[i], INPUT_PULLDOWN);
					
				}

			 #else
			 //arduino
				for(int i=0; i<4; i++) {	
					
					// skip disconnected pins ( pin no is -1)
					if(pinNum[i]==-1)
						continue;  
					
					
					// ignore first two Arduino pins
					//if(pinNum[i]<3)
					//continue;  
				
					pinMode(pinNum[i], INPUT);
					//will enable (HIGH) or disable (LOW) the internal pullup on the input pin
					digitalWrite(pinNum[i], LOW);
				} 

			 #endif
			}
		 else	
			{
			#ifdef _BOARD_MAPLE_MINI_H_
			 //maple mini
				for(int i=0; i<4; i++) {
			
					// skip disconnected pins ( pin no is -1)
					if(pinNum[i]==-1)
						continue;  
					
					pinMode(pinNum[i], INPUT_PULLUP);
					
				}
			 
			 
			 #else  
			 // arduino
					for(int i=0; i<4; i++) {	
					
					// skip disconnected pins ( pin no is -1)
					if(pinNum[i]==-1)
						continue;  
					
					
					// ignore first two Arduino pins
					//if(pinNum[i]<3)
					//continue;  
				
					pinMode(pinNum[i], INPUT);
					//will enable (HIGH) or disable (LOW) the internal pullup on the input pin
					digitalWrite(pinNum[i], HIGH);
				} 
			 #endif
			 }
 	
		
		
		
	}
	//Serial.println("FourButtons:setup completed");
}


// this function will organise a callback to a calling application if a button is pressed 
void FourButtons::update() {

	unsigned long now = millis();

	int8_t justPressed=-1;  // a highest number of a button that  has been just pressed 

//Check each button's status
	for(int i=0; i<4; i++) {
		
	    // ignore first two Arduino pins
		//	if(pinNum[i]<3)
	    //		continue;  

		
		// skip disconnected pins ( pin no is -1)
		if(pinNum[i]==-1)
			continue;  
		

		//Check button's status and update its status 
		if(digitalRead(pinNum[i]) == triggerLevel)
		{ // pressed   
			switch(this->state[i]) {
				case FOURBTN_OFF:  //OFF->PENDING 
					this->state[i] = FOURBTN_PENDING;
					this->pressTime[i] = now;  //the button could be pressed from now
					break;
				case FOURBTN_ON: //ON -> PRESSED  if button is still pressed and more than pressDuration time passed 
					if((now-pressTime[i])>this->pressDuration) {
						this->state[i] = FOURBTN_PRESSED;
						justPressed=i;  //indicates which button number is just pressed 
					}
					break;
				case FOURBTN_PENDING:  //PENDING -> ON  if button is still pressed and if more than debounce time
					if((now-pressTime[i])>FOURBTN_DEBOUNCE_TIMEOUT_MS) {
						this->state[i] = FOURBTN_ON;
					}
					break;
				case FOURBTN_PRESSED:  // already pressed 
					break;
				default:
					break;
			}
		}
		else 
		{//not pressed 
			switch(this->state[i]) {
				case FOURBTN_OFF:
					this->pressTime[i] = 0;
					break;
				case FOURBTN_PRESSED:  // do the same as FOURBTN_ON
				case FOURBTN_ON:
					this->pressTime[i] = 0;
					this->state[i]=FOURBTN_OFF;
					break;
				case FOURBTN_PENDING: //PENDING -> OFF if button is no longer pressed after more than debounce time
					if((now-pressTime[i])>FOURBTN_DEBOUNCE_TIMEOUT_MS) {
						this->pressTime[i] = 0;
						this->state[i]=FOURBTN_OFF;
					}
					break;
				default:
					break;
			}
		}
		
		
	}

	
	
//If a pressed button is detected then check others and perform an appropriate callback	
	if(justPressed>=0) { //any button that is just pressed, justPressed variable would have the highest number if two buttons are pressed absolutely at the same time
		int onBtn = isOtherButtonPressed(justPressed);
		if(onBtn>=0) { //found another button pressed, return 2 numbers 
			if(state[onBtn] == FOURBTN_PRESSED) {
				
				//Perform a callback for two buttons pressed 		
			
				//The call function will use the callback that isn't NULL.
				//http://www.cplusplus.com/forum/general/136410/
				if(callback2 != NULL)
				{
				  //Serial.println("FourButtons:about to call the callback2");
				  callback2(this->pinNum[justPressed], this->pinNum[onBtn]);
				 }
				else if((callback2_obj != NULL) && (object2 != NULL))
				{
				//Serial.println("FourButtons:about to call the callback2 with object specified");
				callback2_obj(object2,this->pinNum[justPressed], this->pinNum[onBtn]);
				}
				
			}
		}
		else //isOtherButtonPressed does not return any other number, so a single button has been pressed. 
		{     
			//Perform a callback for a single button pressed 		
			
			//The call function will use the callback that isn't NULL.
			//http://www.cplusplus.com/forum/general/136410/
			if(callback != NULL)
	        {
		      //Serial.println("FourButtons:about to call the callback");
			  callback(this->pinNum[justPressed]);
	         }
	        else if((callback_obj != NULL) && (object != NULL))
	        {
		    //Serial.println("FourButtons:about to call the callback with object specified");
			callback_obj(object,this->pinNum[justPressed]);
	        }
			
	    }
	}
}



// Check button statuses and determine if any other button has been pressed and return its number  
int8_t FourButtons::isOtherButtonPressed(int skip) {
   

	for(int i=0; i<4; i++) {
		if(i == skip)  //we already know this one is pressed 
			continue;
		if(this->state[i] == FOURBTN_ON || this->state[i] == FOURBTN_PRESSED)
			return i;  // this is when another button is pressed 
	}
	return -1;//isOtherButtonPressed does not return any other number
}


//Set callback for a single button pressed

//There are two setCallback methods for this. The first one is for registering a C function callback 
//and the other is for registering a classes pseudo-callback for an object. 
//The call function will use the callback that isn't NULL.
//http://www.cplusplus.com/forum/general/136410/
		
         void FourButtons::setPressCallback(void (*cb)(int))
{
		this->callback_obj = NULL;
		this->object = NULL;
		this->callback = cb;

}



		void FourButtons::setPressCallback(void (*cb)(void*,int),void* obj)
{
		this->callback = NULL;
		this->object = obj;
		this->callback_obj = cb;
}



//Set callback for two buttons pressed

//There are two setCallback methods for this. The first one is for registering a C function callback 
//and the other is for registering a classes pseudo-callback for an object. 
//The call function will use the callback that isn't NULL.
//http://www.cplusplus.com/forum/general/136410/


         void FourButtons::setTwoPressCallback(void (*cb)(int,int))
{
		this->callback2_obj = NULL;
		this->object2 = NULL;
		this->callback2 = cb;

}



		void FourButtons::setTwoPressCallback(void (*cb)(void*,int,int),void* obj)
{
		this->callback2 = NULL;
		this->object2 = obj;
		this->callback2_obj = cb;
}



//Checks if a button is pressed and returns yes or no 
int  FourButtons::isBtnPressed(int pinNumber)
{
	//get the button index 
	for(int i=0; i<4; i++) {
	   if(pinNum[i]==pinNumber && pinNumber>=0) 
	   {
		   //get button state and check if it is pressed
			if(this->state[i] == FOURBTN_ON || this->state[i] == FOURBTN_PRESSED)
			{		
				return 1; 
			}	
			
	   }
	}
	
	return 0;
}