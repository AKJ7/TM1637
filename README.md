# TM1637
TM1637 Library Driver for Microcontrollers  


## Introdution  
### What the heck is the TM1637?  
Supposely you have this library, you certainly come from the Arduino Library Manager.  
TM1637 is, according to the documentation, a kind of LED drive control special circuit with 
keyboard scan interface and it's internally integrated with MCU digital interface, data latch, LED 
high pressure drive and keyboard scan.  
Basically, it is a set of 7-segments display that are controlled by a integrated controller, with 
keyscan functionality.  
The advantages of the TM1637 include the fact that the load of displaying values to the displays is 
carried by the intergrated controller. This allows the user's controller to only send the values to 
display and not handle the extra task of mutliplexing.  
The TM1637 communicates using a kind of protocol (referred to in the source file as MI2C) that is similar to the I2C protocol. The only difference, is that the address and the MISO interface are not supported.  
The display however doesn't support displaying commas. So no floats and double displays.  

### About the library  
This library was originally intended to be used as part of my personal project. I thought it would be a 
waste to write a driver for the display and not share it.  
I tried to make it user and compiler friendly.  


## Theory of operation  
Since example are easily understood:  

## Example  
```C++
	#include <TM1637.h>
	// Instantiation and pins configurations
	// Pin 2 - > DIO
	// Pin 3 - > CLK
	TM1637 tm1637(2, 3);

	void setup()
	{
    		tm1637.init();
    		tm1637.setBrightness(5);
	}

	void loop()
	{
    		for (int minutes = 12 ; minutes< 60; minutes++)
    		{
        		for (int seconds = 0; seconds < 60; seconds++)
        		{
            			tm1637.dispNumber(seconds + minutes * 100);
            			delay(1000);
            			tm1637.switchColon();

            			if (minutes == 12 && seconds == 30)
            			{
                			tm1637.setBrightness(0); // Turns the display off and keeps it off, until setBrightness() with different values is called
     					// The display setting are saved!
                			//tm1637.offMode(); Turns the display off, but the display will turn on, as soon as the loop continues
               				 // This is due to the fact that offMode() doesn't save the brightness value.
           			 }
       			 }
    		}
	}	

```
This example simulates a clock. Notice that the clock is not accurate. It is slower than the normal clock.  

## Methods overview  
```C++  
    TM1637(uint8_t clkPin, uint8_t dataPin);
    void init();
    void display(uint8_t reg_address, uint8_t value) const;
    void display(uint8_t value[4]) const;
    void send_fixed_address(AddressCommand_e addressCommand, uint8_t value);
    void setBrightness(uint8_t value);
    void changeBrightness(uint8_t value) const;
    void switchColon() noexcept;
    void offMode();
    void onMode();
    template <typename T>
    void dispNumber(T value);
    TM1637() = default;
```
