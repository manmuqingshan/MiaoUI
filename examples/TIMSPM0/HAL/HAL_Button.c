//
// Created by Administrator on 25-8-18.
//

#include "HAL_Button.h"

uint8_t key_scan[3]={0,0,0};

uint8_t ButtonScan(void)
{
    if (DL_GPIO_readPins(KEY_PORT,KEY_NEXT_PIN) == 0)
    {
        key_scan[0]=1;
        return NEXT;
    }
    if (DL_GPIO_readPins(KEY_PORT,KEY_PREV_PIN) == 0)
    {
        key_scan[1]=1;
        return PREV;
    }
    if (DL_GPIO_readPins(KEY_PORT,KEY_SELECT_PIN) == 0)
    {
        key_scan[2]=1;
        return SELECT;
    }
    return NONE;
}