#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

typedef enum { B, C, D, E } Bank;

typedef struct Pin {
    Bank bank;
    uint8_t bit;
} Pin;

// To change these, look in pins.c
extern Pin SD_MISO, SD_MOSI, SD_CLK, SD_CS;
extern Pin ACT_LED;

#endif
