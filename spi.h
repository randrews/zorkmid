#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <util/delay.h>
#include "../pins.h"

typedef unsigned char byte;

void initSPI(volatile uint8_t *clk_port_p, volatile uint8_t* clk_ddr_p, volatile uint8_t clk_bit_p,
             volatile uint8_t *miso_port_p, volatile uint8_t* miso_ddr_p, volatile uint8_t* miso_pin_p, volatile uint8_t miso_bit_p,
             volatile uint8_t *mosi_port_p, volatile uint8_t* mosi_ddr_p, volatile uint8_t mosi_bit_p);
byte spi(byte b);

#endif
