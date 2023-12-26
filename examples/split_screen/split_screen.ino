/***
 * @file split_screen.ino
 * @ingroup examples
 * @brief Split screen example
 *
 * This example shows how to use the split screen functionality of the library by
 * displaying the unit, here degree Celcius ("°") once, and the
 * temperature which changes each 2 seconds.
 *
 * The temperature values start at offset 1 (from the left side of the display)
 * leaving one digit for the unit (which start at offset 0).
 *
 * This feature could be used to save power, as one part of the display needs to be
 * updated, or could just be used to display different contents on the same display
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
        inline Animator *offMode() const noexcept;
        inline Animator *onMode() const noexcept;
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
        operator uint8_t();
    }
 */

#include <TM1637.h>

// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm(2, 3);

// ° Symbol at the 4th position. All segments of the first 3 values are off
const uint8_t degree[] = {0x00, 0x00, 0x00, DisplayDigit().setA().setB().setF().setG()};

// Some get temperature function. Returns random temperatures between -10 and 30
int getTemperature()
{
    return static_cast<int8_t >(random(-10, 30));
}

void setup()
{
    tm.init();                              // Initializes the display
    tm.setBrightness(3);                    // Set brightness to level 3
}

void loop()
{

    tm.displayRawBytes(degree, sizeof(degree));
    tm.display(getTemperature(), false, false);    // Display the random temperature starting at position 1 and fill the
    // spots on the right with zeros
    delay(2000);
}