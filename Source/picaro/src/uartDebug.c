
#include "uartDebug.h"

#include <stm32f1xx_hal.h>

static UART_HandleTypeDef debugUart;
static DMA_HandleTypeDef  debugUartDma;

void debug_init()
{
    __HAL_RCC_USART3_CLK_ENABLE();

    debugUart.Instance          = USART3;
    debugUart.Init.BaudRate     = 115200;
    debugUart.Init.WordLength   = UART_WORDLENGTH_8B;
    debugUart.Init.StopBits     = UART_STOPBITS_1;
    debugUart.Init.Parity       = UART_PARITY_NONE;
    debugUart.Init.Mode         = UART_MODE_TX_RX;
    debugUart.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    debugUart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&debugUart);
}

void debug_send(uint8_t* string)
{
    HAL_UART_Transmit(&debugUart, string, strlen(string), 1000);
}
