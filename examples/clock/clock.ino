#include <TM1637.h>

/***************************************
* With this example, the arduino displays
* a clock that counts from 12:00 to 12:59
* then turns the display off. To restarts,
* simply press the reset button on the 
* arduino.
*
* Please note that 1 second is simulated,
* by using the delay(1000) function. This
* halts the controller for 1000 milliseconds.
* however, since other processes take processor's
* resources, the total delay before the next
* time increment, will be bigger than 1 second.
* The clock is therefore slower than a regular clock.
* I advice the user of this libary to set the delay
* accordinly, if he/she really wish to achieve better
* precision.
****************************************/


// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm1637(2, 3);

void setup()
{
    tm1637.init();
    tm1637.setBrightness(5);
}

void loop()
{
    for (int minutes = 12 ; minutes< 60; minutes++) {
        for (int seconds = 0; seconds < 60; seconds++) {
            tm1637.dispNumber(seconds + minutes * 100);
            delay(1000);            // Waits for 1 second = 1000 millisecond.
            tm1637.switchColon();
            if (minutes == 12 && seconds == 59) {
                // Turns the display off and keeps it off, until setBrightness() with different values is called
                // The display setting are saved!
                // tm1637.offMode(); Turns the display off, but the display will turn on, as soon as the loop continues
                // This is due to the fact that offMode() doesn't save the brightness value.
                tm1637.setBrightness(0); 
            }
        }
    }
}
