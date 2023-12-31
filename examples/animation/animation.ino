/**
 * @file animation.ino
 * @ingroup examples
 * @brief Basic animation usage example
 *
 * This example shows how to animate values on the display.
 */

/**
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
TM1637 tm(2, 3);

void setup()
{
    tm.begin();
    tm.setBrightnessPercent(70);
}

void loop()
{
    for (unsigned counter = 0; counter < 100; ++counter)
    {
        if (counter < 10) {
            tm.display(9876)->blink(1500); // Display 9876 and blink every 1.5 seconds
            delay(1000);
            continue;
        }
        if (counter < 80) {
            tm.display("PLAY-STOP")->scrollLeft(500); // Scroll "PLAY-STOP" to the left each 500 milliseconds
            delay(500);
            continue;
        }
        if (counter < 90) {
            tm.display(1.234354534)->fadeOut(800);  // Fade out 1.2345... every 800 milliseconds
            delay(1000);
            continue;
	}
        if (counter < 100) {
            tm.display(1.234354534)->fadeIn(800); // Fade the same number in every 800 milliseconds
            delay(1000);
            continue;
        }
    }
}
