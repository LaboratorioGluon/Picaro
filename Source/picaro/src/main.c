#include "config.h"

#include <stm32f1xx_hal.h>
#include <string.h>

#include "adcs.h"
#include "gps.h"
#include "picaro.h"
#include "pinout.h"
#include "platform/system.h"
#include "psu.h"
#include "uartDebug.h"

WorkingMode       workingMode = WORKING_MODE_LOWPOWER;
RTC_HandleTypeDef rtc;

void lowPowerLoop();
void normalPowerLoop();

int main()
{

    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
    //    __HAL_RCC_BACKUPRESET_RELEASE();
    HAL_Init();
    system_init();

    adcs_init();
    gps_init();

    // debug_init();

    /*debug_send("************************\r\n");
    debug_send("Hello world from Picaro!\r\n");
    debug_send("************************\r\n");*/
    uint8_t state = 0;
    memset(&rtc, 0, sizeof(RTC_HandleTypeDef));
    rtc.Instance          = RTC;
    rtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
    rtc.Init.OutPut       = RTC_OUTPUTSOURCE_ALARM;

    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_BKP_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();

    HAL_RTC_Init(&rtc);

    RTC_TimeTypeDef time = { 0 };

    time.Hours   = 0;
    time.Minutes = 0;
    time.Seconds = 0;

    HAL_RTC_SetTime(&rtc, &time, RTC_FORMAT_BIN);

    // HAL_RTC_WaitForSynchro(&rtc);

    __HAL_RCC_GPIOB_CLK_ENABLE();

    while (1) {

        // Read sensors.
        adcs_update();

        psu_update(&workingMode);

        if (workingMode == WORKING_MODE_FULL) {
            gps_readData();

            // Read other sensors

            // Send using LoRa
        }

        RTC_AlarmTypeDef alarm;
        alarm.Alarm             = RTC_ALARM_A;
        alarm.AlarmTime.Hours   = 0;
        alarm.AlarmTime.Minutes = 0;
        alarm.AlarmTime.Seconds = 2;

        HAL_RTC_GetTime(&rtc, &time, RTC_FORMAT_BIN);

        alarm.AlarmTime.Seconds = time.Seconds + 5;

        HAL_RTC_SetAlarm(&rtc, &alarm, RTC_FORMAT_BIN);

        HAL_RTC_PollForAlarmAEvent(&rtc, HAL_MAX_DELAY);

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        // HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

        // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        /*adcs_update();

        volatile uint16_t adcSolar, adcSuperCap;
        adcSolar = adcs_getValue(ADC_V_SOLAR);
        adcSuperCap = adcs_getValue(ADC_V_SUPERCAP);
        */
    }
    return 0;
}