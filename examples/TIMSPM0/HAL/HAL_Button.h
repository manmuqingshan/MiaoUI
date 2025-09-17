//
// Created by Administrator on 25-8-18.
//

#ifndef HAL_BUTTON_H
#define HAL_BUTTON_H
#include "Board.h"

typedef enum
{
    NONE,
    NEXT,
    PREV,
    SELECT
} BUTTON_EVENT;


uint8_t ButtonScan(void);
#endif //HAL_BUTTON_H
