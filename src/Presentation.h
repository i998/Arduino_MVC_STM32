//   Class Presentation.  Creates visual representation of data and show it to users. 
//   The Presentation class instance has access to DAO via a pointer (DAO_A). 
//   It also has access a Display object via a pointer (LCD1_A) in order to allow output to a lcd display.
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
#ifndef _Presentation_H_
#define _Presentation_H_

#include "DAO.h"
#include "SSD1306_i2c.h"

class Presentation { 
	  
	  
	public:
		Presentation(void);
		Presentation(DAO *DAO_A);
		Presentation(DAO *DAO_A, SSD1306_i2c *LCD1_A);

		~Presentation(void);								   

		//Anything to do as a part of setup  
		void setup(void);  

		//Activated from Sketch loop and used for dynamic screens or other dynamic effects 
		void update(void); 

		//Activated from Controller and used for displaying one of the screens 
		void PresentScreen(uint8_t screenNo); 

		//Toggle a led on the specified pin  as an example of an output to user.
		void ToggleLed(int Pin); 

		//Display DAO info to the serial monitor as output 
		void displaySerial(void); 

		//Indicate which screen is currently shown 
		uint8_t currentScreenNo = 0;



	private:
		//Pointer to DAO object 
		DAO *_DAO;
		//Pointer to Display object 
		SSD1306_i2c *_LCD1;

		//A time when the update function is completed in milliseconds
		uint32_t timestampUpdateCompleted = 0;	


		//Screens 
		  //static screens
		void screen1(); 
		void screen2(); 
	 
	 
		  //dynamic screens  
		  //each dynamic screen has its variable previousMillisX to calculate intervals for a "blink" or "delay" or for similar functionality
		void screen3(); 
		uint32_t previousMillis3 = 0; 

		void screen4(); 
		uint32_t previousMillis4 = 0;
		//sometimes a dynamic screen needs more variables to keep the state of operations
				 #define NUMFLAKES 10  //number of simultaneous bitmap objects
				 #define XPOS 0        //array index for X position 
				 #define YPOS 1        //array index for Y position 
				 #define DELTAY 2      //array index for a Shift by Y position  

				  //Array to store data for each of bitmap objects
				  uint8_t icons[NUMFLAKES][3];

				  

};
#endif // _Presentation_H_