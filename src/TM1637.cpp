#include "TM1637.h"


TM1637::TM1637(uint8_t clkPin, uint8_t dataPin, uint8_t bufferSize) noexcept: mI2C_{clkPin, dataPin}, bufferSize_{bufferSize}
{
    dp_ = false;
    brightness_ = DisplayControl_e::PULSE_WIDTH_4_16;
    colon_ = false;
    buffer_ = new uint8_t[bufferSize];
    for (decltype(bufferSize) counter{}; counter < bufferSize; ++counter)
        buffer_[counter] = 0;
}

void TM1637::begin()
{
    mI2C_.begin();
    offMode();
}

void TM1637::init()
{
    begin();
}

TM1637::DisplayControl_e TM1637::fetchControl(uint8_t value) const noexcept {
    switch (value) {
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
            return DisplayControl_e::PULSE_WIDTH_4_16;

    }
}

const Animator& TM1637::refresh() const noexcept
{
    sendToDisplay<DataCommand_e::AUTOMATIC_ADDRESS_ADDING, AddressCommand_e::C0H>(buffer_, bufferSize_);
    return animator_;
}

void TM1637::setBrightness(uint8_t value) noexcept
{
    brightness_ = fetchControl(value);
}

void TM1637::changeBrightness(uint8_t value) noexcept
{
    sendToDisplay(fetchControl(value));
}

void TM1637::offMode() const noexcept
{
    sendToDisplay(DisplayControl_e::DISPLAY_OFF);
}

void TM1637::onMode() const noexcept
{
    sendToDisplay(brightness_);
}

void TM1637::switchColon() noexcept
{
    colon_ = !colon_;
}

void TM1637::colonOn() noexcept
{
    colon_ = true;
}

void TM1637::colonOff() noexcept
{
    colon_ = false;
}

void TM1637::setDp(uint8_t value) noexcept
{
    dp_ = value;
}

#if TM1637_LEGACY
void TM1637::update()
{
    refresh();
}
#endif

TM1637::~TM1637()
{
    delete[] buffer_;
}
