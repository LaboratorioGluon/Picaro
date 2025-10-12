
#include "psu.h"
#include "adcs.h"

#include <stm32f1xx_hal.h>

#define SOLAR_MV_THRESH 1000

#define PSU_BOOST_EN_PORT GPIOB
#define PSU_BOOST_EN_PIN  GPIO_PIN_9

void psu_setBoost(uint8_t boostEnable)
{
}

void psu_setSCSwitch(uint8_t scSwitch)
{
}

void psu_update(WorkingMode* mode)
{

    // Check solar voltage.
    volatile uint32_t solarmV = adcs_getValueMv(ADC_V_SOLAR);

    if (solarmV > SOLAR_MV_THRESH) {
        // Turn on Boost Converter.
        *mode = WORKING_MODE_FULL;
    } else {
        // Turn off Boost Converter
        *mode = WORKING_MODE_LOWPOWER;
    }
    (void)solarmV;
}