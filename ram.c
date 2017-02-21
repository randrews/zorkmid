#include <util/delay.h>
#include "ram.h"
#include "pinutils.h"

Pin ser, sck, rck, oe, mreq, read, write, data;

void writeAddr(uint16_t addr);
void ramOutput(int on);

void initRam(Pin ser_p, Pin sck_p, Pin rck_p, Pin oe_p){
    ser = ser_p;
    sck = sck_p;
    rck = rck_p;
    oe = oe_p;

    pinOutput(ser);
    pinOutput(sck);
    pinOutput(rck);
    pinOutput(oe);

    pinHigh(oe);
    pinLow(sck);
    pinLow(rck);
}

void initBus(Pin data_p, Pin mreq_p, Pin read_p, Pin write_p){
    data = data_p;
    mreq = mreq_p;
    read = read_p;
    write = write_p;
    releaseBus();
}

void controlBus(){
    pinHigh(mreq);
    pinHigh(read);
    pinHigh(write);

    pinOutput(mreq);
    pinOutput(read);
    pinOutput(write);
}

void releaseBus(){
    pinInput(mreq);
    pinInput(read);
    pinInput(write);
    bankInput(data);

    pinLow(mreq);
    pinLow(read);
    pinLow(write);
    writeBank(data, 0x00);
}

void writeAddr(uint16_t addr){
    for(int n=0; n<16; n++){
        addr & 1 ? pinHigh(ser) : pinLow(ser);
        pinHigh(sck);
        pinLow(sck);
        addr >>= 1;
    }
    pinHigh(rck);
    pinLow(rck);
}

void writeByte(uint16_t addr, uint8_t byte){
    writeAddr(addr);
    bankOutput(data);
    writeBank(data, byte);
    ramOutput(1);
    pinLow(write);
    pinLow(mreq);
    pinHigh(mreq);
    pinHigh(write);
    ramOutput(0);
}

uint8_t readByte(uint16_t addr){
    writeAddr(addr);
    writeBank(data, 0x00);
    bankInput(data);
    ramOutput(1);
    pinLow(read);
    pinLow(mreq);
    uint8_t byte = readBank(data);
    pinHigh(mreq);
    pinHigh(read);
    ramOutput(0);
    return byte;
}

void ramOutput(int on){
    if(on)
        pinLow(oe);
    else
        pinHigh(oe);
}
