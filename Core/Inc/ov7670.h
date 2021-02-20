#ifndef OV7670_H
#define OV7670_H

#include "stm32h7xx_hal.h"

typedef struct 
{
    I2C_HandleTypeDef *hi2c;
} ov7670_handle_t;

typedef enum
{
    OV7670_OK       = 0x00,
    OV7670_ERROR    = 0x01,
} OV7670_status_t;


OV7670_status_t     ov7670_init(ov7670_handle_t *handle);
void                ov7670_dump_registers(ov7670_handle_t *handle);

#endif
