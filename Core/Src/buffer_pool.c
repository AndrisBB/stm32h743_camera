#include "buffer_pool.h"

#include <stdio.h>
#include <memory.h> 
#include <stdlib.h>



static uint32_t buffer_pool_empty(struct buffer_pool *pool);
static struct buffer *buffer_pool_pop(struct buffer_pool *pool);
// static uint32_t buffer_pool_allocate_node(struct buffer_pool *pool, buffer_node_t **node);

uint32_t buffer_pool_init(struct buffer_pool *pool)
{
    if(pool == NULL) {
        printf("%s: Pool is null\r\n", __func__);
        return 1;
    }

    pool->mem_curr_ptr = pool->mem_base_ptr;
    pool->buf_node_head = NULL;
    pool->items_curr = 0;

    return 0;
}

struct buffer *buffer_pool_aquire(struct buffer_pool *pool)
{
    printf("%s: called\r\n", __func__);

    struct buffer *ptr = NULL;

    // If there is a prealocated buffer available, then return that
    if(!buffer_pool_empty(pool)) {
        // Pop buffer and return
        printf("%s: Buffers  not empty, pop first item\r\n", __func__);

        ptr = buffer_pool_pop(pool);
        if(ptr == NULL) {
            printf("%s: Not great, Popped buffer is NULL\r\n", __func__);
        }
    }
    else {
        printf("%s: No buffers available, allocate new one\r\n", __func__);

        // Check if we not exceeded max size
        if(pool->items_curr >= pool->items_max) {
            printf("%s: Reached max size of pool\r\n", __func__);
        }
        else {
            ptr = malloc(sizeof(struct buffer));
            ptr->data = pool->mem_curr_ptr;
            ptr->length = pool->item_size;
            ptr->timestamp = 0;
            ptr->used = 0;

            pool->mem_curr_ptr = pool->mem_curr_ptr + pool->item_size;
        }
    }

    return ptr;
}

uint32_t buffer_pool_empty(struct buffer_pool *pool)
{
    if(pool->buf_node_head == NULL) {
        return 1;
    }

    return 0;
}

struct buffer *buffer_pool_pop(struct buffer_pool *pool)
{
    buffer_node_t *next = NULL;
    struct buffer *ptr = NULL;

    if(pool->buf_node_head == NULL) {
        return NULL;
    }

    next = pool->buf_node_head->next;

    ptr = pool->buf_node_head->buf;
    free(pool->buf_node_head);

    pool->buf_node_head = next;

    return ptr; 
}