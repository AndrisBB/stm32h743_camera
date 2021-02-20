#include "camera.h"
#include "ov7670.h"
#include "stm32h7xx_hal_dcmi.h"

#include <stdint.h>
#include <stdio.h>

#define WIDTH               640
#define HEIGHT              240
#define BYTES_PER_PIXEL     2
#define LINES_PER_BUFFER    8

#define DMA_BUFFER __attribute__((section(".dma_buffer")))

extern I2C_HandleTypeDef hi2c2;
extern DCMI_HandleTypeDef hdcmi;

DMA_BUFFER uint8_t fb_memory[WIDTH * HEIGHT * BYTES_PER_PIXEL];
// buffer_pool_t pool;
static camera_t camera;

static void DMA_transfer_completed(DMA_HandleTypeDef *hdma);

static void DCMI_VsyncEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_LineEventCallback(DCMI_HandleTypeDef *hdcmi);
static void DCMI_ErrorEventCallback(DCMI_HandleTypeDef *hdcmi);

camera_t * camera_init()
{
	ov7670_handle_t sensor;
	sensor.hi2c = &hi2c2;

	if(ov7670_init(&sensor) != OV7670_OK) {
		printf("Failed to initialize camera sensor\r\n");
		return -1;
	}

	if(HAL_DCMI_RegisterCallback(&hdcmi, HAL_DCMI_FRAME_EVENT_CB_ID, DCMI_FrameEventCallback) != HAL_OK) {
		printf("Failed to register HAL_DCMI_FRAME_EVENT_CB_ID callback\r\n");
		return -1;
	}

	if(HAL_DCMI_RegisterCallback(&hdcmi, HAL_DCMI_ERROR_CB_ID, DCMI_ErrorEventCallback) != HAL_OK) {
		printf("Failed to register HAL_DCMI_ERROR_CB_ID callback\r\n");
	}

	// pool.base_addr = fb_memory;
	// pool.item_size = (WIDTH * LINES_PER_BUFFER * BYTES_PER_PIXEL); 
	// pool.max_items = HEIGHT / LINES_PER_BUFFER;

	// bp_reset(&pool);

	camera.on_data = NULL;

	return &camera;
}

int32_t	camera_snapshot(camera_t *camera)
{
	printf("%s: called\r\n", __func__);

	// bp_reset(&pool);

	uint8_t *ptr_0, *ptr_1;
	// if(bp_acuire(&pool, &ptr_0) != 0) {
	// 	printf("Failed to allocato buffer 0\r\n");
	// 	return -1;
	// }

	// if(bp_acuire(&pool, &ptr_1) != 0) {
	// 	printf("Failed to allocato buffer 1\r\n");
	// 	return -1;
	// }


	// --------------------------------------------------------------
	// Setup DMA
	// --------------------------------------------------------------
	hdcmi.DMA_Handle->XferCpltCallback = DMA_transfer_completed;

	DMA_Stream_TypeDef  *stream = hdcmi.DMA_Handle->Instance;
	stream->CR |= DMA_SxCR_DBM;
	stream->PAR =  (uint32_t)&hdcmi.Instance->DR;
	stream->M0AR = (uint32_t)ptr_0;
	stream->M1AR = (uint32_t)ptr_1;
	// stream->NDTR = (uint32_t)pool.item_size / 4;
	stream->CR  |= DMA_SxCR_TCIE | DMA_SxCR_TEIE;

	uint32_t *ifcRegister_Base = (uint32_t *)((uint32_t)(hdcmi.DMA_Handle->StreamBaseAddress + 8U));
	*ifcRegister_Base = 0x3FUL << (hdcmi.DMA_Handle->StreamIndex & 0x1FU);

	stream->CR  |= DMA_SxCR_TCIE | DMA_SxCR_TEIE;    // Enable transfer complete
	stream->FCR |= 0x00000080U;
	stream->CR |= DMA_SxCR_EN;      // Start stream
	
	// --------------------------------------------------------------
	// Setup HDCMI
	// --------------------------------------------------------------
	hdcmi.Instance->CR |= 0x4000;
	hdcmi.Instance->CR |= 0x002;
	hdcmi.Instance->IER |= 0x1;

	hdcmi.Instance->CR |= 0x1;

	return 0;
}

static void DMA_transfer_completed(DMA_HandleTypeDef *hdma)
{
	uint8_t *buffer, *prev_buffer;
	DMA_Stream_TypeDef  *stream = hdma->Instance;

	// if(bp_acuire(&pool, &buffer) != 0) {
	// 	printf("Failed to acuire buffer, stop DMA\r\n");
	// 	stream->CR &= ~DMA_SxCR_EN;
	// 	return;
	// }

	// printf("buffer address:%p\r\n", buffer);
	if((stream->CR & DMA_SxCR_CT) == 0U) {
		// printf("Memory 1 comlpleted\r\n");
		prev_buffer = (uint8_t *)stream->M1AR;
		stream->M1AR = (uint32_t)buffer;
	}
	else {
		// printf("Memory 0 comlpleted\r\n");
		prev_buffer = (uint8_t *)stream->M0AR;
		stream->M0AR = (uint32_t)buffer;
	}

	if(camera.on_data != NULL) {
		camera.on_data(prev_buffer, (WIDTH * LINES_PER_BUFFER * BYTES_PER_PIXEL));
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
    printf("DCMI Line\r\n");
}

static void DCMI_ErrorEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    printf("DCMI Error\r\n");
}