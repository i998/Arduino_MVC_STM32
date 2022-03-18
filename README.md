
## Overview:
I believe many of us needed to make an application that, in fact, does some basic things: 
   - Get input from a user
   - Collect data from other sources like sensors  
   - Process the information and display the results based on the data captured and/or user input provided
   - Rinse and Repeat

While it looks quite simple for the  first glance and almost all the code could just be placed in an Arduino sketch's *Loop()* and surrounding functions, the complexity increases quickly once you have several screens to show, need to cleanse captured data from errors,  take into account historical data,  and, of course, expect an input from user at any point of time.  

This is a working prototype of a sort-of MVC pattern for Arduino / STM32 which could help with such scenarios. It has been designed to split relevant operations into three domains - Data, Control and Presentation (yes, it is not a true canonical MVC, but I hope you see an analogy here) so there are three classes in the prototype. Together with the sketch itself, it helps to decouple functions that the code performs and make easier to manage and modify the code in future.

The prototype has been built on a Maple Mini STM32 board. With practically no difference with Arduino in price, it offers much better performance, bigger memory and hardware I/O interfaces which are easy to deal with. 
In this prototype we can:
   - simulate a signal from an external sensor with a variable potentiometer (Maple Mini converts an analog read to a  value from 0 to 4095 which is called "voltage" in  this  prototype) 
   - capture user input with buttons (one built-in and a couple extra ones)  
   - display various screens and data on a small LCD display 
   - and also use Serial and built-in LED to provide output to users   
   
Here is the logic that is included in the current version of the prototype: 
   - Show a message in the display while a dedicated button is pressed
   - Get a "voltage" figure, "debounce" it and then show it in the display
   - If "voltage" figure is below the limit and it is more than 10 sec since any button is pressed, show a screensaver 
   - If a "voltage" figure is above a limit then show it in the display with a blinking warning, no screensaver
   - Meanwhile count how many times a button is pressed     	
   - Use other user output other than the LCD display -  use builtin LED and send some data to serial output 	  
   
Note for Arduino users:  
This prototype shall work with Arduino with the following changes:
   - Use pointer to built-in Wire object instead of setting up hardware I2C interface and TwoWire object 
   - Use a Timer library (like TimerOne) instead of setting up STM32 hardware timer 
   - Update HW_Pins.h file with relevant pins    

 

## Software:
You can see a solution overview diagram [here](https://github.com/i998/Arduino_MVC_STM32/blob/main/documentation/Arduino%20MVC%20Solution%20Overview.pdf).

How these components are connected to each other?  In brief, The three key classes (DAO, Controller and Presentation) instantiated in the sketch and access each other via pointers. The sketch's function *Setup()* calls the relevant *Setup()* method of each class  - that allows to perform any  required  setup procedures located inside these classes.  

Similarly,  The sketch's function *Loop()* calls the relevant *Update()* method of some classes - where we need a periodic update or perform actions on a regular basis. Controller's function *Update()* handles all the business logic and manages UI screen flows.  Once a screen needs to be shown, it calls  Presentation's *presentScreen()* with a screen number to show that screen.
 
Presentation then accesses DAO for a required data, compiles and shows the screen on the display. (note, we may have several displays configured  - all we would need is just extra pointers added to them into the Presentation class and some logic to select which one to use for a particular screen). The Presentation's function *Update()* is responsible for updating screens with a dynamic content,  like ones with blinking messages or screen savers.   

The code is mostly self-explanatory and has a lot of notes that could help in further modifications or enhancements of the prototype so please have a look there as well.   

Here is the list of software components used for the prototype: 

   - Arduino sketch. Used for creating class objects and running  loops  as usual. Also has a STM32 timer configured here as some of libraries to handle user input devices (like Encoder) require their timer interrupt service routine *ClickEncoder:service()* to be called every millisecond. (Arduino users may use a software library like TimerOne for that purpose.) Also the sketch has a configuration for an LCD display library and a Display object (Display1). 

   - HW_Pins.h file contains hardware configuration (which input or output devices connected to which pins of the board). This is an attempt to get them configured in one place so it is easier to manage them later. 

   - Class DAO (Data Access Object). Stores relevant data (like values form sensors), timestamps and counts for the solution. Can also store historical data. 

   - Class Presentation.  Creates visual representation of data and show it to users. The Presentation class instance has access to DAO via a pointer (DAO_A). It also has access to a Display object (Display1) via a pointer (LCD1_A) in order to allow output to the LCD display.  

   - Class Controller.  Performs all the necessary interactions between classes and data manipulations. The Controller class instance has access to DAO and Presentation objects via pointers.

   - FourButtons library. Configured in the Controller class and used for capturing user input from four dedicated buttons. The library provides an  example on how to configure callbacks, including routing callbacks to a class member via a static trampoline function. The callbacks are executed once a button or two buttons are pressed. The library also allows to query status of each button by calling its function *isBtnPressed()*.  

   - SSD1306_i2c library for STM32, for use with STM32 hardware I2C ports. It uses a pointer to the I2C object that STM32 has. The library can be used for a generic Arduino too with minimal changes and a pointer to the Arduino's Wire object. 

   - Adafruit_GFX Library for working with LCD display(s).  
  
   - Arduino for STM32 (https://github.com/rogerclarkmelbourne/Arduino_STM32) as a core for STM32

   
## Hardware:
In  order to run this prototype you will need: 
   - Maple Mini CPU module (https://stm32duinoforum.com/forum/wiki_subdomain/index_title_Maple_Mini.html)
   - A small SSD1306 I2C 128x32 display 
   - A variable resistor, any from 1K to 10K will fit 
   - A couple of buttons and a 1K resistor

See the wiring diagram [here](https://github.com/i998/Arduino_MVC_STM32/blob/main/documentation/Arduino%20MVC%20Wiring%20Diagram.JPG).


## Repository Contents:
   - /documentation - A circuit diagram, Data sheets, additional information
   - /src – a set of classes and libraries 

   
## Further improvements:
   - Add a rotary encoder as a user  input  device 
   - Add a hysteresis loop for better handling input signals when they are near the limits configured for alarm messages

   
## License:
Arduino/STM32 MVC prototype is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Arduino/STM32 MVC prototype is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

## Donation:
If you like this project or it helps you to reduce your development effort, you can buy me a cup of coffee :) 

[![paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/donate/?business=ifhone777-hub%40yahoo.com&currency_code=USD)

