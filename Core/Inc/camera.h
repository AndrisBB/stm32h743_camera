#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>
#include "buffer_pool.h"

typedef void (*DataCallback)(uint8_t *data, uint32_t size); 

typedef struct 
{
    DataCallback on_data;
} camera_t;







camera_t *  camera_init();
int32_t     camera_snapshot(camera_t *camera);

#endif