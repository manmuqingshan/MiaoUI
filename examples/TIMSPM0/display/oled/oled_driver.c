/*
 * oled.c
 *
 *  Created on: 2022年7月24日
 *      Author: Unicorn_Li
 */
#include "oled_driver.h"

void delay_us(uint32_t us)
{
    if (us == 0) return;

    if (us < 10) {
        // 小延时直接用周期数，减少循环开销
        busy_wait_cycles(us * CYCLES_PER_US);
    } else {
        // 大延时用循环，每次1us
        while (us--) {
            busy_wait_cycles(CYCLES_PER_US);
        }
    }
}
// void delay_ms(uint32_t ms) {
//     for(uint32_t i = 0; i < ms; i++) {
//         for(uint32_t j = 0; j < (CPUCLK_FREQ/1000)/3; j++) {
//             __asm__("nop");
//         }
//     }
// }


#include "u8g2.h"

#include "u8g2.h"

uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8,
                         uint8_t msg,
                         uint8_t arg_int,
                         void *arg_ptr)
{
    static uint8_t is_data;   // 记录当前发送类型：0=命令, 1=数据
    uint8_t *data;
    uint8_t packet[2];  // 用于构建I2C数据包

    switch (msg)
    {
    case U8X8_MSG_BYTE_SEND:
        data = (uint8_t *)arg_ptr;
        while (arg_int > 0)
        {
            if (is_data)
            {
                // 展开的 OLED_WR_DATA(*data)
                packet[0] = 0x40;   // 控制字节（表示写数据）
                packet[1] = *data;  // 数据本体

                // 填充FIFO
                DL_I2C_fillControllerTXFIFO(I2C_INST, packet, I2C_TX_PACKET_SIZE);

                // Wait for I2C to be Idle
                while (!(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));

                // 启动传输
                DL_I2C_startControllerTransfer(I2C_INST, I2C_TARGET_ADDRESS,
                    DL_I2C_CONTROLLER_DIRECTION_TX, I2C_TX_PACKET_SIZE);

                // 等待传输完成
                while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
            }
            else
            {
                // 展开的 OLED_WR_CMD(*data)
                packet[0] = 0x00;   // 控制字节（表示写命令）
                packet[1] = *data;  // 命令本体

                // 填充FIFO
                DL_I2C_fillControllerTXFIFO(I2C_INST, packet, I2C_TX_PACKET_SIZE);

                // Wait for I2C to be Idle
                while (!(DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_IDLE));

                // 启动传输
                DL_I2C_startControllerTransfer(I2C_INST, I2C_TARGET_ADDRESS,
                    DL_I2C_CONTROLLER_DIRECTION_TX, I2C_TX_PACKET_SIZE);

                // 等待传输完成
                while (DL_I2C_getControllerStatus(I2C_INST) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS);
            }

            data++;
            arg_int--;
        }
        break;

    case U8X8_MSG_BYTE_INIT:
        // 如果需要，可以初始化 I2C
        break;

    case U8X8_MSG_BYTE_SET_DC:
        is_data = arg_int;  // 0=命令, 1=数据
        break;

    case U8X8_MSG_BYTE_START_TRANSFER:
        // 对于你的裸机函数，不需要清缓冲
        break;

    case U8X8_MSG_BYTE_END_TRANSFER:
        // 直接调用裸机函数即可，不用做额外处理
        break;

    default:
        return 0;
    }

    return 1;
}







uint8_t u8x8_gpio_and_delay_hw(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
    case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
        break;
    case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
        break;
    case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
        delay_ms(1);
        break;
    case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
        break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
        break;                    // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
        break;
    default:
        u8x8_SetGPIOResult(u8x8, 1); // default return value
        break;
    }
    return 1;
}

// u8g2_t u8g2;




// U8g2 初始化函数
void u8g2_Init(void)
{
    // 使用条件编译选择 SPI 或 I2C 驱动

    // 初始化 U8g2，使用 SPI 驱动
    u8g2_Setup_ssd1306_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay_hw);


    // 初始化显示屏（发送初始化命令）
    u8g2_InitDisplay(&u8g2);

    // 打开显示屏
    u8g2_SetPowerSave(&u8g2, 0);

    // 清空显示缓冲区
    u8g2_ClearBuffer(&u8g2);
}
