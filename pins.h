#ifndef PINS_H
#define PINS_H

#define ACT_LED_PORT   PORTB
#define ACT_LED_DDR    DDRB
#define ACT_LED_BIT    7

#define SD_MISO_PORT   PORTC
#define SD_MISO_PIN    PINC
#define SD_MISO_DDR    DDRC
#define SD_MISO_BIT    6

#define SD_MOSI_PORT   PORTE
#define SD_MOSI_PIN    PINE
#define SD_MOSI_DDR    DDRE
#define SD_MOSI_BIT    2

#define SD_SCK_PORT    PORTC
#define SD_SCK_PIN     PINC
#define SD_SCK_DDR     DDRC
#define SD_SCK_BIT     7

#define SD_CS_PORT     PORTE
#define SD_CS_PIN      PINE
#define SD_CS_DDR      DDRE
#define SD_CS_BIT      6

#endif
