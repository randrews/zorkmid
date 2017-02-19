#ifndef SD_H
#define SD_H

#include "spi.h"
#include "pins.h"
#include <avr/io.h>
#include <util/delay.h>

int initSD(Pin cs);
int readSD(byte *array, uint32_t sector, unsigned int offset, unsigned int count);

#endif
