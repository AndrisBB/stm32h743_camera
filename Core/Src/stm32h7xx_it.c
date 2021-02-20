/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32h7xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_dcmi;
extern DCMI_HandleTypeDef hdcmi;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32H7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32h7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */
  typedef struct
  {
      uint32_t ISR; /*!< DMA interrupt status register */
      uint32_t Reserved0;
      uint32_t IFCR; /*!< DMA interrupt flag clear register */
  } DMA_Base_Registers;

  typedef struct
  {
      uint32_t CR;   /*!< DMA stream x configuration register      */
      uint32_t NDTR; /*!< DMA stream x number of data register     */
      uint32_t PAR;  /*!< DMA stream x peripheral address register */
      uint32_t M0AR; /*!< DMA stream x memory 0 address register   */
      uint32_t M1AR; /*!< DMA stream x memory 1 address register   */
      uint32_t FCR;  /*!< DMA stream x FIFO control register       */
  } DMA_Stream_TypeDef;

  uint32_t tmpisr_dma;
  DMA_Base_Registers *regs_dma = (DMA_Base_Registers *)hdma_dcmi.StreamBaseAddress;
  DMA_Stream_TypeDef *stream = hdma_dcmi.Instance;

  tmpisr_dma = regs_dma->ISR;

  /* Transfer Error Interrupt management ***************************************/
  if ((tmpisr_dma & (DMA_FLAG_TEIF0_4 << (hdma_dcmi.StreamIndex & 0x1FU))) != 0U)
  {
      if (__HAL_DMA_GET_IT_SOURCE(&hdma_dcmi, DMA_IT_TE) != 0U)
      {
          /* Disable the transfer error interrupt */
          stream->CR &= ~(DMA_IT_TE);

          /* Clear the transfer error flag */
          regs_dma->IFCR = DMA_FLAG_TEIF0_4 << (hdma_dcmi.StreamIndex & 0x1FU);
      }
  }

  /* Transfer Complete Interrupt management ***********************************/
  if ((tmpisr_dma & (DMA_FLAG_TCIF0_4 << (hdma_dcmi.StreamIndex & 0x1FU))) != 0U)
  {
      if (__HAL_DMA_GET_IT_SOURCE(&hdma_dcmi, DMA_IT_TC) != 0U)
      {
          /* Clear the transfer complete flag */
          regs_dma->IFCR = DMA_FLAG_TCIF0_4 << (hdma_dcmi.StreamIndex & 0x1FU);

          if (((stream->CR) & (uint32_t)(DMA_SxCR_DBM)) != 0U)
          {
              if (hdma_dcmi.XferCpltCallback != NULL)
              {
                /* Transfer complete Callback for memory0 */
                hdma_dcmi.XferCpltCallback(&hdma_dcmi);
              }
          }
      }
  }
  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dcmi);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */
  // printf("DMA IRQ\r\n");

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles DCMI global interrupt.
  */
void DCMI_IRQHandler(void)
{
  /* USER CODE BEGIN DCMI_IRQn 0 */
  // printf("DCMI_IRQHandler\r\n");
  /* USER CODE END DCMI_IRQn 0 */
  HAL_DCMI_IRQHandler(&hdcmi);
  /* USER CODE BEGIN DCMI_IRQn 1 */
  // printf("%lu\r\n", hdcmi.Instance->DR);
  /* USER CODE END DCMI_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
