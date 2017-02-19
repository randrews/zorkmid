#ifndef SD_H
#define SD_H

#include "spi.h"
#include <avr/io.h>
#include <util/delay.h>

int initSD(volatile uint8_t *sd_cs_port_p, volatile uint8_t* sd_cs_ddr_p, volatile uint8_t sd_cs_bit_p);
int readSD(byte *array, uint32_t sector, unsigned int offset, unsigned int count);

#endif
