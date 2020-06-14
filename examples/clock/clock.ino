/***************************************
* @file clock.ino
* @ingroup examples
* @brief Clock example
*
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

/** API:
 *
 * Visit https://github.com/AKJ7/TM1637 for more information
 *
 * @code{.cpp}
 * class TM1637
 * {
 *      TM1637(uint8_t clkPin, uint8_t dataPin, uint8_t bufferSize = TOTAL_DIGITS);
 *      void init();
 *      void begin();
 *      const Animator& refresh();
 *      void update();
 *      display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0);
 *      template <typename T>
 *      DEPRECATED void dispNumber(T value)
 *      void sendToDisplay(DisplayControl_e displayControl) const noexcept;
 *      template <DataCommand_e dataCommand, AddressCommand_e addressCommand>
 *      void sendToDisplay(uint8_t* values, uint8_t size) const noexcept;
 *      template <DataCommand_e dataCommand, AddressCommand_e addressCommand, typename ... Ts>
 *      void sendToDisplay(Ts ... values);
 *      void setBrightness(uint8_t value);
 *      void changeBrightness(uint8_t value);
 *      void setDp(uint8_t value);
 *      uint8_t getBrightness();
 *      void offMode();
 *      void onMode();
 *      void colonOn();
 *      void colonOff();
 *      void switchColon();
 *      void clearScreen();
 * }
 * @endcode
 */

#include <TM1637.h>


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
            tm1637.display(seconds + minutes * 100);
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
