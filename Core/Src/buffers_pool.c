#include "buffers_pool.h"
#include "stdio.h"


int32_t bp_reset(buffer_pool_t *pool)
{
    printf("Initialize buffer pool\r\n");

    pool->head = 0;
    pool->tail = 0;
    pool->num_items = 0;

    printf("Base address:%p\r\n", pool->base_addr);
    printf("Buffer size:%lu\r\n", pool->item_size);
    printf("Buffer max:%lu\r\n", pool->max_items);

    return 0;
}

int32_t bp_acuire(buffer_pool_t *pool, uint8_t **buffer)
{
    printf("bp_acuire called, size:%lu, head:%lu, tail:%lu\r\n", pool->num_items, pool->head, pool->tail);

    if(pool->num_items >= pool->max_items) {
        printf("Buffer is full\r\n");
        return -1;
    }

    *buffer = pool->base_addr + (pool->item_size * pool->head);
    // printf("Return:%lu, %p\r\n", pool->head, *buffer);
    
    pool->head = (pool->head + 1) % pool->max_items;
    pool->num_items++;
    
    printf("bp_acuire completed, size:%lu, head:%lu, tail:%lu\r\n", pool->num_items, pool->head, pool->tail);

    return 0;
}

int32_t bp_release(buffer_pool_t *pool, uint8_t *buffer)
{
    printf("bp_release called, size:%lu, head:%lu, tail:%lu\r\n", pool->num_items, pool->head, pool->tail);

    uint8_t *tail_addr;

    tail_addr = pool->base_addr + (pool->item_size * pool->tail);
    printf("Release tail:%p, buffer:%p\r\n", tail_addr, buffer);

    if(tail_addr == buffer) {
        pool->tail = (pool->tail + 1) % pool->max_items;
        if(pool->num_items > 0) {
            pool->num_items--;
        }
    }

    printf("bp_release completed, size:%lu, head:%lu, tail:%lu\r\n", pool->num_items, pool->head, pool->tail);

    return 0;
}