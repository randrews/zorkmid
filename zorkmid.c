#include <avr/io.h>
#include <util/delay.h>

#include "pins.h"
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
    SD_CS_DDR |= (1 << SD_CS_BIT);
    ACT_LED_DDR |= (1 << ACT_LED_BIT);

	usb_init();
    initSPI(&SD_SCK_PORT, &SD_SCK_DDR, SD_SCK_BIT,
            &SD_MISO_PORT, &SD_MISO_DDR, &SD_MISO_PIN, SD_MISO_BIT,
            &SD_MOSI_PORT, &SD_MOSI_DDR, SD_MOSI_BIT);

    printString("ATmega32u4 Initialized\r\n");

    while(1) {
        testFs();
        void usb_serial_flush_output(void);
        for(int n=0; n<3; n++){
            actLed(1);
            _delay_ms(50);
            actLed(0);
            _delay_ms(50);
        }
    }

    return 0;
}

void actLed(int on){
    if(on)
        ACT_LED_PORT |= (1 << ACT_LED_BIT);
    else
        ACT_LED_PORT &= ~(1 << ACT_LED_BIT);
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
