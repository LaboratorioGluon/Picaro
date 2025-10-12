#include "adcs.h"
#include "platform/system.h"

#include <stm32f1xx_hal.h>
#include <string.h>

// Calibrate for each device.
#define VREFINT_EXPECTED_MV 1217
#define ADC_SAMPLES         10

static ADC_HandleTypeDef adc;
static uint16_t          calculatedVdd;
static uint16_t          adcRawValues[ADC_NAME_END] = { 0 };

void adc_calibrate(uint16_t rawVrefInt)
{
    calculatedVdd = VREFINT_EXPECTED_MV * 4095 / rawVrefInt;
}

void adcs_init()
{
    __HAL_RCC_ADC1_CLK_ENABLE();

    memset(&adc, 0, sizeof(ADC_HandleTypeDef));
    adc.Instance                   = ADC1;
    adc.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
    adc.Init.ContinuousConvMode    = DISABLE;
    adc.Init.DiscontinuousConvMode = ENABLE;
    adc.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
    adc.Init.NbrOfConversion       = 3;
    adc.Init.NbrOfDiscConversion   = 1;
    adc.Init.ScanConvMode          = ADC_SCAN_ENABLE;
    HAL_ADC_Init(&adc);

    ADC_ChannelConfTypeDef adcConfig;
    memset(&adcConfig, 0, sizeof(ADC_ChannelConfTypeDef));
    adcConfig.Channel      = ADC_CHANNEL_0;
    adcConfig.Rank         = ADC_REGULAR_RANK_1;
    adcConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&adc, &adcConfig);

    adcConfig.Channel = ADC_CHANNEL_1;
    adcConfig.Rank    = ADC_REGULAR_RANK_2;
    HAL_ADC_ConfigChannel(&adc, &adcConfig);

    adcConfig.Channel = ADC_CHANNEL_17;
    adcConfig.Rank    = ADC_REGULAR_RANK_3;
    HAL_ADC_ConfigChannel(&adc, &adcConfig);
}

void adcs_update()
{

    uint32_t adcSum[ADC_NAME_END];
    memset(adcSum, 0, sizeof(uint32_t) * ADC_NAME_END);

    for (uint8_t s = 0; s < ADC_SAMPLES; s++) {

        for (uint8_t i = 0; i < ADC_NAME_END; i++) {
            HAL_ADC_Start(&adc);
            HAL_ADC_PollForConversion(&adc, 10);
            adcSum[i] += HAL_ADC_GetValue(&adc);
        }
    }
    adcRawValues[ADC_V_SOLAR]    = adcSum[ADC_V_SOLAR] / ADC_SAMPLES;
    adcRawValues[ADC_V_SUPERCAP] = adcSum[ADC_V_SUPERCAP] / ADC_SAMPLES;
    adcRawValues[ADC_V_VREFINT]  = adcSum[ADC_V_VREFINT] / ADC_SAMPLES;
    adc_calibrate(adcRawValues[ADC_V_VREFINT]);
}

uint16_t adcs_getValueRaw(AdcsName sensor)
{
    return adcRawValues[sensor];
}

uint16_t adcs_getValueMv(AdcsName sensor)
{
    return adcRawValues[sensor] * calculatedVdd * 1.0f / 4095.0f;
}