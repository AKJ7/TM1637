/**
 * @file basic.ino
 * @ingroup examples
 * @brief Basic library usage example
 *
 * This example shows how to display different type of values on the display.
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