//   Class Controller.  Performs all the nessesary interactions between classes and data manipulations. 
//   The Controller class instance has access to DAO and Presentation via pointers (DAO_A, Presentation_A)
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
#ifndef _Controller_H_
#define _Controller_H_

#include "Arduino.h"
#include "FourButtons.h"
#include "../HW_Pins.h"
#include "DAO.h"
#include "Presentation.h"


class Controller { 
  
  
	public:
		Controller(void);
		Controller(DAO *DAO_A, Presentation *Presentation_A);
		~Controller(void);
		
		//Anything to do as a part of setup  
		void setup(void);  
		
		//Activated from Sketch loop and used for handling data acquisition logic, screen flow etc
		void update(void); 

		//Input from the timer 
		void clk_In(void); 
			
		//Single button pressed callback 
		// This is a is the pseudo-callback method  - a static trampoline function to a class method 
		//described here  http://www.cplusplus.com/forum/general/136410/	
		static void CntrlCallBackTrampoline(void* object,int value);	
			
		//a class method - real callback action is here 	
		void btnPressed(int p1); 
			
			
		//Two buttons pressed callback	
		// This is a is the pseudo-callback method  - a static trampoline function to a class method 
		//described here  http://www.cplusplus.com/forum/general/136410/	
		static void CntrlCallBack2Trampoline(void* object,int value1,int value2 );	
			
		//a class method - real callback action is here 	
		void twoBtnPressed(int p1, int p2);	
		
		
		
		
		
	private:
		//pointers to DAO and  Presentation objects 
		DAO *_DAO;
		Presentation *_Presentation;

		//button object  - from FourButtons library that handles user input 
		FourButtons btn = FourButtons(P1, P2, P3, P4);

		//just a class variable 	
		uint8_t _isbtnpressed=0;		


			
			
		//--------other functions------
		int getVoltage(int Pin);	
		int getVoltage(void);	
	
};
#endif // _Controller_H_