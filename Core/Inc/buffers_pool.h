#ifndef BUFFERS_POOL_H
#define BUFFERS_POOL_H

#include "stdint.h"

typedef struct {
    uint32_t item_size;
    uint32_t max_items;
    uint32_t num_items;
    uint32_t head;
    uint32_t tail;
    uint8_t *base_addr;
} buffer_pool_t;

int32_t bp_reset(buffer_pool_t *pool);
int32_t bp_acuire(buffer_pool_t *pool, uint8_t **buffer);
int32_t bp_release(buffer_pool_t *pool, uint8_t *buffer);




#endif