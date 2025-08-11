
#include <stm32f1xx_hal.h>

#include "adcs.h"
#include "gps.h"
#include "platform/system.h"
#include "uartDebug.h"

int main()
{
    system_init();
    adcs_init();
    gps_init();
    debug_init();

    debug_send("Hola Mundo!\r\n");
    uint8_t state = 0;
    while (1)
    {
        //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, state);
        //state = !state;

        adcs_update();

        volatile uint16_t adcSolar, adcSuperCap;
        adcSolar = adcs_getValue(ADC_V_SOLAR);
        adcSuperCap = adcs_getValue(ADC_V_SUPERCAP);

        
        gps_readData();

        //HAL_Delay(1000);
    }
    return 0;
}