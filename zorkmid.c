#include <avr/io.h>
#include <util/delay.h>

#include "pins.h"
#include "pinutils.h"
#include "usb_serial.h"
#include "spi.h"
#include "pff/pff.h"

void printString(const char myString[]);
void printStringHex(const char myString[], unsigned char c);
void printHex(unsigned char a);
int streq(const char* str1, const char* str2);
void testFs();
void actLed(int on);

int main(){
    pinOutput(ACT_LED);

	usb_init();
    initSPI(SD_MISO, SD_MOSI, SD_CLK);
    printString("ATmega32u4 Initialized\r\n");

    while(1) {
        testFs();
        for(int n=0; n<3; n++){
            actLed(1);
            _delay_ms(100);
            actLed(0);
            _delay_ms(100);
        }
    }

    return 0;
}

void actLed(int on){
    if(on)
        pinHigh(ACT_LED);
    else
        pinLow(ACT_LED);
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
