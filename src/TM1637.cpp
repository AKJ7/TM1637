#include "TM1637.h"

#define TO_UINT8_T(value) static_cast<uint8_t>(value)

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
    uint8_t ACK;
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(clkPin, LOW);
        digitalWrite(dataPin, value & 0x01);
        delayMicroseconds(3);
        digitalWrite(clkPin, HIGH);
        delayMicroseconds(3);
        value >>= 1;
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

TM1637::Animator::Animator(const uint8_t* input) noexcept
{
}

TM1637::Animator* TM1637::Animator::set(const uint8_t* input) noexcept
{
    return this;
}

TM1637::Animator* TM1637::Animator::setOverflow(bool overflow)
{
    this->overflow = overflow;
    return this;
}

TM1637::Animator* TM1637::Animator::blink(size_t delay_ts)
{
    disp->offMode();
    delay(delay_ts);
    disp->onMode();
    return this;
}

TM1637::Animator* TM1637::Animator::scroll(size_t delay, uint8_t direction_left)
{

    return this;
};

TM1637::Animator* TM1637::Animator::fadeOut(size_t delay_ts)
{
   uint8_t counter = (TO_UINT8_T(disp->getBrightness()) - 0x88);
   do 
   {
        disp->changeBrightness(--counter);
        delay(delay_ts);
   } while (counter != 0);
   return this;
};

TM1637::Animator* TM1637::Animator::fadeIn(size_t delay_ts)
{
    uint8_t counter = (TO_UINT8_T(disp->getBrightness()) - 0x88);
    do {
        disp->changeBrightness(++counter);
        delay(delay_ts);
    } while (counter != 8);
    return this;
}

TM1637::TM1637(uint8_t clkPin, uint8_t dataPin) : mI2C(clkPin, dataPin), animator(this)
{
}

void TM1637::init()
{
    pinMode(mI2C.clkPin, OUTPUT);
    pinMode(mI2C.dataPin, OUTPUT);
    offMode();
}

void TM1637::begin()
{
    init();
}

void TM1637::update()
{
    display(digits);
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
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(addressCommand));
    mI2C.send(value);
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(brightness));
    mI2C.endTransmission();
}

void TM1637::display(uint8_t* value) const
{
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(DataCommand_e::AUTOMATIC_ADDRESS_ADDING));
    mI2C.endTransmission();
    mI2C.beginTransmission();
    mI2C.send(static_cast<uint8_t>(AddressCommand_e::C0H));
    for (uint8_t i = TOTAL_DIGITS; i; --i)
        // If colon is true, or bit i of dp is set, then turn on MSb
        (colon || (dp & (1 << i))) ? mI2C.send(fetch(value[i - 1]) + 0x80) : mI2C.send(fetch(value[i - 1]));
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
    changeDisplayControl(brightness);
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

TM1637::DisplayControl_e TM1637::getBrightness() const noexcept
{
    return brightness;
}

void TM1637::switchColon() noexcept
{
    colon = !colon;
}

void TM1637::setDp(uint8_t value) noexcept
{
    dp = value;
}

#undef TO_UINT8_T
