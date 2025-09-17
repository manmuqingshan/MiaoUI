//
// Created by Administrator on 25-8-19.
//

#ifndef HAL_SPI_H
#define HAL_SPI_H
#include "ti_msp_dl_config.h"

uint8_t spi_read_write_byte(SPI_Regs *spi_inst, uint8_t byte);
#endif //HAL_SPI_H
