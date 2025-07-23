
#include <stm32f1xx_hal.h>

#include "adcs.h"
#include "platform/system.h"

int main()
{
    system_init();
    adcs_init();

    uint8_t state = 0;
    while (1)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, state);
        state = !state;

        adcs_update();

        
        volatile uint16_t adcSolar, adcSuperCap;
        adcSolar = adcs_getValue(ADC_V_SOLAR);
        adcSuperCap = adcs_getValue(ADC_V_SUPERCAP);

        HAL_Delay(1000);
    }
    return 0;
}