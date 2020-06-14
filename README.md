# TM1637

TM1637 Library Driver for Microcontrollers  

## Features  
- Supports integers, float and string displaying  
- Very simple API  
- Very scalable API
- Supportable ASCII characters support  
- Supports overflow, padding and offset values
- Supports Brightness modification
- On/Off Mode
- On/Off colon
- Screen clearing 

## Example
Basic example:

```cpp

#include <TM1637.h>


// Instantiation and pins configurations
// Pin 3 - > DIO
// Pin 2 - > CLK
TM1637 tm(2, 3);

void setup()
{
    tm.begin();
}

void loop()
{
    // Display Integers:
    tm.display(1234);
    delay(1000);

    // Display float:
    tm.display(2.65);
    delay(1000);

    // Display String:
    tm.display("PLAY");
    delay(1000);
    tm.display("STOP");
    delay(1000);
}
```

## API
```cpp

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
    display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0);
    template <typename T>
    typename type_traits::enable_if<type_traits::is_floating_point<T>::value, const Animator&>::type
    display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0);
    template <typename T>
    typename type_traits::enable_if<type_traits::is_string<T>::value, const Animator&>::type
    display(T value, bool overflow = true, bool pad = true, uint8_t offset = 0);
    const Animator& display(const String& s, bool overflow = true, bool pad = true, uint8_t offset = 0);

#if TM1637_LEGACY
    DEPRECATED void update();
    template <typename T>
    DEPRECATED void dispNumber(T value);
#endif


public:

    void sendToDisplay(DisplayControl_e displayControl) const noexcept;
    template <DataCommand_e dataCommand, AddressCommand_e addressCommand>
    void sendToDisplay(uint8_t* values, uint8_t size) const noexcept;
    template <DataCommand_e dataCommand, AddressCommand_e addressCommand, typename ... Ts>
    void sendToDisplay(Ts ... values) const noexcept;
    void setBrightness(uint8_t value) noexcept;
    void changeBrightness(uint8_t value) noexcept;
    void setDp(uint8_t value) noexcept;
    uint8_t getBrightness() const noexcept;
    void offMode() const noexcept;
    void onMode() const noexcept;
    void colonOn() noexcept;
    void colonOff() noexcept;
    void switchColon() noexcept;
    void clearScreen() const noexcept;

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
};

```


## TODO

- [ ] Add Documentation
- [ ] Add Animation support

## Test  

 Tested on my Arduino Uno. Should work on every controller board that supports the Arduino platform .... hopefully.


## Contribution  

Since i am very lazy, feel free to contribute. Pull Requests get added faster ;-)

## Contributors

Many thanks to these contributors:

- [tjhenry](https://github.com/tjhenry)  
- [andrewerrington](https://github.com/andrewerrington)
