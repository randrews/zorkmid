#include "pinutils.h"

volatile uint8_t *pinDdr(Pin pin);
volatile uint8_t *pinPort(Pin pin);
volatile uint8_t *pinPin(Pin pin);

void pinHigh(Pin pin){
    volatile uint8_t *port = pinPort(pin);
    *port |= (1 << pin.bit);
}

void pinLow(Pin pin){
    volatile uint8_t *port = pinPort(pin);
    *port &= ~(1 << pin.bit);
}

void pinInput(Pin pin){
    volatile uint8_t *ddr = pinDdr(pin);
    *ddr &= ~(1 << pin.bit);
}

void pinOutput(Pin pin){
    volatile uint8_t *ddr = pinDdr(pin);
    *ddr |= (1 << pin.bit);
}

int readPin(Pin pin){
    if(*(pinPin(pin)) & (1 << pin.bit))
        return 1;
    else
        return 0;
}

volatile uint8_t *pinDdr(Pin pin){
    switch(pin.bank){
    case B: return &DDRB;
    case C: return &DDRC;
    case D: return &DDRD;
    case E: return &DDRE;
    }
}

volatile uint8_t *pinPort(Pin pin){
    switch(pin.bank){
    case B: return &PORTB;
    case C: return &PORTC;
    case D: return &PORTD;
    case E: return &PORTE;
    }
}

volatile uint8_t *pinPin(Pin pin){
    switch(pin.bank){
    case B: return &PINB;
    case C: return &PINC;
    case D: return &PIND;
    case E: return &PINE;
    }
}
