/**
 * @file basic.ino
 * @ingroup examples
 * @brief Basic library usage example
 *
 * This example shows how to display different type of values on the display.
 */

#include <TM1637.h>


// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm(2, 3);

void setup()
{
    tm.begin();
    tm.setBrightness(4);
}

void loop()
{
    // Display Integers:
    tm.display(1234);
    delay(1000);

    // Display float:
    tm.display(2.65);
    delay(1000);

    // Display String:
    tm.display("PLAY");
    delay(1000);
    tm.display("STOP");
    delay(1000);
}