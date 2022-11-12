#ifndef _DIGITAL_GPIO_H
#define _DIGITAL_GPIO_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

class DigitalGPIO
{
private:
    uint pin;
    bool input;
    bool pullup;

public:
    DigitalGPIO(uint pin, bool input, bool pullup);
    void write(bool dir);
    bool read();
    ~DigitalGPIO();
};



#endif