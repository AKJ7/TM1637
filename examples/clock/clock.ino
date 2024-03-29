/***************************************
 * @file clock.ino
 * @ingroup examples
 * @brief Clock example
 *
 * With this example, the arduino displays
 * a clock that counts from 12:00 to 12:59
 * while toggling the colon once every second.
 * Once, the clocks reached 12:10, it stops
 * and starts blinking endlessly
 * To restarts, simply press the reset button
 * on the arduino.
 *
 * Please note that 1 second is simulated,
 * by using the delay(1000) function. This
 * halts the controller for 1000 milliseconds.
 * however, since other processes take processor's
 * resources, the total delay before the next
 * time increment, will be bigger than 1 second.
 * The clock is therefore slower than a regular clock.
 * I advice the user of this library to set the delay
 * accordingly, if they really wish to achieve better
 * precision.
 ****************************************/

/**
 *
 * Visit https://github.com/AKJ7/TM1637/ for more info
 *
 * API
    class TM1637 {
    public:
        TM1637(uint8_t clkPin, uint8_t dataPin) noexcept: animator_(clkPin, dataPin, TOTAL_DIGITS) {};
        TM1637(const TM1637 &) = delete;
        TM1637 &operator=(const TM1637 &) = delete;
        ~TM1637() = default;
        void begin();
        inline void init();
        inline Animator *refresh();
        template<typename T>
        typename type_traits::enable_if<
                type_traits::is_string<T>::value ||
                type_traits::is_floating_point<T>::value ||
                type_traits::is_integral<T>::value,
                Animator *>::type
        display(const T value, bool overflow = true, bool pad = false, uint8_t offset = 0);
        Animator *displayRawBytes(const uint8_t *buffer, size_t size);
        inline Animator *offMode() noexcept;
        inline Animator *onMode() noexcept;
        inline Animator *colonOff() noexcept;
        inline Animator *colonOn() noexcept;
        inline Animator *switchColon() noexcept;
        inline Animator *clearScreen() noexcept;
        inline Animator *setDp(uint8_t value) noexcept;
        inline uint8_t getBrightness() const noexcept;
        inline float getBrightnessPercent() const noexcept;
        void changeBrightness(uint8_t value) noexcept;
        void setBrightness(uint8_t value) noexcept;
        inline Animator *setBrightnessPercent(float percent) noexcept;
        inline Animator *changeBrightnessPercent(float percent) noexcept;
        inline void setFloatDigitCount(unsigned count);
    };

    class Animator
    {
        Animator(uint8_t clkPin, uint8_t dataPin, uint8_t totalDigits);
        void blink(Tasker::duration_type delay);
        void fadeOut(Tasker::duration_type delay);
        void fadeIn(Tasker::duration_type delay);
        void scrollLeft(Tasker::duration_type delay);
        void off() const;
        void on(DisplayControl_e brightness) const;
        void reset(const String& value);
        inline void resetAnimation() noexcept;
        void clear();
        void refresh();
    }

    struct DisplayDigit
    {
        DisplayDigit& setA();
        DisplayDigit& setB();
        DisplayDigit& setC();
        DisplayDigit& setD();
        DisplayDigit& setE();
        DisplayDigit& setF();
        DisplayDigit& setG();
        DisplayDigit& setDot();
        DisplayDigit& operator=(uint8_t rhs);
        operator uint8_t() const;
    }
 */

#include <TM1637.h>

// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm1637(2, 3);

void setup()
{
    tm1637.init();
    tm1637.setBrightnessPercent(50);
}

void loop()
{
    for (int minutes = 12; minutes < 60; minutes++) {
        for (int seconds = 0; seconds < 60; seconds++) {
            if (minutes == 12 && seconds == 10) {
                tm1637.display(seconds + minutes * 100)->blink(1000); // Display the time and blink each 1 second
                --seconds; // To keep the loop at this stage
            } else {
                tm1637.display(seconds + minutes * 100); // Display the time
                tm1637.switchColon(); // Switch the colon. Your display must support it.
            }
            delay(1000);
        }
    }
}