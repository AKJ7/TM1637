#ifndef TM1637_H
#define TM1637_H

#include "Arduino.h"

#define TM1637_LEGACY 0
#define MAX_OVERFLOW 32

/****************************************************************
  * TODO:
  *     - Add scroll effect (Vertical scroll and horizontal scroll)
  *     - Add blink effect
  *     - Add fade effect
  *     - Add overflow support
  *
  **************************************************************/



namespace type_traits
{   
    // Type traits
    // TODO: Handle const and volatile types

    template <typename T, T v>
    struct integral_constant
    {
        static constexpr T value = v;
        using value_type = T;
        using type = integral_constant;
        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }
    };

    using true_type = integral_constant<bool, true>;
    using false_type = integral_constant<bool, false>;

    template <typename T>
    struct is_integral_base : false_type
    {};

    template <> struct is_integral_base<char> : true_type {};
    template <> struct is_integral_base<short>: true_type {};
    template <> struct is_integral_base<int>: true_type {};
    template <> struct is_integral_base<long>: true_type {};
    template <> struct is_integral_base<unsigned char>: true_type {};
    template <> struct is_integral_base<unsigned short>: true_type {};
    template <> struct is_integral_base<unsigned int>: true_type {};
    template <> struct is_integral_base<unsigned long>: true_type {};

    template <typename T>
    struct is_integral : is_integral_base<T>
    {};

    template <typename T> struct is_floating_point_base: false_type {};
    template <> struct is_floating_point_base<float>: true_type {};
    template <> struct is_floating_point_base<double>: true_type {};
    template <> struct is_floating_point_base<long double>: true_type {};
    template <typename T> 
    struct is_floating_point : is_floating_point_base<T>
    {};

    template <typename T> struct is_string_base: false_type {};
    template <> struct is_string_base<const char*>: true_type {};
    template <> struct is_string_base<String>: true_type {};

    template <typename T>
    struct is_string : is_string_base<T>
    {};

    template <bool B, class T = void>
    struct enable_if
    {};

    template <class T>
    struct enable_if <true, T> { using type = T; };
    
    template <bool B, class T = void>
    using enable_if_t = typename enable_if<B, T>::type;
};


class TM1637 {
public:
    static constexpr uint8_t TOTAL_DIGITS = 4;
    enum class DataCommand_e: uint8_t
    {
        FIXED_ADDRESS                   = 0x44,
        AUTOMATIC_ADDRESS_ADDING        = 0x40,
        READ_KEY_SCAN_DATA              = 0x42,
        WRITE_DATA_TO_DISPLAY_REGISTER  = 0x40,
        NORMAL_MODE                     = 0x40,
        TEST_MODE                       = 0x48
    };
    enum class AddressCommand_e: uint8_t
    {
        C0H                             = 0xC0,
        C1H,
        C2H,
        C3H,
        C4H,
        C5H,
        C6H                             // Ignore Scan
    };
    enum class DisplayControl_e: uint8_t
    {
        PULSE_WIDTH_1_16                = 0x88,
        PULSE_WIDTH_2_16,
        PULSE_WIDTH_4_16,
        PULSE_WIDTH_10_16,
        PULSE_WIDTH_11_16,
        PULSE_WIDTH_12_16,
        PULSE_WIDTH_13_16,
        PULSE_WIDTH_14_16,
        DISPLAY_OFF                     = 0x80,
        DISPLAY_ON                      = 0x88
    };

    /**
     *  @class mI2C
     *  @brief Modified I2C that the display supports.
     *         The communication protocoll used by the display is similar to the I2C,
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

    template <size_t NUM, typename T>
    class Array
    {
    private:
        T buffer[NUM] = {};
        friend TM1637;
    public:
        Array() = default;
        Array(const Array& rhs)
        {

        }
        Array(Array&& rhs)
        {
        }
        T& operator[](size_t index) noexcept
        {
            return buffer[index];
        }
        T operator[](size_t index) const noexcept
        {
            return buffer[index];
        }
        ~Array() = default;
    };
    
    class Animator
    {
    private:
        static constexpr decltype(TOTAL_DIGITS) WIDTH = TOTAL_DIGITS; 
        bool overflow;
        const TM1637* disp;
    public:
        Animator(const TM1637* disp)
        {
            this->disp = disp;
        };
        Animator(const uint8_t* input) noexcept;
        Animator* set(const uint8_t* input);
        Animator* setOverflow(bool overflow);
        Animator* blink(size_t delay_ts);
        Animator* scroll(size_t delay, uint8_t direction_left);
        Animator* fadeIn(size_t delay);
        Animator* fadeOut(size_t delay);
        ~Animator() = default;
    };

private:
    void changeDisplayControl(DisplayControl_e displayControl) const;
    DisplayControl_e fetchControl(uint8_t value) const noexcept;
    friend Animator;
    TM1637(const TM1637&) = delete;                                     // No copy for you man!
    TM1637& operator=(const TM1637&) = delete;
    Array<MAX_OVERFLOW, uint8_t> digits;

protected:
    MI2C mI2C;
    Animator animator;
    DisplayControl_e brightness = DisplayControl_e::PULSE_WIDTH_4_16;
    bool colon                           = false;
    uint8_t dp                           = 0x00;                        // dp LED bit pattern. LSb is leftmost dp.
    uint8_t numbers[10] = {0x3f, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    uint8_t alphabet[6] = {0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
    static constexpr uint8_t minus       = 0x40;
    static constexpr uint8_t empty       = 0x00;
    virtual uint8_t fetch(uint8_t value) const noexcept;
    
public:
    TM1637(uint8_t clkPin, uint8_t dataPin) noexcept;
    void init();
    void begin();
    void update();
    void display(uint8_t reg_address, uint8_t value) const;
    void display(uint8_t* value) const;
    template <size_t NUM, typename T>
    void display(const Array<NUM, T>& arr)
    {
        display((T*)arr.buffer);
    }
    template <typename T>
    typename type_traits::enable_if<type_traits::is_integral<T>::value, const Animator*>::type display2(T value, bool overflow = true, bool pad = true, uint8_t offset = 0)
    {
        bool negativ = (value < 0);
        if (offset > TOTAL_DIGITS - 1)
            return 0;
        if (negativ) {
            value = -value;
        }
        uint8_t expected = overflow ? MAX_OVERFLOW - offset : TOTAL_DIGITS - offset;
        if (expected == 0)
            return 0;
        do {
           digits[offset++] = value % 10;
           value /= 10;
        } while(value && --expected);
        if (negativ && expected)
            digits[offset++] = minus;
        if (pad)
            while (expected)
                digits[offset++] = 0;
        update();
        return animator.setOverflow(overflow);
    };
    template <typename T>
    typename type_traits::enable_if<type_traits::is_floating_point<T>::value, const Animator*>::type display2(T value)
    {
        return &animator;
    };
    template <typename T>
    typename type_traits::enable_if<type_traits::is_string<T>::value, const Animator*>::type display2(T value)
    {

        return &animator;
    };
    void send_fixed_address(AddressCommand_e addressCommand, uint8_t value);
    void setBrightness(uint8_t value) noexcept;
    void changeBrightness(uint8_t value) const;
    void switchColon() noexcept;
    void setDp(uint8_t value);
    void offMode();
    void onMode();
    DisplayControl_e getBrightness() const noexcept;
#if TM1637_LEGACY
    template <typename T>
    void dispNumber(T value);
#endif
    TM1637() = default;
};


#if TM1637_LEGACY
template <typename T>
void TM1637::dispNumber(T value)
{
    display(value);
}
#endif

#endif // TM1637_H
