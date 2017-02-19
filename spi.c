#include "spi.h"

volatile uint8_t *clk_port, *clk_ddr, clk_bit;
volatile uint8_t *miso_port, *miso_pin, *miso_ddr, miso_bit;
volatile uint8_t *mosi_port, *mosi_ddr, mosi_bit;
    
void initSPI(volatile uint8_t *clk_port_p, volatile uint8_t* clk_ddr_p, volatile uint8_t clk_bit_p,
             volatile uint8_t *miso_port_p, volatile uint8_t* miso_ddr_p, volatile uint8_t* miso_pin_p, volatile uint8_t miso_bit_p,
             volatile uint8_t *mosi_port_p, volatile uint8_t* mosi_ddr_p, volatile uint8_t mosi_bit_p) {
    clk_port = clk_port_p;
    clk_ddr = clk_ddr_p;
    clk_bit = clk_bit_p;
    miso_port = miso_port_p;
    miso_ddr = miso_ddr_p;
    miso_pin = miso_pin_p;
    miso_bit = miso_bit_p;
    mosi_port = mosi_port_p;
    mosi_ddr = mosi_ddr_p;
    mosi_bit = mosi_bit_p;

    // Make sure all pins are LOW, for when we make them outputs:
    *clk_port &= ~(1 << clk_bit);
    *mosi_port &= ~(1 << mosi_bit);
    *miso_port &= ~(1 << miso_bit); // External pullup on MISO to ensure it's pulled to 3v

    // Make MOSI and CLK outputs to set the levels LOW:
    *mosi_ddr |= (1 << mosi_bit);
    *clk_ddr |= (1 << clk_bit);
}

byte spi(byte b) {
    byte result = 0;
    for(int n=0; n<8; n++){
        // Copy bit into MOSI
        if(b & 0x80)
            *mosi_ddr &= ~(1 << mosi_bit); // 0 means input means high
        else
            *mosi_ddr |= (1 << mosi_bit); // 1 means output means low

        // Rising edge of clk
        *clk_ddr &= ~(1 << clk_bit); // 0 means input means high

        // Copy bit into result
        result <<= 1;
        if(*miso_pin & (1 << miso_bit)) result |= 1;

        // Shift b down 1
        b <<= 1;

        // Falling edge of clk
        *clk_ddr |= (1 << clk_bit); // 1 means output means low
    }
    return result;
}
