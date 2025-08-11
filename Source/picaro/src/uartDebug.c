
#include "uartDebug.h"

#include <stm32f1xx_hal.h>

static UART_HandleTypeDef debugUart;
static DMA_HandleTypeDef debugUartDma;

void debug_init()
{

    __HAL_RCC_USART3_CLK_ENABLE();

    debugUart.Instance = USART3;
    debugUart.Init.BaudRate = 115200;
    debugUart.Init.WordLength = UART_WORDLENGTH_8B;
    debugUart.Init.StopBits = UART_STOPBITS_1;
    debugUart.Init.Parity = UART_PARITY_NONE;
    debugUart.Init.Mode = UART_MODE_TX_RX;
    debugUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    debugUart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init( &debugUart);
/*
    debugUartDma.Instance = DMA1_Channel2;
    debugUartDma.Init.Direction = DMA_MEMORY_TO_PERIPH;
    debugUartDma.Init.PeriphInc = DMA_PINC_DISABLE;
    debugUartDma.Init.MemInc = DMA_MINC_ENABLE;
    debugUartDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    debugUartDma.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    debugUartDma.Init.Mode = DMA_NORMAL;
    debugUartDma.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&debugUartDma);

    __HAL_LINKDMA(&debugUart, hdmatx, debugUartDma);*/
   
}

void debug_send(uint8_t *string)
{
    //HAL_UART_Transmit_DMA(&debugUart, string, strlen(string));
    HAL_UART_Transmit(&debugUart, string, strlen(string), 1000);
}


void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */

  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&debugUartDma);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}
