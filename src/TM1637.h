#ifndef TM1637_TM1637_H
#define TM1637_TM1637_H

#include <Arduino.h>
#include "type_traits.h"
#include "animator.h"
#include "mI2C.h"

#ifndef TM1637_LEGACY
#define TM1637_LEGACY 1
#endif

#if __GNUC__ >= 3
#define DEPRECATED          __attribute__((deprecated))
#else
#define DEPRECATED
#endif



class TM1637 {
public:

    static constexpr uint8_t TOTAL_DIGITS = 4;

    enum class DataCommand_e : uint8_t
    {
        FIXED_ADDRESS               = 0x44,
        AUTOMATIC_ADDRESS_ADDING    = 0x40,
        READ_KEY_SCAN_DATA          = 0x42,
        WRITE_DATA_TO_DISPLAY_REGISTER = 0x40,
        NORMAL_MODE                 = 0x40,
        TEST_MODE                   = 0x48
    };

    enum class AddressCommand_e : uint8_t
    {
        C0H                         = 0xC0,
        C1H,
        C2H,
        C3H,
        C4H,
        C5H,
        C6H                             // Ignore Scan
    };

    enum class DisplayControl_e : uint8_t
    {
        PULSE_WIDTH_1_16            = 0x88,
        PULSE_WIDTH_2_16,
        PULSE_WIDTH_4_16,
        PULSE_WIDTH_10_16,
        PULSE_WIDTH_11_16,
        PULSE_WIDTH_12_16,
        PULSE_WIDTH_13_16,
        PULSE_WIDTH_14_16,
        DISPLAY_OFF                 = 0x80,
        DISPLAY_ON                  = 0x88
    };

    TM1637(uint8_t clkPin, uint8_t dataPin, uint8_t bufferSize = TOTAL_DIGITS) noexcept;

    TM1637(const TM1637 &) = delete;

    TM1637 &operator=(const TM1637 &) = delete;

    ~TM1637();

    void init();

    void begin();

    const Animator& refresh() const noexcept;

    template<typename T>
    typename type_traits::enable_if<type_traits::is_integral<T>::value, const Animator&>::type
    display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0)
    {
        bool negative;
        signed expected = overflow ? bufferSize_ - offset : TOTAL_DIGITS - offset;
        if (expected < 1)
            return animator_;
        if (value < 0) {
            negative = true;
            value = -value;
        } else {
            negative = false;
        }
        do {
            buffer_[--expected] = ascii[value % 10 + 0x30];
            value /= 10;
        } while (value && expected);
        if (negative && expected)
            buffer_[--expected] = minus;
        if (pad)
            while(expected > 0)
                buffer_[--expected] = ascii[0x30];
        refresh();
        return animator_;
    }

    template <typename T>
    typename type_traits::enable_if<type_traits::is_floating_point<T>::value, const Animator&>::type
    display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0)
    {
        String s(value);
        return display(s, overflow, pad, offset);
    }

    template <typename T>
    typename type_traits::enable_if<type_traits::is_string<T>::value, const Animator&>::type
    display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0)
    {
        signed expected = overflow ? bufferSize_ - offset : TOTAL_DIGITS - offset;
        if (expected < 1)
            return animator_;
        for (size_t counter{strlen(value)}; counter> 0 && expected > 0; --counter)
        {
            buffer_[--expected] = ascii[static_cast<uint8_t >(value[counter - 1])];
        }
        if (pad)
            while (expected > 0)
                buffer_[--expected] = ascii[0x30];
        refresh();
        return animator_;
    };

    const Animator& display(const String& s, bool overflow = true, bool pad = true, uint8_t offset = 0)
    {
        signed expected = overflow ? bufferSize_ - offset : TOTAL_DIGITS - offset;
        if (expected < 1)
            return animator_;

        for (size_t counter = s.length(); counter > 0 && expected > 0; --counter)
        {
            buffer_[--expected] = ascii[static_cast<uint8_t >(s[counter - 1])];
        }

        if (pad)
            while (expected > 0)
                buffer_[--expected] = ascii[0x30];

        refresh();
        return animator_;
    }

#if TM1637_LEGACY
    DEPRECATED void update();
    template <typename T>
    DEPRECATED void dispNumber(T value)
    {
        display(value, true, true, 0);
    }
#endif


public:

    void sendToDisplay(DisplayControl_e displayControl) const noexcept
    {
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t >(DataCommand_e::FIXED_ADDRESS));
        mI2C_.endTransmission();
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t>(AddressCommand_e::C6H));
        mI2C_.endTransmission();
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t >(displayControl));
        mI2C_.endTransmission();
    }

    template <DataCommand_e dataCommand, AddressCommand_e addressCommand>
    void sendToDisplay(uint8_t* values, uint8_t size) const noexcept
    {
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t >(dataCommand));
        mI2C_.endTransmission();
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t>(addressCommand));
        for (decltype(size) counter{}; counter < size; ++counter)
        {
            (colon_ || (dp_ & (1u << counter))) ? mI2C_.send(values[counter] | 0x80u) : mI2C_.send(values[counter]);
        }

        mI2C_.endTransmission();
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t >(brightness_));
        mI2C_.endTransmission();
    }

    template <DataCommand_e dataCommand, AddressCommand_e addressCommand, typename ... Ts>
    void sendToDisplay(Ts ... values) const noexcept
    {
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t >(dataCommand));
        mI2C_.endTransmission();
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t>(addressCommand));
        sendImp(values...);
        mI2C_.endTransmission();
        mI2C_.beginTransmission();
        mI2C_.send(static_cast<uint8_t >(brightness_));
        mI2C_.endTransmission();
    }

    void setBrightness(uint8_t value) noexcept;

    void changeBrightness(uint8_t value) noexcept;

    void setDp(uint8_t value) noexcept;

    uint8_t getBrightness() const noexcept
    {
        return static_cast<uint8_t >(brightness_);
    }

    void offMode() const noexcept;

    void onMode() const noexcept;

    void colonOn() noexcept;

    void colonOff() noexcept;

    void switchColon() noexcept;

    void clearScreen() const noexcept
    {
        for (uint8_t counter = TOTAL_DIGITS; counter; --counter)
            buffer_[counter] = 0;
        refresh();
    }

protected:

    const uint8_t ascii[128] =
    {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x22, 0x00, 0x6D, 0x00, 0x00, 0x20,
            0x39, 0x0F, 0x00, 0x00, 0x10, 0x40, 0x80, 0x52, 0x3F, 0x06,
            0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
            0x00, 0x48, 0x00, 0x53, 0x00, 0x77, 0x7C, 0x39, 0x5E, 0x79,
            0x71, 0x3D, 0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x54, 0x3F,
            0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00, 0x00, 0x6E,
            0x5B, 0x39, 0x64, 0x00, 0x0F, 0x08, 0x20, 0x77, 0x7C, 0x58,
            0x5E, 0x79, 0x71, 0x3D, 0x74, 0x04, 0x1E, 0x00, 0x38, 0x00,
            0x54, 0x5C, 0x73, 0x67, 0x50, 0x6D, 0x78, 0x3E, 0x00, 0x00,
            0x00, 0x6E, 0x5B, 0x39, 0x30, 0x0F, 0x40, 0x00,
    };

    bool                colon_;
    uint8_t             dp_;
    DisplayControl_e    brightness_;
    const MI2C          mI2C_;
    Animator            animator_;
    const uint8_t       bufferSize_;
    uint8_t*            buffer_;
    uint8_t minus = 0x40;


private:
    DisplayControl_e fetchControl(uint8_t value) const noexcept;

    // Argument passed by values, because most values are byte-size long.
    template <typename T>
    void sendImp(T value)
    {
        mI2C_.send(value);
    }

    template <typename T, typename ... Ts>
    void sendImp(T value, Ts ... values)
    {
        mI2C_.send(value);
        sendImp(values...);
    }
};


#endif //TM1637_TM1637_H
