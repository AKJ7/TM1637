# TM1637

TM1637 Library Driver for Microcontrollers.  

## Features  
- Supports displaying floating point numbers, integers, String  
- Supports custom raw data display  
- Tunable offset, pad, fill  
- Supports animation: blink, left scroll, fade in and fadeout with custom delay  
- Screen clearing, on/off mode, on/off colons  
- Tunable brightness  


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
    tm.display(29.65);
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
    inline uint8_t getBrightness() noexcept;
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
```

## Test  

 Tested on my Arduino Uno. Should work on every controller board that supports the Arduino platform.


## Contributions  

Since i am very lazy, feel free to contribute. Pull Requests get added faster ;-)

## Contributors

Many thanks to these contributors:

- [tjhenry](https://github.com/tjhenry)  
- [andrewerrington](https://github.com/andrewerrington)   
- [jcmadrioso](https://github.com/jcmadrioso)
- [Kostyantyn Matviyevskyy](https://github.com/kostyamat)
- [vortigont](https://github.com/vortigont)