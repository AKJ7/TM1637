#ifndef TM1637_H
#define TM1637_H

#include "Arduino.h"

class TM1637 {
public:
    enum class DataCommand_e: uint8_t
    {
        FIXED_ADDRESS = 0x44,
        AUTOMATIC_ADDRESS_ADDING = 0x40,
        READ_KEY_SCAN_DATA = 0x42,
        WRITE_DATA_TO_DISPLAY_REGISTER = 0x40,
        NORMAL_MODE = 0x40,
        TEST_MODE = 0x48
    };
    enum class AddressCommand_e: uint8_t
    {
        C0H = 0xC0,
        C1H,
        C2H,
        C3H,
        C4H,
        C5H,
        C6H  // Ignore Scan
    };
    enum class DisplayControl_e: uint8_t
    {
        PULSE_WIDTH_1_16 = 0x88,
        PULSE_WIDTH_2_16,
        PULSE_WIDTH_4_16,
        PULSE_WIDTH_10_16,
        PULSE_WIDTH_11_16,
        PULSE_WIDTH_12_16,
        PULSE_WIDTH_13_16,
        PULSE_WIDTH_14_16,
        DISPLAY_OFF = 0x80,
        DISPLAY_ON = 0x88
    };
    /**
     *  @class mI2C
     *  @brief Modified I2C that the display supports.
     *         The communication protocaoll used by the display is similar to the I2C,
     *         except for the fact, that it doesn't necessite the transmission of the
     *         slave address. The following class handles the low level communication
     *         between the Display and the controller.
     */
    class MI2C
    {
    private:
        uint8_t clkPin;
        uint8_t dataPin;
        friend TM1637;
    public:
        MI2C(uint8_t clkPin, uint8_t dataPin);
        void beginTransmission() const;
        void endTransmission() const;
        uint8_t send(uint8_t value) const;
        ~MI2C() = default;
    };

private:
    static constexpr uint8_t TOTAL_DIGITS = 4;
    void changeDisplayControl(DisplayControl_e displayControl) const;
    DisplayControl_e fetchControl(uint8_t value) const noexcept;

protected:
    MI2C mI2C;
    DisplayControl_e brightness = DisplayControl_e::PULSE_WIDTH_4_16;
    bool colon = false;
    uint8_t numbers[10] = {0x3f, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    uint8_t alphabet[6] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    uint8_t minus       = 0x40;
    uint8_t empty       = 0x00;
    virtual uint8_t fetch(uint8_t value) const noexcept;

public:

    TM1637(uint8_t clkPin, uint8_t dataPin);
    void init();
    void display(uint8_t reg_address, uint8_t value) const;
    void display(uint8_t value[4]) const;
    void send_fixed_address(AddressCommand_e addressCommand, uint8_t value);
    void setBrightness(uint8_t value);
    void changeBrightness(uint8_t value) const;
    void switchColon() noexcept;
    void offMode();
    void onMode();
    template <typename T>
    void dispNumber(T value);
    TM1637() = default;
};

template <typename T>
void TM1637::dispNumber(T value)
{
    uint8_t digit[TOTAL_DIGITS] {0};
    unsigned counter = 0;
    unsigned expected = 4;
    bool negative = false;
    if (value < 0 )
    {
        value = -value;
        expected = 3;
        negative = true;
    }
    while (value >= 10 && counter < expected)
    {
        digit[TOTAL_DIGITS - counter - 1] = value % 10;
        value /= 10;
        counter++;
    };
    digit[TOTAL_DIGITS - counter - 1] = value;
    counter++;
    if (negative)
        digit[TOTAL_DIGITS - counter - 1] = minus;
    display(digit);
}

#endif // TM1637_H