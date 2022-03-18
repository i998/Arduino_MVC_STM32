/*
 v1.0 - Arduino/STM32 MVC prototype

 Status: Works OK 

  Change list:
  v1.0: Initial version 


This  is a working prototype of a sort-of MVC pattern for Arduino / STM32. 
Can be used to operate in a common scenario like:  
  - Get user input 
  - Collect data from other sources like sensors  
  - Process the information and display the results based on the data captured and/or user input provided

Consists of the following software components: 
  - Arduino sketch. Used for creating class objects and running  loops  as usual.  
    Also has a STM32 timer configured here as some of libraries to handle user input device(like Encoder) 
	require their timer interrupt service routine ClickEncoder:service() to be called every millisecond. 
    (Arduino users may use a software library like TimerOne for that purpose)
    Also has a configuration for an LCD display library and a Display object (Display1). 

  - HW_Pins.h file contains hardware configuration (which input or output devices connected to which pins). 
    This is an attempt to get them configured in one place so it is easier to manage them later. 

  - Class DAO (Data Access Object). Stores relevant data (for example historic values form sensors) for the solution. 

  - Class Presentation. Creates visual representation of data and show it to users. 
    The Presentation class instance has access to DAO via a pointer. It also has access to a Display object
   (Display1) via a pointer (LCD1_A) in order to allow output to the LCD display.  

  - Class Controller.  Performs all the necessary interactions between classes and data manipulations. 
    The Controller class instance has access to DAO and Presentation objects via pointers.

  - FourButtons library. Configured in the Controller class and used for capturing user input from four dedicated buttons. 
    The library provides an  example on how to configure callbacks, including routing callbacks to a class member
    via a static trampoline function.

  - SSD1306_i2c library for STM32, for use with STM32 hardware I2C ports. 
    It uses a pointer to the I2C object that STM32 has. The library can be used for a generic Arduino too with 
    minimal changes and a pointer to the Arduino's Wire object. 

  - Adafruit_GFX Library for working with LCD display(s).  
  
  - Arduino for STM32 (https://github.com/rogerclarkmelbourne/Arduino_STM32)
  
  
Built of the following hardware components:  
  - Maple Mini STM32 (https://stm32duinoforum.com/forum/wiki_subdomain/index_title_Maple_Mini.html)
  - SSD1306 I2C 128x32 OLED display  
  - 10K variable resistor 
  - a couple of buttons 
  See wiring diagram in the /documentation folder.  
  
Note for Arduino users:  
  This prototype shall work with Arduino with the following changes:
  - Use pointer to built-in Wire object instead of setting up hardware I2C interface and TwoWire object 
  - Use a Timer library (like TimerOne) instead of setting up STM32 hardware timer 
  - Update HW_Pins.h file with relevant pins   

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

#include "src/DAO.h"
#include "src/Presentation.h"
#include "src/Controller.h"
#include "HW_Pins.h"
#include "src/SSD1306_i2c.h"
#include <Adafruit_GFX.h>

//Create STM32 Timer 
	#define TIMER_RATE 500000    // in microseconds; should give 0.5Hz toggles

	// We'll use timer 2
	HardwareTimer timer(2);


//Setup I2C interface
// hardware interface:
	//TwoWire I2C_FAST=TwoWire(1,I2C_FAST_MODE); //I2C #1  
	TwoWire I2C_FAST=TwoWire(2,I2C_FAST_MODE); //I2C #2

//For software interface for STM32 use  #include <Wire_slave.h> 

	
//Set Up Display(s) 
//Setup the SSD1306_i2c with a pointer to the I2C interface 
	SSD1306_i2c Display1 = SSD1306_i2c(&I2C_FAST,LCD_RESET);

	
	
	
//This is for a quick way to get Presentation output to serial
	unsigned long previousMillis = 0;

  
//Create instances of the classes 
	//DAO (Data Access Object)
	DAO Data = DAO();

	//Presentation with links to Data and Display objects
	Presentation Pres = Presentation(&Data, &Display1);


	//Controller with links to Data and Presentation objects
	Controller Cntrl = Controller(&Data,&Pres);


void setup() {
	
  //==============================
   Serial.begin(9600);
   //Serial.begin(115200);
   //The program will wait for serial to be ready up to 10 sec then it will continue anyway  
     for (int i=1; i<=10; i++){
          delay(1000);
     if (Serial){
         break;
       }
     }
    Serial.println("Setup() started ");
  //===============================

//hardware set up 
	#ifdef _BOARD_MAPLE_MINI_H_
	 //maple mini
	  pinMode(P1, INPUT); //floating
	  pinMode(P2, INPUT_PULLDOWN);
	  pinMode(P3, INPUT_PULLDOWN);
	  pinMode(analogPin, INPUT_ANALOG); // set up pin for analog input
	 #else
	  pinMode(P1, INPUT);
	  pinMode(P2, INPUT);
	  pinMode(P3, INPUT);
	 #endif
	  
	  pinMode(LEDPIN, OUTPUT);
 

 
//Setup STM32 Timer   
    // Pause the timer while we're configuring it
    timer.pause();

    // Set up period
    timer.setPeriod(TIMER_RATE); // in microseconds

    // Set up an interrupt on channel 1
    timer.setChannel1Mode(TIMER_OUTPUT_COMPARE);
    timer.setCompare(TIMER_CH1, 1);  // Interrupt 1 count after each update
	// Attach the timer interrupt handler 
    timer.attachCompare1Interrupt(TimerISR);

    // Refresh the timer's count, prescale, and overflow
    timer.refresh();

    // Start the timer counting
    timer.resume();
  
 
//Initialize the SSD1306 library
	//SSD1306_SWITCHCAPVCC: by default, we'll generate the high voltage from the 3.3v line internally
	//0x3C: initialize with the I2C addr 0x3C for the 128x32  (0x3D for the 128x64)
	//false: we do not use hardware reset
	Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  
  
	
	//Uncomment to just check LCD works - shall show Adafruit logo on start	
	//Display1.display();
    //delay(1000);
	
	
  
	// Clear the buffer.
	Display1.clearDisplay();

   
  

	//Each class has its setup function. We call them here as a part of the sketch's setup(). 
	Data.setup();
	Cntrl.setup();
	Pres.setup();

 
	//Example of setting some default values in DAO directly if needed
	Data.v=22;
  
}

//Set STM32 Timer interrupt service routine function
void TimerISR(void) {
	//Call Controller class function 
    Cntrl.clk_In();
}



void loop() {
	
//call Controller , Presentation and DAO  for update as a part of the loop
	 Cntrl.update();  // for data acqusition and screenflow logic
	 Pres.update();   // for updates on dynamic screens 
	 //Data.update();  // not currently used 


// use Presentation class to produce and  output to serial on a regular basis. 
// that is a simulation for output to user function. 
// can be done as a part of the loop here or called by a timer instead. 
// this is a quick way to get Presentation output to serial every 10 sec
	 unsigned long interval = 10000;    
	 unsigned long currentMillis = millis();
	  if (currentMillis - previousMillis >= interval) {
		   previousMillis = currentMillis;
			Pres.displaySerial();
	  }

  
}
