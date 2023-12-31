#ifndef TM1637_TM1637_H
#define TM1637_TM1637_H

#include <Arduino.h>
#include "type_traits.h"
#include "animator.h"

#ifndef TM1637_LEGACY
#define TM1637_LEGACY 1
#endif

#if __GNUC__ >= 3
#define DEPRECATED          __attribute__((deprecated))
#else
#define DEPRECATED
#endif

/**
 * @brief Interface to help build single segment in a functional
 * way
 *
 * @details A single 7-Segment display is structured as followed:
 *    -- A --
 *    |      |
 *    F      B
 *    |      |
 *    --  G --
 *    |      |
 *    E      C
 *    |      |
 *    -- D --   -Dot-
 */
struct DisplayDigit {
    /**
     * @brief Set the A segment
     * @return self
     */
    DisplayDigit &setA() {
        value |= 0x01;
        return *this;
    }

    /**
     * @brief Set the B segment
     * @return self
     */
    DisplayDigit &setB() {
        value |= 0x02;
        return *this;
    }

    /**
     * @brief Set the C segment
     * @return self
     */
    DisplayDigit &setC() {
        value |= 0x04;
        return *this;
    }

    /**
     * @brief Set the D segment
     * @return self
     */
    DisplayDigit &setD() {
        value |= 0x08;
        return *this;
    }

    /**
     * @brief Set the E segment
     * @return self
     */
    DisplayDigit &setE() {
        value |= 0x10;
        return *this;
    }

    /**
     * @brief Set the F segment
     * @return self
     */
    DisplayDigit &setF() {
        value |= 0x20;
        return *this;
    }

    /**
     * @brief Set the G segment
     * @return self
     */
    DisplayDigit &setG() {
        value |= 0x40;
        return *this;
    }

    /**
     * @brief Set the Dot segment
     * @return self
     */
    DisplayDigit &setDot() {
        value |= 0x80;
        return *this;
    }

    /**
     * Allow implicit static casting to uint8_t
     * @return Encoded digit in byte
     */
    operator uint8_t() const { return value; }

    /**
     * Allow implicit conversion from uint8_t
     * @param rhs Already encoded byte as uint8_t
     * @return self
     */
    DisplayDigit &operator=(uint8_t rhs) {
        value = rhs;
        return *this;
    }

    uint8_t value{};
};


/**
 * @brief Facade to the underlying access to the display device
 * @details Wrapper to the low-level functions
 */
class TM1637 {
    friend Animator;

public:
    static constexpr uint8_t TOTAL_DIGITS = 4;

    /**
     * @brief Construct a TM1637 Instance
     * @param clkPin Port number of the connected display CLK pin
     * @param dataPin Port number of the connected display DATA pin
     */
    TM1637(uint8_t clkPin, uint8_t dataPin) noexcept: animator_(clkPin, dataPin, TOTAL_DIGITS) {};

    /**
     * Deleted Copy Constructor
     */
    TM1637(const TM1637 &) = delete;

    /**
     * Deleted Copy Assign Constructor
     */
    TM1637 &operator=(const TM1637 &) = delete;

    /**
     * Default Destructor
     */
    ~TM1637() = default;

    /**
     * Initialise the low-level communication with the display device
     */
    void begin() {
        animator_.mi2C_.begin();
        offMode();
    };

    /**
     * Alias for `TM1637::begin()`
     */
    inline void init() { begin(); }

    /**
     * Send the underlying display buffer to the display device
     * @return Current instance of the animator
     */
    inline Animator *refresh() {
        animator_.refresh();
        return &animator_;
    }

    /**
     * Print/Display some values on the display
     * @param value Content to display
     * @param overflow Cache more characters than the display allows. Useful for scrolling
     * @param pad Fill the unoccupied 7-segments displays with 0
     * @param offset Start Position to display from
     * @return Current instance of the animator
     */
    template<typename T>
    typename type_traits::enable_if<
            type_traits::is_string<T>::value ||
            type_traits::is_floating_point<T>::value ||
            type_traits::is_integral<T>::value,
            Animator *>::type
    display(const T value, bool overflow = true, bool pad = false, uint8_t offset = 0) {
        String temp = stringer<T>(value);
        if (temp == cache_)
            return &animator_;
        cache_ = temp;
        String cache;
        cache = "";
        for (decltype(offset) counter{}; counter < offset; ++counter) {
            cache.concat(static_cast<char>(0x00));
        }
        cache.concat(temp);
        if (pad) {
            for (size_t counter = cache.length(); counter < animator_.totalDigits_; ++counter) {
                cache.concat(static_cast<char>('O'));
            }
        }
        if (!overflow) {
            cache.substring(0, animator_.totalDigits_);
        }
        animator_.reset(cache);
        return refresh();
    }

    /**
     * @brief Display raw bytes (already encoded binaries) to the display
     * @warning The buffer is not cached
     * @param buffer The data to display
     * @param size Size to the data to display
     * @return Instance of the current animator
     */
    Animator *displayRawBytes(const uint8_t *buffer, size_t size) {
        cache_ = "";
        animator_.buffer_ = "";
        for (decltype(size) counter{}; counter < size; ++counter) {
            animator_.buffer_.concat(static_cast<char>(buffer[counter]));
        }
        return refresh();
    }

    /**
     * @brief Turn display off
     * @return Instance of the current animator
     */
    inline Animator *offMode() noexcept {
        animator_.off();
        return &animator_;
    };

    /**
     * @brief Turn display on
     * @return Instance of the current animator
     */
    inline Animator *onMode() noexcept {
        animator_.on(animator_.brightness_);
        return &animator_;
    };

    /**
     * @brief Turn the colon off
     * @note This feature is not supported by all TM1637 displays
     * @return Instance of the current animator
     */
    inline Animator *colonOff() noexcept {
        animator_.colon_ = false;
        animator_.refresh();
        return &animator_;
    };

    /**
     * @brief Turn the colon on
     * @note This feature is not supported by all TM1637 displays
     * @return Instance of the current animator
     */
    inline Animator *colonOn() noexcept {
        animator_.colon_ = true;
        animator_.refresh();
        return &animator_;
    };

    /**
     * @brief Toggle the state of the colon
     * @return Instance of the current animator
     */
    inline Animator *switchColon() noexcept {
        animator_.colon_ = !animator_.colon_;
        animator_.refresh();
        return &animator_;
    };

    /**
     * @brief Fill the screen with 0s
     * @return Instance of the current animator
     */
    inline Animator *clearScreen() noexcept {
        animator_.clear();
        cache_ = "";
        return refresh();
    };

    /**
     * @brief Set the value of the Dot
     * @param value Value of the dop
     * @return Instance of the current animator
     */
    inline Animator *setDp(uint8_t value) noexcept {
        animator_.dp_ = value;
        return refresh();
    }

    /**
     * @brief Get the current brightness level of the display
     * @return uint8_t Level of the display's brightness
     */
    inline uint8_t getBrightness() const noexcept { return static_cast<uint8_t>(animator_.brightness_); }

    /**
     * @brief Get the current brightness level of the display in percent
     * @return float Level of the diplay's brightness in float
     */
    inline float getBrightnessPercent() const noexcept {
        return Animator::control2Percent(animator_.brightness_);
    }

    /**
     * @brief Immediately change the brightness of the display
     * @param value Display's brightness value between 0 and 8
     */
    void changeBrightness(uint8_t value) noexcept {
        animator_.brightness_ = Animator::fetchControl(value);
        refresh();
    };

    /**
     * @brief Set the brightness level of the display.
     * @note The display doesn't refresh immediately, the new brightness is
     * applied after the first `display()` call
     * @param value Display's brightness value between 0 and 8
     */
    void setBrightness(uint8_t value) noexcept {
        animator_.brightness_ = Animator::fetchControl(value);
    };

    /**
     * @brief Set brightness level of the display in percentage
     * @param percent Display's brightness value between 0 and 100 percent
     * @note The brightness of the display in percent is not linearly interpolated
     * @return Instance of the current animator
     */
    inline Animator *setBrightnessPercent(float percent) noexcept {
        animator_.brightness_ = Animator::fetchControlPercent(percent);
        return &animator_;
    }

    /**
     * @brief Immediately change the brightness of the display in percentage
     * @param percent Display's brightness value between 0 and 100 percent
     * @return Instance of the current animator
     */
    inline Animator *changeBrightnessPercent(float percent) noexcept {
        animator_.brightness_ = Animator::fetchControlPercent(percent);
        return refresh();
    }

    /**
     * @brief Set the number of digits when displaying float values
     * @param count number of digit values for float
     */
    inline void setFloatDigitCount(unsigned count) {
        floatDecimal_ = count;
    }

private:
    template<typename T>
    typename type_traits::enable_if<type_traits::is_string<T>::value, String>::type stringer(T value) {
        return value;
    }

    template<typename T>
    typename type_traits::enable_if<type_traits::is_integral<T>::value, String>::type stringer(T value) {
        return String(value);
    }

    template<typename T>
    typename type_traits::enable_if<type_traits::is_floating_point<T>::value, String>::type stringer(T value) {
        return String(value, static_cast<unsigned>(floatDecimal_));
    }

private:
    Animator animator_;
    String cache_ = "";
    unsigned floatDecimal_ = TOTAL_DIGITS;
};

#endif //TM1637_TM1637_H
