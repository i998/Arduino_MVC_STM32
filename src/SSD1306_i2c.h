/*
Lightweight SSD1306_i2c library for STM32, for use with STM32 hardware I2C ports. Requires Adafruit_GFX Library.
Original library is from https://github.com/rogerclarkmelbourne/Arduino_STM32/tree/master/STM32F1/libraries/Adafruit_SSD1306
Updated by IF 
2022-03-10
- tested support of hardware I2C for Maple Mini board / STM32 (https://github.com/rogerclarkmelbourne/Arduino_STM32/)
- simplified to support I2C only and use a pointer to I2C object in order to reduce potential problems with redefining Wire etc 
=================================================================*/
/*********************************************************************
This is a library for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

These displays use SPI to communicate, 4 or 5 pins are required to  
interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/
#ifndef SSD1306_i2c_H
#define SSD1306_i2c_H

#include "Wire.h"  
#include <Adafruit_GFX.h>

/*=========================================================================
    SSD1306 Displays
    -----------------------------------------------------------------------
    The driver is used in multiple displays (128x64, 128x32, etc.).
    Select the appropriate display below to create an appropriately
    sized framebuffer, etc.
     SSD1306_128_64  128x64 pixel display
     SSD1306_128_32  128x32 pixel display
     SSD1306_96_16
    -----------------------------------------------------------------------*/
//   #define SSD1306_128_64
  #define SSD1306_128_32
//   #define SSD1306_96_16
/*=========================================================================*/

//Uncomment for debug
//#define  ENABLE_DEBUG_OUTPUT_SSD1306_i2c

//Check if display is defined 
#if defined SSD1306_128_64 && defined SSD1306_128_32
  #error "Only one SSD1306 display can be specified at once in SSD1306_i2c.h"
#endif
#if !defined SSD1306_128_64 && !defined SSD1306_128_32 && !defined SSD1306_96_16
  #error "At least one SSD1306 display must be specified in SSD1306_i2c.h"
#endif

//Set display height and width
#if defined SSD1306_128_64
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 64
#endif
#if defined SSD1306_128_32
  #define SSD1306_LCDWIDTH                  128
  #define SSD1306_LCDHEIGHT                 32
#endif
#if defined SSD1306_96_16
  #define SSD1306_LCDWIDTH                  96
  #define SSD1306_LCDHEIGHT                 16
#endif

//Set constants
#define BLACK 0
#define WHITE 1
#define INVERSE 2

// SSD1306 Commandset
// ------------------
// Fundamental Commands
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_NOP					0xE3
// Scrolling Commands
#define SSD1306_ACTIVATE_SCROLL     0x2F
#define SSD1306_DEACTIVATE_SCROLL   0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL  0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL   0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
// Addressing Setting Commands
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22
// Hardware Configuration Commands
#define SSD1306_SETSTARTLINE 0x40
#define SSD1306_SEGREMAP 0xA0
#define SSD1306_SETMULTIPLEX 0xA8
#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8
#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA
#define SSD1306_CHARGEPUMP 0x8D
// Timing & Driving Scheme Setting Commands
#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9
#define SSD1306_SETVCOMDETECT 0xDB
//Other
#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2
//I2C Address
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)
#define SSD1306_I2C_ADDRESS   0x3C	// 011110+SA0+RW - 0x3C or 0x3D
#define SSD1306_ADDR		0x3C   
#define SSD1306_ADDR_128_32	0x3C 
#define SSD1306_ADDR_128_64	0x3D




class SSD1306_i2c : public Adafruit_GFX {

 public:
  SSD1306_i2c(void);								   
  SSD1306_i2c(TwoWire *I2C);
  SSD1306_i2c(TwoWire *I2C, uint8_t rstPin);  
 ~SSD1306_i2c(void);
  
  //Sets up library and hardware. Include in setup() function in your sketch
  void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = SSD1306_I2C_ADDRESS, bool reset=false);
  
  void ssd1306_command(uint8_t c);
  void ssd1306_data(uint8_t c);

  void clearDisplay(void);
  void invertDisplay(uint8_t i);
  void display();

  void startscrollright(uint8_t start, uint8_t stop);
  void startscrollleft(uint8_t start, uint8_t stop);

  void startscrolldiagright(uint8_t start, uint8_t stop);
  void startscrolldiagleft(uint8_t start, uint8_t stop);
  void stopscroll(void);

  void dim(boolean dim);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

 private:
  // Core test 
	#ifdef ARDUINO_ARCH_STM32           // official "stm32duino core"
	   Wire *_i2c;   
	#elif defined(ARDUINO_ARCH_STM32F1)     //Roger Clark "libmaple core"
	  TwoWire *_i2c;  
	#elif defined(ARDUINO_ARCH_STM32F4)    //Roger Clark "libmaple f4 core"
	  TwoWire *_i2c; 
	#elif defined(__AVR__)  // Arduino 
	  Wire *_i2c;
	#elif defined(__arm__)  // Arduino
	   Wire *_i2c;
	#else   //  "unknown core"
		  Wire *_i2c;
	#endif

	//variables 
    int8_t _i2caddr, _vccstate, sid, sclk, dc, rst, cs;   	
    
volatile uint32 *mosiport, *clkport, *csport, *dcport;
   uint32_t  mosipinmask, clkpinmask, cspinmask, dcpinmask;

  inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
  inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

    //read from I2C
	uint8_t read8(uint8_t addr);
  
};

#endif //SSD1306_i2c_H
