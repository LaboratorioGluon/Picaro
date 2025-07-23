#include "adcs.h"
#include "platform/system.h"

#include <string.h>
#include <stm32f1xx_hal.h>

static ADC_HandleTypeDef adc;
static uint16_t adcRawValues[ADC_NAME_END] = {0};

void adcs_init()
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    memset(&adc, 0, sizeof(ADC_HandleTypeDef));
    adc.Instance = ADC1;
    adc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc.Init.ContinuousConvMode = DISABLE;
    adc.Init.DiscontinuousConvMode = ENABLE;
    adc.Init.ExternalTrigConv = ADC_SOFTWARE_START; 
    adc.Init.NbrOfConversion = 2;
    adc.Init.NbrOfDiscConversion = 1;
    adc.Init.ScanConvMode = ADC_SCAN_ENABLE;
    HAL_ADC_Init(&adc);

    ADC_ChannelConfTypeDef adcConfig;
    memset(&adcConfig, 0, sizeof(ADC_ChannelConfTypeDef));
    adcConfig.Channel = ADC_CHANNEL_0;
    adcConfig.Rank = ADC_REGULAR_RANK_1;
    adcConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&adc, &adcConfig);

    adcConfig.Channel = ADC_CHANNEL_1;
    adcConfig.Rank = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&adc, &adcConfig);

}

void adcs_update()
{
    
    for( uint8_t i = 0; i < ADC_NAME_END; i++)
    {
        HAL_ADC_Start(&adc);
        HAL_ADC_PollForConversion(&adc, 10);
        adcRawValues[i] = HAL_ADC_GetValue(&adc);
    }

}

uint16_t adcs_getValue(AdcsName sensor)
{
    return adcRawValues[sensor];
}