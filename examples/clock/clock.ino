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