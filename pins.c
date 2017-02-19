#include "pins.h"

// SD stuff
Pin SD_MISO = {C, 6},
    SD_MOSI = {E, 2},
    SD_CLK  = {C, 7},
    SD_CS   = {E, 6};

// Activity / boot LED
Pin ACT_LED = {B, 7};
