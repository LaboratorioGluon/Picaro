#include "config.h"

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

    debug_send("************************\r\n");
    debug_send("Hello world from Picaro!\r\n");
    debug_send("************************\r\n");
    uint8_t state = 0;

    while (1)
    {
        adcs_update();

        volatile uint16_t adcSolar, adcSuperCap;
        adcSolar = adcs_getValue(ADC_V_SOLAR);
        adcSuperCap = adcs_getValue(ADC_V_SUPERCAP);

        gps_readData();
    }
    return 0;
}