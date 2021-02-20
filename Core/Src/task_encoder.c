#include "task_encoder.h"
#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "buffer_pool.h"

#include <stdio.h>

extern QueueHandle_t raw_buffers;
extern UART_HandleTypeDef huart2;

// extern buffer_pool_t pool;

void task_encoder_start(void *args)
{
    static uint8_t cnt = 0;

    printf("Encoder task started\r\n");

    uint8_t *ptr;

    while(1) 
    {
        if(xQueueReceive(raw_buffers, &ptr, portMAX_DELAY) == pdPASS)
        {
            printf("Received ptr:%p, %u\r\n", ptr, cnt);
            cnt++;
            if(HAL_UART_Transmit(&huart2, ptr,  640 * 8 * 2, HAL_MAX_DELAY) != HAL_OK)
            {
                printf("Failed to dump frame 0\r\n");
            }

            // bp_release(&pool, ptr);
        }
    }
}