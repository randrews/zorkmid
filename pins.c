#include "pins.h"

// SD stuff
Pin SD_MISO = {C, 6},
    SD_MOSI = {E, 2},
    SD_CLK  = {C, 7},
    SD_CS   = {E, 6};

// Activity / boot LED
Pin ACT_LED = {B, 7};

// Shift registers for RAM
Pin RAM_SER = {B, 4},
    RAM_SCK = {B, 5},
    RAM_RCK = {B, 6},
    RAM_OE  = {B, 0};

// Control bus
Pin MREQ    = {F, 1},
    WRITE   = {F, 5},
    READ    = {F, 4},
    Z80RESET= {F, 7},
    WAIT    = {B, 3},
    IORQ    = {F, 0},
    BUSRQ   = {B, 2},
    Z80INT  = {B, 1},
    BUSAK   = {F, 6};

// Data bus: the data bus is actually the entire "D" bank
Pin DATA    = {D, 0};
