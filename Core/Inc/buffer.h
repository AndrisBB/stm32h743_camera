#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h> 

struct buffer
{
    uint8_t *data;
    uint32_t length;
    uint32_t used;
    uint32_t timestamp;
};


#endif