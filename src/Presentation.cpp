//   Implementation of Class Presentation.  Creates visual representation of data and show it to users. 
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
#include "Presentation.h"



Presentation::Presentation(void)
{}

Presentation::Presentation(DAO *DAO_A)
{
  _DAO = DAO_A;
}

Presentation::Presentation(DAO *DAO_A, SSD1306_i2c *LCD1_A)
{
  _DAO = DAO_A;
  _LCD1= LCD1_A;
}


Presentation::~Presentation(void)
{}							   

void Presentation::setup(void)   
{}

//Activated from Sketch loop and used for dynamic screens or other dynamic effects 
void Presentation::update(void)   
{
  
	
	//List all the dynamic screens  that would rely on periodic update here.
	//The current screen will be called and therefore shown as per the screen code  
	switch(currentScreenNo)
		{

    		case 3: //screen3
			  //do update here
			  screen3();
			break;
			case 4: //screen4
			  //do update here
			  screen4();
			break;
			//default:
			
        }



    //Other dynamic effects or actions that rely on periodic update is here


	
    //Save a a time when the update function is completed 
	uint32_t timestampUpdateCompleted = millis();	
	
}


//Activated from Controller and used for displaying one of the screens 
// 0 is to clear the display
void Presentation::PresentScreen(uint8_t screenNo) 
{
	
 switch(screenNo)
    {
        
		case 0: //clear display and set that there is no current screen 
		  _LCD1->clearDisplay();
		  currentScreenNo=0;
		break;
		
		case 1: //screen1
	      screen1();
		  currentScreenNo=1;
		break;
		 
		case 2: //screen2
		  screen2();
          currentScreenNo=2; 
		  break;
        
		case 3: //screen3
		  screen3();
          currentScreenNo=3;
		  break;
		
		case 4: //screen4
		  screen4();
		  currentScreenNo=4;
		  break;
		
		//default:
    }
}


//Static screen 1
//Shows voltage and a number of times a button is pressed
void Presentation::screen1(void) 
{

//get the voltage from DAO and show 
  _LCD1->clearDisplay();
  _LCD1->setTextSize(2);
  _LCD1->setTextColor(WHITE);
  _LCD1->setCursor(10,0);
  _LCD1->print("V:");
  _LCD1->println(_DAO->v);
  _LCD1->setCursor(0,16);
  _LCD1->print("Prst:");
  _LCD1->println(_DAO->cnt);
  _LCD1->display();
	

}

//Static screen 2 
//Sows a message that button is pressed 
void Presentation::screen2(void) 
{
  _LCD1->clearDisplay();
  _LCD1->setTextSize(1);
  _LCD1->setTextColor(WHITE);	
  _LCD1->setCursor(10,0);
  _LCD1->print("Screen2 - Button Pressed");
    //display the compiled picture		 
  _LCD1->display();
}

//Dynamic screen 3 
//Shows a blinking alarm message when voltage is over the limit 
void Presentation::screen3(void) 
{

//get the voltage from DAO and show 
  _LCD1->clearDisplay();
  _LCD1->setTextSize(2);
  _LCD1->setTextColor(WHITE);
  _LCD1->setCursor(10,0);
  _LCD1->println(_DAO->v);
  _LCD1->setCursor(0,16);
    
  // this is a dynamic section  of the screen
  	 uint16_t interval = 1000;    //blink interval
	 uint32_t currentMillis = millis();
	  if (currentMillis - previousMillis3 >= interval)
		 { //interval passed 
		   previousMillis3 = currentMillis;
		 }  
		 
        //show blinking warning message 		 
        if (currentMillis - previousMillis3 >= interval/2)
         {
		  _LCD1->println("Over Limit");  //"Over Limit" message shall be blinking. 
	     }
        else 
	     {
		    _LCD1->println("         ");  //"Over Limit" message shall be blinking.
	     }		  


  //display the compiled picture		 
  _LCD1->display();

}


//Dynamic screen 4 
//Moving bitmaps - a bit more complex example of a dynamic screen.  
void Presentation::screen4(void) 
{
	
     	//a small bitmap image
		static constexpr unsigned char PROGMEM logo16_glcd_bmp[32] =
		{ B00000000, B11000000,
		  B00000001, B11000000,
		  B00000001, B11000000,
		  B00000011, B11100000,
		  B11110011, B11100000,
		  B11111110, B11111000,
		  B01111110, B11111111,
		  B00110011, B10011111,
		  B00011111, B11111100,
		  B00001101, B01110000,
		  B00011011, B10100000,
		  B00111111, B11100000,
		  B00111111, B11110000,
		  B01111100, B11110000,
		  B01110000, B01110000,
		  B00000000, B00110000 };
		  
		  

		  // initialize the screen parameters - that shall happen when this screen
		  // called for the first time or replaces another screen, so we start from scratch 		  
		  if (currentScreenNo!=4)
		  {
			_LCD1->clearDisplay(); //clear old content 
			
				 for (uint8_t f=0; f< NUMFLAKES; f++) {
					icons[f][XPOS] = random(_LCD1->width());
					icons[f][YPOS] = 0;
					icons[f][DELTAY] = random(5) + 1;
					
					Serial.print("testdrawbitmap");
					Serial.print("x: ");
					Serial.print(icons[f][XPOS], DEC);
					Serial.print(" y: ");
					Serial.print(icons[f][YPOS], DEC);
					Serial.print(" dy: ");
					Serial.println(icons[f][DELTAY], DEC);
				  }
  
            }
		  
			// draw each icon as per positions recorded in icons[][] array
			for (uint8_t f=0; f< NUMFLAKES; f++) {
			  _LCD1->drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, 16, 16, WHITE);
			}
              //display the compiled picture		 
              _LCD1->display();
			
			// this is a delay(200)
				 uint16_t interval = 200;    
				 uint32_t currentMillis = millis();
				  if (currentMillis - previousMillis4 >= interval)
					 { //interval passed, continue 
					   previousMillis4 = currentMillis;
					        //do action
					   			//erase bitmaps and move them 
								for (uint8_t f=0; f< NUMFLAKES; f++) 
								{
								  //erase a bitmap
								  _LCD1->drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, 16, 16, BLACK);
								  //move a bitmap
								  icons[f][YPOS] += icons[f][DELTAY];
								  //if its gone outside of the screen then reinit this bitmap
								  if (icons[f][YPOS] > _LCD1->height()) 
								  {
									  icons[f][XPOS] = random(_LCD1->width());
									  icons[f][YPOS] = 0;
									  icons[f][DELTAY] = random(5) + 1;
								  }
								}
			                //end of action
					 }
				  else 
					 {
						//"wait", do nothing 
					 }		  

			//end of delay(200) 
		   
}



	





//Toggles a led on the specified pin  as an example of an output to user.
void Presentation::ToggleLed(int Pin)
{
  int  ledState = digitalRead(Pin);

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(Pin, ledState);
  }


//Produces an output to serial as another simulation for output to user.   
void Presentation::displaySerial(void)
{
    Serial.print("PRES:v  :");
    Serial.println(_DAO->v);	
	Serial.print("PRES:ts :");
    Serial.println(_DAO->ts);
	Serial.print("PRES:cnt:");
    Serial.println(_DAO->cnt);
	Serial.print("PRES:cnt2:");
    Serial.println(_DAO->cnt2);
	Serial.print("PRES:ms :");
    Serial.println(millis());
	Serial.print("PRES:32 :");
    Serial.println(digitalRead(32));
	Serial.print("PRES:20 :");
    Serial.println(digitalRead(20));
	Serial.print("PRES:21 :");
    Serial.println(digitalRead(21));
	Serial.print("previousMillis3 :");
    Serial.println(previousMillis3);
	Serial.print("previousMillis4 :");
    Serial.println(previousMillis4);
}



