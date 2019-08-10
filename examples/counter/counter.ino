#include <TM1637.h>

// Instantiation and pins configurations
// Pin 2 - > DIO
// Pin 3 - > CLK
TM1637 tm(2, 3);

void setup()
{
    tm.init();
}

void loop()
{
    for (int i = -100 ; i < 10000; i++)
    {
        tm.dispNumber(i);
        delay(1000);
    }
}