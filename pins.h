#ifndef PINS_H
#define PINS_H

#include <avr/io.h>

typedef enum { B, C, D, E, F } Bank;

typedef struct Pin {
    Bank bank;
    uint8_t bit;
} Pin;

// To change these, look in pins.c
extern Pin SD_MISO, SD_MOSI, SD_CLK, SD_CS;
extern Pin ACT_LED;
extern Pin RAM_SER, RAM_SCK, RAM_RCK, RAM_OE;
extern Pin MREQ, WRITE, READ;
extern Pin Z80RESET, WAIT, IORQ;
extern Pin DATA;
#endif
