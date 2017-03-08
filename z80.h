#ifndef Z80_H
#define Z80_H

#include "pins.h"

void z80_init(Pin reset, Pin busrq, Pin busak, Pin wait);
void requestZ80Bus();
void releaseZ80Bus();
void z80_start();

#endif
