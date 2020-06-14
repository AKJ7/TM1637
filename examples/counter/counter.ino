/**
 * @file counter.ino
 * @ingroup examples
 *
 * @brief Simple counter example.
 *
 * This example shows how to initialize the display and display numbers.
 */

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
