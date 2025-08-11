#include "gps.h"
#include <stm32f1xx_hal.h>
#include <string.h>

#include "nmea.h"
#include "casic.h"

#define RAW_DATA_BUFFER_SIZE 256
#define READ_DATA_CHUNK_SIZE 90

static UART_HandleTypeDef gpsUart;
static uint8_t rawDataBuffer[RAW_DATA_BUFFER_SIZE]="";
static uint32_t rawDataBufferCurrent = 0;


void gps_parseNMEA(uint8_t *nmea);

void gps_configureUart(uint32_t baudrate)
{
    gpsUart.Instance = USART1;
    gpsUart.Init.BaudRate = baudrate;
    gpsUart.Init.WordLength = UART_WORDLENGTH_8B;
    gpsUart.Init.StopBits = UART_STOPBITS_1;
    gpsUart.Init.Parity = UART_PARITY_NONE;
    gpsUart.Init.Mode = UART_MODE_TX_RX;
    gpsUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    gpsUart.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&gpsUart);
}

void gps_init()
{
    __HAL_RCC_USART1_CLK_ENABLE();

    gps_configureUart(115200);   

    uint8_t buf[200];
    pcas_queryInformation(buf, CASIC_INFO_FW);
    HAL_UART_Transmit(&gpsUart, buf, strlen(buf), 1000);

    //HAL_Delay(100);
    HAL_UART_Receive(&gpsUart, buf, 6, 1000);
    buf[6] = '\0';

    // Check the received data, if the data is correct
    // the module is already working in 115200
    // otherwise reset the baudrate configuration.
    if (strcmp(buf, "$GPTXT") != 0)
    {
        gps_configureUart(9600);

        pcas_setBaudRate(buf, CASIC_BR_115200);
        HAL_UART_Transmit(&gpsUart, buf, strlen(buf), 1000);

        pcas_reset(buf, CASIC_RESET_HOT);
        HAL_UART_Transmit(&gpsUart, buf, strlen(buf), 1000);

        gps_configureUart(115200);
    }


    CasicNmeaOutput nmeaOutput;
    nmeaOutput.nGGA=1;
    nmeaOutput.nGLL=0;
    nmeaOutput.nGSA=1;
    nmeaOutput.nGSV=0;
    nmeaOutput.nRMC=1;
    nmeaOutput.nVTG=1;
    nmeaOutput.nZDA=0;
    nmeaOutput.nTXT=1;
    pcas_setNmeaOutput(buf, nmeaOutput);
    HAL_UART_Transmit(&gpsUart, buf, strlen(buf), 1000);
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
        volatile NMEA_Payload_GGA_t gga;
        NMEA_GGA_Parse(&gga, &nmeaMessage);
	}

    return;
}