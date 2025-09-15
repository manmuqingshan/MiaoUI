#include "../../include/Board.h"


void delay_ms(uint32_t ms) {
    for(uint32_t i = 0; i < ms; i++) {
        for(uint32_t j = 0; j < (CPUCLK_FREQ/1000)/3; j++) {
            __asm__("nop");
        }
    }
}
