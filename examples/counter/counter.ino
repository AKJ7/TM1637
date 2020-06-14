/**
 * @file counter.ino
 * @ingroup examples
 *
 * @brief Simple counter example.
 *
 * This example shows how to initialize the display and display numbers.
 */

#include <TM1637.h>


// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm(2, 3);

void setup()
{
    tm.init();
}

void loop()
{
    for (int i = -100 ; i < 10000; i++) {
        tm.display(i);
        delay(1000);
    }
}
