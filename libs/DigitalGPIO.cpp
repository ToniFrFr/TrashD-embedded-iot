#include "DigitalGPIO.h"

/**
 * Create regular gpio pin
*/
DigitalGPIO::DigitalGPIO(uint pin, bool input, bool pullup=false, bool invert=false) : pin(pin), input(input), pullup(pullup), invert(invert)
{
    gpio_init(this->pin);

    if(input) {
        gpio_set_dir(this->pin, GPIO_IN);
    } else {
        gpio_set_dir(this->pin, GPIO_OUT);
    }

    if(input && pullup) {
        gpio_pull_up(this->pin);
    }
}
DigitalGPIO::~DigitalGPIO()
{
    gpio_deinit(this->pin);
}

void DigitalGPIO::write(bool dir) {
    if(invert) {
        gpio_put(this->pin, !dir);
    } else {
        gpio_put(this->pin, dir);
    }
}

bool DigitalGPIO::read() {
    if(invert) {
        return !gpio_get(this->pin);
    } else {
        return gpio_get(this->pin);
    }

    
}