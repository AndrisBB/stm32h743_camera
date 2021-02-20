#include "main.h"
#include "cmsis_os.h"
#include "stm32h7xx_hal.h"
#include "camera.h"
#include "task_encoder.h"

#include "FreeRTOS.h"
#include <queue.h>
#include <task.h>

#include <stdio.h>
#include <string.h>

#include "video_device.h"
#include "buffer.h"
// #include "buffer_pool.h"


#define CAPTURE_WIDTH           640
#define CAPTURE_HEIGHT          480
#define CAPTURE_BPP             2

#define BUFFER_LINES            8
#define BUFFER_MAX              30
#define BUFFER_SIZE             CAPTURE_WIDTH * CAPTURE_BPP * BUFFER_LINES

extern UART_HandleTypeDef huart2;

void task_init_start(void *argument)
{
	static int32_t buffers_received = 0;

	struct video_device dev;
	struct buffer_pool pool;

	printf("Init task started\r\n");
	
	// Creating video device
	printf("Creating video device\r\n");
	if(video_device_create(&dev) != 0) {
		printf("Failed to create video device, exiting ....\r\n");
		return;
	}
	printf("Created video device OK %p\r\n", &dev);

	// Allocate and enqueue buffers
	printf("Allocate and Enqueue video buffers\r\n");
	for(int i = 0; i < dev.max_vbufs; i++) {

		struct video_buffer *vbuf = video_device_allocate(&dev, BUFFER_SIZE);
		if(vbuf == NULL) {
			printf("Failed to allocate vbuf\r\n");
			Error_Handler();
		}

		if(video_device_enqueue(&dev, vbuf) != pdTRUE) { 
		}
	}
	printf("Enqueue buffers OK\r\n");


	printf("Starting video stream\r\n");
	if(video_device_stream_start(&dev) != 0) {
		printf("Failed to start video stream\r\n");
		Error_Handler();
	}
	printf("Stream started OK\r\n");


	struct video_buffer *vbuf_in = NULL;
	while(1) 
	{
		if(video_device_dequeue(&dev, &vbuf_in, 1000) != 0)
		{
			printf("Failed to dequeue buffer\r\n");
			break;
		}

		buffers_received++;
		printf("Got buffer:%p, received:%d\r\n", vbuf_in->data, buffers_received);

		// HAL_UART_Transmit(&huart2, vbuf_in->data, vbuf_in->length, HAL_MAX_DELAY);

		if(video_device_enqueue(&dev, vbuf_in) != pdTRUE) {
			printf("Failed to enqueue vbuf back\r\n");
			Error_Handler();
		}
	}



	// video_init();
	// video_enqueue(&buffers[0]);
	// video_enqueue(&buffers[1]);




	// raw_buffers = xQueueCreate(64, sizeof(uint8_t *));
	// if(raw_buffers == NULL) {
	// 	printf("Failed to create rew buffer queue\r\n");
	// 	return;
	// }

	// // Start encoder task
	// TaskHandle_t encoder_task_handle = NULL;
	// BaseType_t r = xTaskCreate(
    //                 task_encoder_start,			/* Function that implements the task. */
    //                 "EncoderTask",				/* Text name for the task. */
    //                 256,						/* Stack size in words, not bytes. */
    //                 NULL,						/* Parameter passed into the task. */
    //                 tskIDLE_PRIORITY,			/* Priority at which the task is created. */
    //                 &encoder_task_handle );		/* Used to pass out the created task's handle. */

    // if(r != pdPASS ) {
    //     printf("Failed to create encoder task\r\n");
    //     vTaskDelete( encoder_task_handle );
    // }

	// camera_t *camera;

	// camera = camera_init();
	// camera->on_data = &on_video_buffer;

	// camera_snapshot(camera);

	// osDelay(2500);

	/* Infinite loop */
	for (;;)
	{
		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
		osDelay(1000);
	}
	/* USER CODE END 5 */
}