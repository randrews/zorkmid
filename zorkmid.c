#include <avr/io.h>
#include <util/delay.h>

#include "pins.h"
#include "pinutils.h"
#include "usb_serial.h"
#include "spi.h"
#include "ram.h"
#include "z80.h"
#include "pff/pff.h"

void printString(const char myString[]);
void printStringHex(const char myString[], unsigned char c);
void printHex(unsigned char a);
int streq(const char* str1, const char* str2);
void testFs();
void testRam();
void actLed(int on);
int loadFile(const char fname[]);

int main(){
    pinOutput(ACT_LED);

	usb_init();
    z80_init(Z80RESET, BUSRQ, BUSAK, WAIT);
    initSPI(SD_MISO, SD_MOSI, SD_CLK);
    initRam(RAM_SER, RAM_SCK, RAM_RCK, RAM_OE);
    initBus(DATA, MREQ, READ, WRITE);

    printString("ATmega32u4 Initialized\r\n");

    while(loadFile("BOOT.OBJ"));

    z80_start();
    actLed(1);

    return 0;
}

int loadFile(const char fname[]){
    FATFS fs;
    DIR dir;
    FILINFO file;
    FRESULT res;
    uint16_t addr = 0x0000;
    byte buf[512]; UINT count;

    requestZ80Bus();
    controlBus();
    pf_mount(&fs);
    res = pf_opendir(&dir, "");

    while(1){
        FRESULT read = pf_readdir(&dir, &file);
        if(file.fname[0] == 0) break;
    }

    res = pf_open(fname);

    if(!res){
        for(int n=0; n<3; n++){
            actLed(1);
            _delay_ms(100);
            actLed(0);
            _delay_ms(100);
        }
    } else {
        return 1;
    }

    while(1){
        res = pf_read(buf, 512, &count);
        if(res || !count) break;

        for(int n=0; n<count; n++)
            writeByte(addr++, buf[n]);

        actLed(1);
        _delay_ms(50);
        actLed(0);
        _delay_ms(50);
    }

    releaseBus();
    releaseZ80Bus();
    return 0;
}

void actLed(int on){
    if(on)
        pinHigh(ACT_LED);
    else
        pinLow(ACT_LED);
}

void testRam(){
    requestZ80Bus();
    controlBus();
    uint8_t counter;
    printStringHex("Reading counter byte 0xbeef: ", counter = readByte(0xbeef));
    printStringHex("Writing byte to 0x12f1: ", 'a');
    writeByte(0x12f1, 'a');
    printStringHex("Writing byte to 0xf20f: ", 'z');
    writeByte(0xf20f, 'z');
    counter += 16;
    printStringHex("Writing counter byte 0xbeef: ", counter);
    writeByte(0xbeef, counter);
    printStringHex("Reading byte from 0x12f1: ", readByte(0x12f1));
    printStringHex("Reading byte from 0xf20f: ", readByte(0xf20f));
    printString("--------------------\r\n");
    releaseBus();
    releaseZ80Bus();
}

void testFs(){
    FATFS fs;
    DIR dir;
    FILINFO file;
    FRESULT res;

    printStringHex("Mount: ", pf_mount(&fs));
    res = pf_opendir(&dir, "");
    printStringHex("opendir: ", res);

    int found_foo = 0;
    while(!res){
        FRESULT read = pf_readdir(&dir, &file);
        if(file.fname[0] == 0) break;
        printString("File: ");
        printString(file.fname);
        printString("\r\n");
        if(!found_foo) found_foo = streq(file.fname, "FOO.TXT");
    }

    printStringHex("Found foo: ", found_foo);

    res = pf_open("FOO.TXT");
    printStringHex("Open file: ", res);

    printString("File contents: ");

    byte buf[16]; UINT count;
    while(1){
        res = pf_read(buf, 16, &count);
        if(res || !count) break;

        for(int n=0; n<count; n++)
            usb_serial_putchar(buf[n]);
    }

    printString("\r\n");
}

int streq(const char* str1, const char* str2){
    while(1){
        if(*str1 != *str2) return 0;
        if(!*str1) return 1;
        str1++; str2++;
    }
}

void printString(const char myString[]) {
    uint8_t i = 0;
    while (myString[i]) i++;
    usb_serial_write(myString, i);
}

void printStringHex(const char myString[], unsigned char c) {
    printString(myString);
    printHex(c);
    printString("\r\n");
}

void printHex(unsigned char a){
    int n1 = (a >> 4);
    int n2 = (a & 0x0f);

    if(n1 >= 10) usb_serial_putchar('A' + n1 - 10);
    else usb_serial_putchar('0' + n1);

    if(n2 >= 10) usb_serial_putchar('A' + n2 - 10);
    else usb_serial_putchar('0' + n2);
}
