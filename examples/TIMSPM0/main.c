#include "ti_msp_dl_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "include/Board.h"
#include "oled.h"
// #include "bmp.h"
#include "ui_conf.h"
#include "ui.h"
ui_t *ui;
//
int Wave_TestData;
//
// void UI_Task(void *pvParameters)
// {
//     while(1)
//     {
//         ui_loop(&ui);
//         // vTaskDelay(10);
//     }
// }
//


float sqrtf(float x) {
    if (x <= 0.0f) return 0.0f;
    
    // 处理特殊值
    if (x == 1.0f) return 1.0f;
    
    // 使用更好的初始猜测值
    float guess = x;
    if (x >= 1.0f) {
        // 对于大于等于1的数，使用位操作获得更好的初始猜测
        union { float f; int i; } u = {guess};
        u.i = (1<<29) + (u.i >> 1) - (1<<22); 
        guess = u.f;
    } else {
        // 对于小于1的数，使用x/2作为初始猜测
        guess = x * 0.5f;
    }
    
    // 增加迭代次数到8次以提高精度
    for (int i = 0; i < 8; i++) {
        float new_guess = 0.5f * (guess + x / guess);
        // 添加收敛检查，如果足够接近就提前退出
        if (new_guess == guess) {
            break;
        }
        guess = new_guess;
    }
    
    return guess;
}


int main(void) {
    // 初始化系统配置
    SYSCFG_DL_init();
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);//ÇåÖÐ¶Ï¹ÒÆð
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);//Ê¹ÄÜ´®¿ÚÖÐ¶Ï
    RetargetInit(UART_0_INST);
    printf("Hello\n");
    u8g2_Init();
    // 初始化LED状态为关闭
    MiaoUi_Setup(&ui);
    while(1)
    {
        Wave_TestData = rand() % 600;
        ui_loop(&ui);
        // delay_ms(1);
    }
// int t = 1;
//     while (1)
//     {
//         if(++t >= 32) t = 1;
//         u8g2_ClearBuffer(&u8g2);
//         u8g2_DrawCircle(&u8g2,64,32,t,U8G2_DRAW_ALL);
//         u8g2_DrawCircle(&u8g2,32,32,t,U8G2_DRAW_ALL);
//         u8g2_DrawCircle(&u8g2,96,32,t,U8G2_DRAW_ALL);
//         u8g2_SendBuffer(&u8g2);
//     }


    // xTaskCreate(UI_Task, "UI_Task", 10240, NULL, 1, NULL);
    //
    // vTaskStartScheduler(); // 启动调度器

}


static StaticTask_t xIdleTaskTCB;
static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* 栈溢出时的处理代码 */
    /* 例如，打印错误信息并重置系统 */

    /* 参数:
     * xTask - 发生栈溢出的任务句柄
     * pcTaskName - 发生栈溢出的任务名称
     */

    /* 通常在这里添加一些错误处理和系统恢复的代码 */
    while(1) {

        /* 可以在此处添加错误处理代码，如闪烁LED或输出调试信息 */
    }
}

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                  StackType_t **ppxIdleTaskStackBuffer,
                                  uint32_t *pulIdleTaskStackSize)
{
    /* 传递静态分配的空闲任务数据结构的地址 */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* 传递静态分配的空闲任务堆栈的地址 */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* 传递堆栈大小 */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}


/* 为定时器任务定义静态内存 */
static StaticTask_t xTimerTaskTCB;
static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                   StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize)
{
    /* 传递静态分配的定时器任务数据结构的地址 */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* 传递静态分配的定时器任务堆栈的地址 */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* 传递堆栈大小 */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}