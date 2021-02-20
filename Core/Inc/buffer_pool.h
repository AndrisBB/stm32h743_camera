#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include <stdint.h>
#include <buffer.h>

typedef struct buffer_node 
{
    struct buffer           *buf;
    struct buffer_node      *next;
} buffer_node_t;

struct buffer_pool 
{
    uint32_t items_max;
    uint32_t items_curr;
    uint32_t item_size;

    buffer_node_t *buf_node_head;

    uint8_t *mem_base_ptr;
    uint8_t *mem_curr_ptr;
    uint32_t mem_size;
};


uint32_t buffer_pool_init(struct buffer_pool *pool);
struct buffer *buffer_pool_aquire(struct buffer_pool *pool);

#endif