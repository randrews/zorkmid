#include "sd.h"
#include "pinutils.h"

Pin cs;

void sd_select(byte selected){
    if(!selected) pinOutput(cs); // Make it output, thus 0
    else          pinInput(cs); // Make it input, thus 1
}

// Returns 1 for card present
byte cmd0(){
    sd_select(0);

    spi(0x40); // cmd0
    spi(0x00); spi(0x00); spi(0x00); spi(0x00);
    spi(0x95); // CRC
    spi(0xFF);

    byte i; byte count=255;
    do { i = spi(0xFF); } while(i & 0x80 && count--);

    sd_select(1);

    return i == 1;
}

byte cmd8(){
    sd_select(0);

    spi(0x48);
    spi(0x00); spi(0x00); spi(0x01); spi(0xAA);
    spi(0x87);
    spi(0xFF);

    byte response; byte count=255;
    do { response = spi(0xFF); } while(response & 0x80 && count--);

    spi(0xFF); spi(0xFF); // Throw away two bytes
    byte r1 = spi(0xFF);
    byte r2 = spi(0xFF);

    sd_select(1);

    return r1 == 0x01 && r2 == 0xAA;
}

byte cmd1(){
    byte try_count = 10; // Number of times we'll try this mess

 start:
    // Let's pause for a minute
    _delay_ms(10);

    // CMD55
    sd_select(0);
    spi(0xFF);
    spi(0x77);
    spi(0x00); spi(0x00); spi(0x00); spi(0x00);
    spi(0xFF);

    spi(0xFF); spi(0xFF);
    sd_select(1);

    // Send cmd 1
    sd_select(0);
    spi(0xFF);
    spi(0x69); // cmd1
    spi(0x40); spi(0x00); spi(0x00); spi(0x00); // Note the first byte of the arg is 40h
    spi(0xFF); // CRC

    byte i = 0xFF;
    byte count = 100;
    while(1){
        for(int n=0; n < 8; n++){
            byte b = spi(0xFF);
            if(b != 0xFF) i = b;
        }
        if(i != 0xFF) break;
        if(!(count--)) break;
    }

    sd_select(1);

    // Forgive me. I can't recurse because I don't want to waste the stack.
    if(i == 0x01 && try_count--) goto start; // 1 means still init-ing

    // If it's 1, we've retried.
    // If it's 0, then it's ready to rock.
    // Anything else, it won't init.
    return i == 0x00;
}

byte cmd50(){
    sd_select(0);
    spi(0x50 | 0x40);
    spi(0x00); spi(0x00); spi(0x02); spi(0x00);
    spi(0xFF);
    spi(0xFF);

    byte r; byte count=255;
    do { r = spi(0xFF); } while(r & 0x80 && count--);

    sd_select(1);

    return r == 0x00;
}

// Returns nonzero on success
int initSD(Pin cs_p) {
    cs = cs_p;
    pinLow(cs);

    _delay_ms(10); // Ensure it's had time to power up

    // Send some clock pulses to let it awaken
    sd_select(1);
    for(int k=0; k < 10; k++) spi(0xFF);

    if(! cmd0() ) return 0; // No card found
    if(! cmd8() ) return 0; // Not even sure what can go wrong with this
    if(! cmd1() ) return 0; // Card won't init
    if(! cmd50()) return 0; // Can't set the size?

    return 1;
}

// array is an array of count bytes
// sector is the sector number (a sector is 512 bytes)
// offset is a byte address within the sector (0 .. 511)
// count is the number of bytes to read
// (0 .. 512, and count+offset is at most 512)
// Like everything else, this returns nonzero on success
int readSD(byte *array, uint32_t sector, unsigned int offset, unsigned int count){
    if(count == 0) return 1;
    if(offset >= 512 || offset + count > 512) return 0;

    // Okay, now that that's out of the way, let's actually talk to the card
    sd_select(0);

    spi(0x51 | 0x40);
    spi(sector >> 24);
    spi(sector >> 16);
    spi(sector >> 8);
    spi(sector);
    spi(0xFF);

    // Loop until we see an FE
    byte tries = 100;
    byte response;
    while((response = spi(0xFF)) != 0xFE
          && tries--) _delay_ms(1);
    if(response != 0xFE) return 0;

    int array_index = 0;
    for(int n=0; n < 512; n++){
        byte c = spi(0xFF); // Grab the byte
        if(n >= offset && array_index < count){
            array[array_index++] = c;
        }
    }

    spi(0xFF); spi(0xFF); // Consume a CRC
    sd_select(1);
    spi(0xFF); // Command separator?

    return 1;
}
