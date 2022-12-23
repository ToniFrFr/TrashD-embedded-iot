#ifndef _DIGITAL_GPIO_H
#define _DIGITAL_GPIO_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

/**
 * @brief Basic digital gpio handling class, that can be used to configure gpio pins within code, has minimal use on this project, includes basic functionality of input/output configuration and 
 * reading and writing methods
 */

class DigitalGPIO
{
private:
    uint pin;
    bool input;
    bool pullup;
    bool invert;

public:
    DigitalGPIO(uint pin, bool input, bool pullup, bool invert);
    void write(bool dir);
    bool read();
    ~DigitalGPIO();
};




#endif