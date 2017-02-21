#ifndef PINUTILS_H
#define PINUTILS_H

#include "pins.h"

void pinHigh(Pin pin);
void pinLow(Pin pin);
void pinInput(Pin pin);
void pinOutput(Pin pin);
int readPin(Pin pin);

void bankInput(Pin pin);
void bankOutput(Pin pin);
uint8_t readBank(Pin pin);
void writeBank(Pin pin, uint8_t byte);

#endif
