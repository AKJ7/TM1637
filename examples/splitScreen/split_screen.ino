/***
 * @file split_screen.ino
 * @ingroup examples
 * @brief Split screen example
 *
 * This example shows how to use the split screen functionality of the library by
 * displaying the unit, here degree Celcius ("C") once, and the
 * temperature which changes each 2 seconds.
 *
 * The temperature values start at offset 1 (from the left side of the display)
 * leaving one digit for the unit (which start at offset 0).
 *
 * This feature could be used to save power, as one part of the display needs to be
 * updated, or could just be used to display different contents on the same display
 */

#include <TM1637.h>


// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm(2, 3);


// Some get temperature function. Returns random temperatures between -10 and 30
int getTemperature()
{
    return static_cast<int8_t >(random(-10, 30));
}

void setup()
{
    tm.init();                              // Initializes the display
    tm.setBrightness(3);                    // Set brightness to level 3
    tm.display("C", false, false, 0);        // Displays the 'C' character at position 0 and don't fill the empty
                                            // spots on the right
}

void loop()
{
    tm.display(getTemperature(), false, true, 1);    // Display the random temperature starting at position 1 and fill the
                                                    // spots on the right with zeros
    delay(2000);
}
