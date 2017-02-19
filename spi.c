#include "spi.h"
#include "pinutils.h"

Pin miso, mosi, clk;

void initSPI(Pin miso_p, Pin mosi_p, Pin clk_p) {
    miso = miso_p;
    mosi = mosi_p;
    clk = clk_p;

    // Make sure all pins are LOW, for when we make them outputs:
    pinLow(clk);
    pinLow(miso);
    pinLow(mosi);

    // Make MOSI and CLK outputs to set the levels LOW:
    pinOutput(mosi);
    pinOutput(clk);
}

byte spi(byte b) {
    byte result = 0;
    for(int n=0; n<8; n++){
        // Copy bit into MOSI
        if(b & 0x80)
            pinInput(mosi); // input means high
        else
            pinOutput(mosi); // output means low

        // Rising edge of clk
        pinInput(clk); // input means high

        // Copy bit into result
        result <<= 1;
        if(readPin(miso)) result |= 1;

        // Shift b down 1
        b <<= 1;

        // Falling edge of clk
        pinOutput(clk); // output means low
    }
    return result;
}
