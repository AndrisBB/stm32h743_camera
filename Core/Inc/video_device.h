#ifndef VIDEO_DEVICE_H
#define VIDEO_DEVICE_H

#include <stdint.h>

#include "FreeRTOS.h"
#include "queue.h"

struct video_buffer
{
    uint8_t *data;
    uint32_t length;
    uint32_t used;
    uint32_t timestamp;
};

struct video_device
{
    QueueHandle_t   _fifo_in;           // FIFO for empty video buffers
    QueueHandle_t   _fifo_out;          // FIFO for filled video buffers

    uint32_t        max_vbufs;

    uint8_t         *mem_base;
    uint8_t         *mem_ptr;
    uint32_t        mem_len;

    struct video_buffer* active_vbufs[2];
};

uint32_t 
video_device_create(struct video_device *dev);

uint32_t
video_device_stream_start(struct video_device *dev);

struct video_buffer *
video_device_allocate(struct video_device *dev, uint32_t size);

uint32_t 
video_device_enqueue(struct video_device *dev, struct video_buffer *buf);

uint32_t 
video_device_dequeue(struct video_device *dev, struct video_buffer **buf, uint32_t timeout);




#endif