#ifndef RAM_H
#define RAM_H

#include "pins.h"

void initRam(Pin ser, Pin sck, Pin rck, Pin oe);
void initBus(Pin data, Pin mreq, Pin read, Pin write);
void controlBus();
void releaseBus();
void writeByte(uint16_t addr, uint8_t byte);
uint8_t readByte(uint16_t addr);

#endif
