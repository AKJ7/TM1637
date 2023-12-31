#include <TM1637.h>

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

TM1637 tm(2, 3);

static const auto d = DisplayDigit().setA().setE().setF().setD();
static const uint8_t rawBuffer[4] = {d, 0x09, DisplayDigit().setA().setD(), 15};

void setup()
{
    tm.begin();
    tm.setBrightness(4);
}

void loop()
{
    tm.displayRawBytes(rawBuffer, 4)->blink(250);
    delay(250);
}