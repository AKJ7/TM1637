#include "TM1637.h"


TM1637::MI2C::MI2C(uint8_t clkPin, uint8_t dataPin) : clkPin{clkPin}, dataPin{dataPin}
{
}

void TM1637::MI2C::beginTransmission() const
{
    digitalWrite(clkPin, HIGH);
    digitalWrite(dataPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(dataPin, LOW);
}

void TM1637::MI2C::endTransmission() const
{
    digitalWrite(clkPin, LOW);
    delayMicroseconds(2);
    digitalWrite(dataPin, LOW);
    delayMicroseconds(2);
    digitalWrite(clkPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(dataPin, HIGH);
}

uint8_t TM1637::MI2C::send(uint8_t value) const
{
    static uint8_t ACK;
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(clkPin, LOW);
        value & 0x01 ? digitalWrite(dataPin, HIGH) : digitalWrite(dataPin, LOW);
        delayMicroseconds(3);
        value >>= 1;
        digitalWrite(clkPin, HIGH);
        delayMicroseconds(3);
    }
    digitalWrite(clkPin, LOW);
    digitalWrite(dataPin, HIGH);
    digitalWrite(clkPin, HIGH);
    pinMode(dataPin, INPUT);
    delayMicroseconds(50);
    if ((ACK = digitalRead(dataPin)) == 0)
    {
        pinMode(dataPin, INPUT);
        digitalWrite(dataPin, LOW);
    }
    delayMicroseconds(50);
    pinMode(dataPin, OUTPUT);
    delayMicroseconds(50);
    return ACK;
}

TM1637::TM1637(uint8_t clkPin, uint8_t dataPin) : mI2C(clkPin, dataPin)
{
}

void TM1637::init()
{
    pinMode(mI2C.clkPin, OUTPUT);
    pinMode(mI2C.dataPin, OUTPUT);
    offMode();
}

uint8_t TM1637::fetch(uint8_t value) const noexcept
{
    if (value == minus)
        return minus;
    if (value <= 9)
        return numbers[value];
    return alphabet[value - 97];
}

void TM1637::display(uint8_t reg_address, uint8_t value) const
{
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(DataCommand_e::FIXED_ADDRESS));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(0xC0 | reg_address);
    mI2C.send(fetch(value));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(brightness));
    mI2C.endTransmission();
}

void TM1637::send_fixed_address(AddressCommand_e addressCommand, uint8_t value)
{
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(DataCommand_e::FIXED_ADDRESS));
    mI2C.endTransmission();
    mI2C.send(static_cast<uint8_t>(addressCommand));
    mI2C.send(value);
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(brightness));
    mI2C.endTransmission();
}

void TM1637::display(uint8_t value[4]) const
{
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(DataCommand_e::AUTOMATIC_ADDRESS_ADDING));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(AddressCommand_e::C0H));
    for (uint8_t i = 0; i < TOTAL_DIGITS; i++)
        colon ? mI2C.send(fetch(value[i]) + 0x80) : mI2C.send(fetch(value[i]));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(brightness));
    mI2C.endTransmission();
}

void TM1637::changeDisplayControl(DisplayControl_e displayControl) const
{
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(DataCommand_e::FIXED_ADDRESS));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(AddressCommand_e::C6H));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(displayControl));
    mI2C.endTransmission();
}

void TM1637::offMode()
{
    changeDisplayControl(DisplayControl_e::DISPLAY_OFF);
}

void TM1637::onMode()
{
    changeDisplayControl(DisplayControl_e::DISPLAY_ON);
}

TM1637::DisplayControl_e TM1637::fetchControl(uint8_t value) const noexcept
{
    switch(value)
    {
        case 0:
            return DisplayControl_e::DISPLAY_OFF;
        case 1:
            return DisplayControl_e::PULSE_WIDTH_1_16;
        case 2:
            return DisplayControl_e::PULSE_WIDTH_2_16;
        case 3:
            return DisplayControl_e::PULSE_WIDTH_4_16;
        case 4:
            return DisplayControl_e::PULSE_WIDTH_10_16;
        case 5:
            return DisplayControl_e::PULSE_WIDTH_11_16;
        case 6:
            return DisplayControl_e::PULSE_WIDTH_12_16;
        case 7:
            return DisplayControl_e::PULSE_WIDTH_13_16;
        case 8:
            return DisplayControl_e::PULSE_WIDTH_14_16;
        default:
            return DisplayControl_e::PULSE_WIDTH_2_16;
    }
}
/**
 *  @param value any value between 0 and 8.
 *  0 turns off the display
 *  @note Any value above 8 and below 0 will be assigned the defaulted brightness,
 *  which is 2/16 or rather /a value = 2
 */
void TM1637::changeBrightness(uint8_t value) const
{
    changeDisplayControl(fetchControl(value));
}

void TM1637::setBrightness(uint8_t value) noexcept
{
    brightness = fetchControl(value);
}
void TM1637::switchColon() noexcept
{
    colon = !colon;
}