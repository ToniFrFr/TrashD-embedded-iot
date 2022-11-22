#include "CustomSPI.h"

CustomSPI::CustomSPI(uint cs, uint sck, uint, uint miso, uint mosi, uint baudrate,spi_inst_t *spi_port) : CS(cs), SCK(sck), MISO(miso), MOSI(mosi), BAUDRATE(baudrate) {
    spi_init(spi_port, this->BAUDRATE);
    gpio_set_function(MISO, GPIO_FUNC_SPI);
    gpio_set_function(CS, GPIO_FUNC_SIO);
    gpio_set_function(MOSI, GPIO_FUNC_SPI);
    gpio_set_function(SCK, GPIO_FUNC_SPI);

    gpio_set_dir(CS, GPIO_OUT);
    gpio_put(CS, 1);

}

CustomSPI::~CustomSPI()
{
}