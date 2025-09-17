/*
 * oled_driver.h
 *
 *  Created on: 2022年7月24日
 *      Author: Unicorn_Li
 */
#ifndef __OLED_DRIVER_H__
#define __OLED_DRIVER_H__

#include "stdint.h"
#include "u8g2.h"
#include "string.h"
#include "u8x8.h"
#include "ti_msp_dl_config.h"
#include "Board.h"
#include "OLED.h"
// #include "../../include/Board.h"

#define I2C_INST I2C0
#define I2C_TARGET_ADDRESS 0x3C
#define I2C_TX_PACKET_SIZE 2
#define SIZE 1

extern  u8g2_t u8g2;


// 定义毫秒和秒的转换常量
#define MS_PER_SECOND     (1000U)
#define US_PER_MS         (1000U)

// 从系统配置推算，MSPM0G350X 主频应该是 80MHz
#ifndef CPUCLK_FREQ
#define CPUCLK_FREQ       (80000000U)  // 80MHz
#endif

// 定义CPU时钟频率相关的延时周期计算
#define CYCLES_PER_US     (CPUCLK_FREQ / 1000000U)  // 80 cycles per microsecond
#define CYCLES_PER_MS     (CPUCLK_FREQ / 1000U)     // 80000 cycles per millisecond

static void __attribute__((noinline)) busy_wait_cycles(uint32_t cycles)
{
    delay_cycles(cycles);
}

void delay_us(uint32_t us);


// 驱动模式选择
#define OLED_DRIVER_MODE_SPI 1
#define OLED_DRIVER_MODE_I2C 2

#define OLED_DRIVER_MODE OLED_DRIVER_MODE_SPI

#if OLED_DRIVER_MODE == OLED_DRIVER_MODE_SPI

#define OLED_RST_Clr() DL_GPIO_clearPins(SPI_PORT, SPI_RST_PIN)
#define OLED_RST_Set() DL_GPIO_setPins(SPI_PORT,SPI_RST_PIN )
#define OLED_DC_Clr()  DL_GPIO_clearPins(SPI_PORT, SPI_DC_PIN)
#define OLED_DC_Set()  DL_GPIO_setPins(SPI_PORT,SPI_DC_PIN )
#define OLED_CS_Clr()  DL_GPIO_clearPins(SPI_PORT,SPI_CS_PIN )
#define OLED_CS_Set()  DL_GPIO_setPins(SPI_PORT, SPI_CS_PIN)

#elif OLED_DRIVER_MODE == OLED_DRIVER_MODE_I2C

// I2C 模式下使用的 GPIO 定义 (通常与 SPI 复用)
// 你的代码中复用了 SPI 的 PICO 和 SCLK 引脚作为 I2C 的 SDA 和 SCL
#define OLED_I2C_SDA_PORT   SOFT_I2C_PORT
#define OLED_I2C_SDA_PIN    SOFT_I2C_SDA_PIN

#define OLED_I2C_SCL_PORT   SOFT_I2C_PORT
#define OLED_I2C_SCL_PIN    SOFT_I2C_SCL_PIN

#define OLED_I2C_SCL_IOMUX 	SOFT_I2C_SCL_IOMUX
#define OLED_I2C_SDA_IOMUX  SOFT_I2C_SDA_IOMUX

// #define OLED_I2C_RST_PORT PORTB_PORT
// #define OLED_I2C_RST_PIN  PORTB_OLED_RST_PIN

#define OLED_SDA_Clr() DL_GPIO_clearPins(OLED_I2C_SDA_PORT, OLED_I2C_SDA_PIN)
#define OLED_SDA_Set() DL_GPIO_setPins(OLED_I2C_SDA_PORT, OLED_I2C_SDA_PIN)

#define OLED_SCL_Clr() DL_GPIO_clearPins(OLED_I2C_SCL_PORT, OLED_I2C_SCL_PIN)
#define OLED_SCL_Set() DL_GPIO_setPins(OLED_I2C_SCL_PORT, OLED_I2C_SCL_PIN)

// #define OLED_I2C_RST_Clr() DL_GPIO_clearPins(OLED_I2C_RST_PORT, OLED_I2C_RST_PIN)
// #define OLED_I2C_RST_Set() DL_GPIO_setPins(OLED_I2C_RST_PORT, OLED_I2C_RST_PIN)

#endif // OLED_DRIVER_MODE_I2C

void u8g2_Init(void);

extern u8g2_t u8g2;

#endif // __OLED_DRIVER_H__