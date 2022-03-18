//   Implementation of the class Controller. Performs all the necessary interactions between classes and data manipulations. 
//   The Controller class instance has access to DAO and Presentation via a pointers (DAO_A, Presentation_A)
//   Includes FourButtons library that manages four buttons for user input.
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
#include "Controller.h"

Controller::Controller(void)
{

	
}

Controller::Controller(DAO *DAO_A, Presentation *Presentation_A)
{

  _DAO = DAO_A;
  _Presentation = Presentation_A;

}



Controller::~Controller(void)
{}							   

void Controller::setup(void)
{

//set up FourButtons library 
//and point its callbacks to static functions as required (cannot point directly to a class function)    
	btn.setup();
	//set the single button pressed callback 
	btn.setPressCallback(Controller::CntrlCallBackTrampoline,this);
	//set the two buttons pressed callback 
	btn.setTwoPressCallback(Controller::CntrlCallBack2Trampoline,this);

} 	

//This is the function that contains data acquisition logic and screen flow
//called from the sketch's loop()
void Controller::update(void)  
{
	
// Here is an example of a logic:
//  a) Show a message in the display while a dedicated button is pressed
//  b) Get a "voltage" figure, "debounce" it and then show it in the display
//  c) If "voltage" figure is below the limit and it is more than 10 sec since any button is pressed, show a screensaver 
//  d) If a "voltage" figure is above a limit then show it in the display with a blinking warning, no screensaver
//  e) Meanwhile count how many times a button is pressed     	
//  f) Use other user output other than the LCD display -  use builtin LED and send some data to serial output 	
	
	//check user input  
	 //get info from buttons  	
     btn.update();
     
    //update presentation  
    if (btn.isBtnPressed(P2)==1) 
	{//show screen 2 while P2 is pressed  
		_Presentation->PresentScreen(2);
		//Serial.println("PresentScreen(2) called");
	}
    else
	{
		//get other hardware input
		uint16_t v= this->getVoltage();

		
		//check acquired data and update DAO 
		int8_t voltageTreshhold = 3;
		int16_t diff = v-_DAO->v;
		if (abs(diff) > voltageTreshhold) 
		{
		  //update DAO
		  _DAO->v = this->getVoltage();
		}
		
		
		//update presentation
		int16_t voltageLimit = 2000;  
		if (v <voltageLimit) 
		{ //Normal voltage, normal screen	or screen-saver 
		
	        if ((millis() -_DAO->ts) < 10000)  
			{// less than 10 sec since any button press
              //show screen with voltage (screen1)   			
		      _Presentation->PresentScreen(1);
			  //Serial.println("PresentScreen(1) called");
			}
             else
			{
			 //show dynamic screen saver (screen4)   			
		      _Presentation->PresentScreen(4);
			} 

			
		} 
		else 
		{ // Voltage over limit, screen3 
		  _Presentation->PresentScreen(3);	
		  //Serial.println("PresentScreen(3) called");
		}	
	
	}
}



//Input from the timer 
void Controller::clk_In(void)
{}


//Single button pressed callback 
// This is a is the pseudo-callback method  - a static trampoline function to a class method 
//described here  http://www.cplusplus.com/forum/general/136410/		
void Controller::CntrlCallBackTrampoline(void* object,int value)
{
        
	//Serial.println("CNTRL:CntrlCallBackTrampoline called");
	
	Controller* This = (Controller*) object;
	// call the object's btnPressed Method with the arguments passed
	This->btnPressed(value);
	
}	



//a class method - real callback action is here 
void Controller::btnPressed(int p1) {
	Serial.print("CNTRL:btnPressed:");
	Serial.println(p1);
  
	//cannot do anything like below form a static function  
	//but can do that here as a part of a class function:
	_isbtnpressed=1; //have access to a class variable
	_DAO->cnt=_DAO->cnt + 1;  //update DAO
	_DAO->ts=millis();  //update DAO
	_Presentation->ToggleLed(LEDPIN);  //update Presentation
  
		Serial.print("CNTRL:DAOCount:");
		Serial.println(_DAO->cnt);
	
	
}




//Two buttons pressed callback 
// This is a is the pseudo-callback method  - a static trampoline function to a class method 
//described here  http://www.cplusplus.com/forum/general/136410/		
void Controller::CntrlCallBack2Trampoline(void* object,int value1, int value2)
{
        
	//Serial.println("CNTRL:CntrlCallBackTrampoline called");
		
	Controller* This = (Controller*) object;
	// call the object's btnPressed Method with the arguments passed
	This->twoBtnPressed(value1, value2);
		
	
}	


//a class method - real callback action is here 
void Controller::twoBtnPressed(int p1, int p2) {
	
	Serial.print("CNTRL:twoBtnPressed:");
	Serial.print(p1);
	Serial.print(" ");Serial.println(p2);
  
	//cannot do anything like below form a static function  
	//but can do that here as a part of a class function:
	_isbtnpressed=1; //have access to a class variable
	_DAO->cnt2=_DAO->cnt2 + 1;  //update DAO
	_DAO->ts=millis();  //update DAO
	_Presentation->ToggleLed(LEDPIN);  //update Presentation
  
		Serial.print("CNTRL:DAOCount2:");
		Serial.println(_DAO->cnt2);
		
	
}




//--------Other Controller functions------
int Controller::getVoltage(int Pin)
{
  return analogRead(Pin); 	
}

int Controller::getVoltage(void) 
{
  return analogRead(analogPin); 	
}