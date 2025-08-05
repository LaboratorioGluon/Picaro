#include "gps.h"
#include <stm32f1xx_hal.h>
#include <string.h>

#include "nmea.h"

#define RAW_DATA_BUFFER_SIZE 256
#define READ_DATA_CHUNK_SIZE 90

static UART_HandleTypeDef gpsUart;
static uint8_t rawDataBuffer[RAW_DATA_BUFFER_SIZE]="";
static uint32_t rawDataBufferCurrent = 0;


void gps_parseNMEA(uint8_t *nmea);

void gps_init()
{
    __HAL_RCC_USART1_CLK_ENABLE();

    gpsUart.Instance = USART1;
    gpsUart.Init.BaudRate = 9600;
    gpsUart.Init.WordLength = UART_WORDLENGTH_8B;
    gpsUart.Init.StopBits = UART_STOPBITS_1;
    gpsUart.Init.Parity = UART_PARITY_NONE;
    gpsUart.Init.Mode = UART_MODE_TX_RX;
    gpsUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    gpsUart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&gpsUart);

}


void gps_readData()
{
    
    // If we arrive here, houston we have a problem, discard previous data.
    if ((rawDataBufferCurrent+READ_DATA_CHUNK_SIZE) >= RAW_DATA_BUFFER_SIZE)
    {
        rawDataBufferCurrent = 0;
    }

    HAL_StatusTypeDef ret = HAL_UART_Receive(&gpsUart, &rawDataBuffer[rawDataBufferCurrent], READ_DATA_CHUNK_SIZE, 1000);
    if (ret == HAL_OK)
    {
        rawDataBufferCurrent += READ_DATA_CHUNK_SIZE;
    }


    //strchr(rawDataBuffer, '\n');

    uint32_t dFound=0, eFound=0, lasteFound = 0;
    for( uint32_t i = 0; i < rawDataBufferCurrent; i++)
    {
        if (rawDataBuffer[i] == '$')
        {
            dFound = i;
        }
        if (rawDataBuffer[i] == '\r')
        {
            eFound = i;
            lasteFound = eFound;
            if (dFound)
            {
                rawDataBuffer[eFound] = 0;

                gps_parseNMEA((char*)&rawDataBuffer[dFound]);

                dFound = 0;
                eFound = 0;
            }
        }
    }

    if (lasteFound)
    {
        rawDataBufferCurrent = rawDataBufferCurrent - lasteFound;
        for( uint32_t i = 0; i < rawDataBufferCurrent; i++)
        {
            rawDataBuffer[i] = rawDataBuffer[lasteFound + i];
        }
    }
    
}



void gps_parseNMEA(uint8_t *nmea)
{
    
    NMEA_Message_t nmeaMessage;
    NMEA_Pack(&nmeaMessage, nmea);
    
    if( nmeaMessage.payloadId == NMEA_MSG_GGA ){
        NMEA_Payload_GGA_t gga;
		NMEA_GGA_Parse(&gga, &nmeaMessage);
	}
    return;
}