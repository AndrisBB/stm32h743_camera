#include "video_device.h"

#include <stdio.h>

#include "ov7670.h"
#include "stm32h7xx_hal_dcmi.h"

#define FIFO_SIZE_MAX           32
#define VIDEO_MEMORY_SIZE       0x80000


#define DMA_BUFFER __attribute__((section(".dma_buffer")))
DMA_BUFFER static uint8_t video_memory[VIDEO_MEMORY_SIZE];

extern I2C_HandleTypeDef hi2c2;
extern DCMI_HandleTypeDef hdcmi;

static void DMA_transfer_completed(DMA_HandleTypeDef *hdma);

static void DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_ErrorEventCallback(DCMI_HandleTypeDef *hdcmi);

uint32_t 
video_device_create(struct video_device *dev)
{
    // Setup image sensor
    ov7670_handle_t sensor;
    sensor.hi2c = &hi2c2;

    if(ov7670_init(&sensor) != OV7670_OK) {
        printf("Failed to initialize camera sensor\r\n");
        return 1;
    }

    if(HAL_DCMI_RegisterCallback(&hdcmi, HAL_DCMI_FRAME_EVENT_CB_ID, DCMI_FrameEventCallback) != HAL_OK) {
        printf("Failed to register HAL_DCMI_FRAME_EVENT_CB_ID callback\r\n");
        return 1;
    }

    if(HAL_DCMI_RegisterCallback(&hdcmi, HAL_DCMI_ERROR_CB_ID, DCMI_ErrorEventCallback) != HAL_OK) {
        printf("Failed to register HAL_DCMI_ERROR_CB_ID callback\r\n");
        return 1;
    }

    if(HAL_DCMI_RegisterCallback(&hdcmi, HAL_DCMI_LINE_EVENT_CB_ID, DCMI_LineEventCallback) != HAL_OK) {
        printf("Failed to register HAL_DCMI_ERROR_CB_ID callback\r\n");
        return 1;
    }

    // Setup video device
    dev->max_vbufs = FIFO_SIZE_MAX;

    dev->_fifo_in = xQueueCreate(FIFO_SIZE_MAX, sizeof(struct video_buffer *));
    if(dev->_fifo_in == NULL) {
        printf("%s: Failed to init FIFO in queue\r\n", __func__);
        return 1;
    }

    dev->_fifo_out = xQueueCreate(FIFO_SIZE_MAX, sizeof(struct video_buffer *));
    if(dev->_fifo_out == NULL) {
        printf("%s: Failed to init FIFO out queue\r\n", __func__);
        return 1;
    }

    dev->mem_base = video_memory;
    dev->mem_ptr = dev->mem_base;
    dev->mem_len = VIDEO_MEMORY_SIZE;

    dev->active_vbufs[0] = NULL;
    dev->active_vbufs[1] = NULL;

    return 0;
}

uint32_t 
video_device_stream_start(struct video_device *dev)
{
    if(xQueueReceive(dev->_fifo_in, &dev->active_vbufs[0], ( TickType_t )0) != pdPASS) {
        printf("%s: Failed to dequeue first vbuf\r\n", __func__);
        return 1;
    }

    printf("%s: Vbuf addr:%p\r\n", __func__, dev->active_vbufs[0]);
    printf("%s: Vbuf data addr:%p\r\n", __func__, dev->active_vbufs[0]->data);

    if(xQueueReceive(dev->_fifo_in, &dev->active_vbufs[1], ( TickType_t )0) != pdPASS) {
        printf("%s: Failed to dequeue second vbuf\r\n", __func__);
        return 1;
    }

    printf("%s: Vbuf addr:%p\r\n", __func__, dev->active_vbufs[1]);
    printf("%s: Vbuf data addr:%p\r\n", __func__, dev->active_vbufs[1]->data);

    // --------------------------------------------------------------
    // Setup DMA
    // --------------------------------------------------------------
    hdcmi.DMA_Handle->XferCpltCallback = DMA_transfer_completed;

    DMA_Stream_TypeDef  *stream = hdcmi.DMA_Handle->Instance;
    stream->CR |= DMA_SxCR_DBM;
    stream->PAR =  (uint32_t)&hdcmi.Instance->DR;
    stream->M0AR = (uint32_t)dev->active_vbufs[0]->data;
    stream->M1AR = (uint32_t)dev->active_vbufs[1]->data;
    stream->NDTR = (uint32_t)dev->active_vbufs[0]->length / 4;
    stream->CR  |= DMA_SxCR_TCIE | DMA_SxCR_TEIE;

    uint32_t *ifcRegister_Base = (uint32_t *)((uint32_t)(hdcmi.DMA_Handle->StreamBaseAddress + 8U));
    *ifcRegister_Base = 0x3FUL << (hdcmi.DMA_Handle->StreamIndex & 0x1FU);

    stream->CR  |= DMA_SxCR_TCIE | DMA_SxCR_TEIE;    // Enable transfer complete
    stream->FCR |= 0x00000080U;
    stream->CR |= DMA_SxCR_EN;      // Start stream

    hdcmi.DMA_Handle->Parent = (void *)dev;

    // --------------------------------------------------------------
    // Setup HDCMI
    // --------------------------------------------------------------
    hdcmi.Instance->CR |= 0x4000;
    hdcmi.Instance->CR |= 0x002;
    // hdcmi.Instance->IER |= 0x1 | DCMI_IT_FRAME | DCMI_IT_VSYNC | DCMI_IT_ERR | DCMI_IT_OVR;
    __HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_LINE | DCMI_IT_VSYNC | DCMI_IT_ERR | DCMI_IT_OVR);

    hdcmi.Instance->CR |= 0x1;

    return 0;
}

struct video_buffer *
video_device_allocate(struct video_device *dev, uint32_t size)
{
    struct video_buffer *ptr = NULL;

    if((dev->mem_ptr + size) >= (dev->mem_base + dev->mem_len)) {
        printf("%s: Error: No more VIDEO_MEM left\r\n");
        return ptr;
    }

    ptr = pvPortMalloc(sizeof(struct video_buffer));
    if(ptr == NULL) {
        printf("%s: Error: No more Kernel memory left\r\n");
        return ptr;
    }

    ptr->data = dev->mem_ptr;
    ptr->length = size;
    ptr->timestamp = 0;
    ptr->used = 0;

    dev->mem_ptr += size;

    return ptr;
}

uint32_t 
video_device_enqueue(struct video_device *dev, struct video_buffer *buf)
{
    return xQueueSend(dev->_fifo_in, ( void * )&buf, (TickType_t) 0);
}

uint32_t 
video_device_dequeue(struct video_device *dev, struct video_buffer **buf, uint32_t timeout)
{
    if(xQueueReceive(dev->_fifo_out, buf, ( TickType_t )timeout) != pdPASS) {
        printf("%s: Failed to dequeue buffer\r\n", __func__);
        return 1;
    }

    return 0;
}

static void DMA_transfer_completed(DMA_HandleTypeDef *hdma)
{
    struct video_device *dev = (struct video_device *)hdma->Parent;
    DMA_Stream_TypeDef  *stream = hdma->Instance;

    if((stream->CR & DMA_SxCR_CT) == 0U) {
        if(dev->active_vbufs[1] == NULL) {
            printf("Data pointer 1 is NULL\r\n");
            return;
        }

        // printf("Done %p\r\n", dev->active_vbufs[1]->data);
        xQueueSendFromISR(dev->_fifo_out, ( void * )&dev->active_vbufs[1], NULL);
        dev->active_vbufs[1] = NULL;

        // Accuire new buffer
        if(xQueueReceiveFromISR(dev->_fifo_in, &dev->active_vbufs[1], NULL) != pdPASS) {
            printf("Failed to dequeue IRQ 1\r\n");
            // stream->CR &= ~DMA_SxCR_EN;
            return;
        }

        stream->M1AR = (uint32_t)dev->active_vbufs[1]->data;
        // printf("Set new ptr1 %p\r\n", dev->active_vbufs[1]->data);
	}
	else {
        if(dev->active_vbufs[0] == NULL) {
            printf("Data pointer 0 is NULL\r\n");
            return;
        }

        // printf("Done %p\r\n", dev->active_vbufs[0]->data);
        xQueueSendFromISR(dev->_fifo_out, ( void * )&dev->active_vbufs[0], NULL);
        dev->active_vbufs[0] = NULL;

        // Accuire new buffer
        if(xQueueReceiveFromISR(dev->_fifo_in, &dev->active_vbufs[0], NULL) != pdPASS) {
            printf("Failed to dequeue IRQ 0\r\n");
            // stream->CR &= ~DMA_SxCR_EN;
            return;
        }

        stream->M0AR = (uint32_t)dev->active_vbufs[0]->data;
        // printf("Set new ptr0 %p\r\n", dev->active_vbufs[0]->data);
	}
}

static void DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    printf("DCMI Vsync\r\n");
}

static void DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    printf("DCMI Frame\r\n");
}

static void DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    // printf("DCMI Line\r\n");
}

static void DCMI_ErrorEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    printf("DCMI Error\r\n");
}