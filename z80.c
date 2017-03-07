#include "z80.h"
#include "pinutils.h"

Pin reset, busrq, busak, wait;
uint8_t bus_held;

void z80_init(Pin reset_p, Pin busrq_p, Pin busak_p, Pin wait_p) {
    reset = reset_p;
    busrq = busrq_p;
    busak = busak_p;
    wait = wait_p;

    pinInput(busak);
    pinOutput(reset);
    pinOutput(busrq);
    pinOutput(wait);

    pinHigh(busrq);
    pinHigh(reset);
    pinHigh(wait);
}

void requestZ80Bus() {
    pinLow(busrq);
    while(readPin(busak));
}

void releaseZ80Bus(){
    pinHigh(busrq);
}
