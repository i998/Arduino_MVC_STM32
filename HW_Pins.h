// HW_Pins.h file which contains hardware configuration (which input or output devices connected to which pins)
// An attempt to get them configured in one plase so it is easier to manage them later. 
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

#ifndef _HW_PINS_H_
#define _HW_PINS_H_

//NOTE: Pin numbers used below are for Maple Mini!

// Pins for FourButtons
#define P1 32     //BULTIN BUTTON
#define P2 20    //-1 -> not connected 
#define P3 21    //-1 -> not connected 
#define P4 -1    //-1 -> not connected  

//Led
#define LEDPIN 33  //BULTIN LED

//analog pin for viltage input 
#define analogPin 3

//Set Up Display(s) 
//Pin number that can be used for  SSD1306 reset
#define LCD_RESET 4 


#endif
