#ifndef _CUSTOM_SPI_H
#define _CUSTOM_SPI_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"

class CustomSPI
{
private:
    uint CS;
    uint SCK;
    uint MISO;
    uint MOSI;
    uint BAUDRATE;
    spi_inst_t *SPI_PORT;
public:
    CustomSPI(uint cs, uint sck, uint, uint miso, uint mosi,  uint baudrate, spi_inst_t *spi_port);
    ~CustomSPI();
};




#endif