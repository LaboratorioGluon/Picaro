
#include "platform/system.h"
#include "pinout.h"
#include <stm32f1xx_hal.h>
#include <string.h>

typedef struct {
    GPIO_TypeDef*    port;
    GPIO_InitTypeDef gpioDef;
} gpioInitConfig;

/*************************/
/** GPIO CONFIGURATION  **/
/*************************/

const gpioInitConfig gpioConfig[] = {

    // ADC
    { ADC_VSOLAR_PORT,      { ADC_VSOLAR_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }         },
    { ADC_VCAP_PORT,        { ADC_VCAP_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }           },
    { ADC_TEMP_PORT,        { ADC_TEMP_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }           },

    // GPS - UART
    { GPS_RX_PORT,          { GPS_RX_PIN, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }              },
    { GPS_TX_PORT,          { GPS_TX_PIN, GPIO_MODE_AF_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }           },
    { GPS_NRESET_PORT,      { GPS_NRESET_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }      }, // GPS_RESET
    { GPS_LNASHDN_PORT,     { GPS_LNASHDN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }     }, // LNA_SHDN
    { GPS_ONOFF_PORT,       { GPS_ONOFF_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }       },
    // GPS_ONOFF

    /*{ GPS_RX_PORT,          { GPS_RX_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }             },
    { GPS_TX_PORT,          { GPS_TX_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }             },
    { GPS_NRESET_PORT,      { GPS_NRESET_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }         }, // GPS_RESET
    { GPS_LNASHDN_PORT,     { GPS_LNASHDN_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }        }, // LNA_SHDN
    { GPS_ONOFF_PORT,       { GPS_ONOFF_PIN, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }          },*/
    // GPS_ONOFF

    // DEBUG - UART
    { GPIOB,                { GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }             },
    { GPIOB,                { GPIO_PIN_11, GPIO_MODE_AF_INPUT, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }          },

    // LEDS - GPIO
    { LEDS_RED_PORT,        { LEDS_RED_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }        },

    // PSU
    { PSU_SUPERCAP_EN_PORT, { PSU_SUPERCAP_EN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW } }, // SUPERCAP_EN
    { PSU_BOOST_EN_PORT,    { PSU_BOOST_EN_PIN, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW }    }  // BOOST_EN
};

const uint32_t gpioConfigSize = sizeof(gpioConfig) / sizeof(gpioInitConfig);

/************************************************/

static void system_initAllGpio()
{
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef config;
    config.Mode  = GPIO_MODE_INPUT;
    config.Pin   = GPIO_PIN_All;
    config.Pull  = GPIO_PULLDOWN;
    config.Speed = GPIO_SPEED_FREQ_LOW;
    // HAL_GPIO_Init(gpioConfig[i].port, &gpioConfig[i].gpioDef);
    HAL_GPIO_Init(GPIOA, &config);
    HAL_GPIO_Init(GPIOB, &config);
    HAL_GPIO_Init(GPIOC, &config);
    HAL_GPIO_Init(GPIOD, &config);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_All, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_All, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_All, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_All, GPIO_PIN_RESET);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
}

static void system_initGpio()
{
    system_initAllGpio();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    for (uint32_t i = 0; i < gpioConfigSize; i++) {
        HAL_GPIO_Init(gpioConfig[i].port, &gpioConfig[i].gpioDef);
    }

    HAL_GPIO_WritePin(PSU_BOOST_EN_PORT, PSU_BOOST_EN_PIN, GPIO_PIN_RESET);

    __HAL_RCC_GPIOA_CLK_DISABLE();
    __HAL_RCC_GPIOB_CLK_DISABLE();
    __HAL_RCC_GPIOC_CLK_DISABLE();
    __HAL_RCC_GPIOD_CLK_DISABLE();
    /*HAL_GPIO_WritePin(GPS_ONOFF_PORT, GPS_ONOFF_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPS_NRESET_PORT, GPS_NRESET_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPS_LNASHDN_PORT, GPS_LNASHDN_PIN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(PSU_SUPERCAP_EN_PORT, PSU_SUPERCAP_EN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(PSU_BOOST_EN_PORT, PSU_BOOST_EN_PIN, GPIO_PIN_RESET);*/
}

static void system_initClocks()
{
    RCC_OscInitTypeDef       RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = { 0 };

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.LSIState            = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState        = RCC_PLL_NONE;
    /*RCC_OscInitStruct.PLL.PLLState  = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL    = RCC_PLL_MUL12;*/
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        // Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    // RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSE;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        // Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC;
    PeriphClkInit.RTCClockSelection    = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV4;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        // Error_Handler();
    }
}

void system_init()
{
    system_initClocks();
    system_initGpio();
}