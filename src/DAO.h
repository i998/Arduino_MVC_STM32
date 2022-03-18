//Class DAO (Data Access Object).  Stores relevant data (like values form sensors), timestamps and counts for the solution 
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

#ifndef _DAO_H_
#define _DAO_H_


#include "Arduino.h"
class DAO { 
  
  
	public:
		DAO();
		~DAO();	
		
        //Anything to do as a part of setup 
		void setup(void);   
        //Anything  to do with regular updates
		void update(void);   // not currently used

		//public variables to store the data:
		uint16_t v=0;   // analog voltage
		uint32_t ts=0;  // timestamp (in miliseconds) when any button is last pressed
		
		uint16_t cnt=0; // count of  "single button pressed"
		uint16_t cnt2=0; // count of  "two button pressed"

};
#endif // _DAO_H_