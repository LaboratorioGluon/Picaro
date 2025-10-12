#ifndef ADCS_H__
#define ADCS_H__

#include <stdint.h>

typedef enum {
    ADC_V_SOLAR = 0, /** Voltage from the sun panel */
    ADC_V_SUPERCAP,  /** SuperCap voltage value  1.2 V */
    ADC_V_VREFINT,   /** Internal voltage reference, shall be */
    ADC_NAME_END
} AdcsName;

/** Initialize the ADC peripheral */
void adcs_init();

/** Call it every loop before using the values */
void adcs_update();

uint16_t adcs_getValueMv(AdcsName sensor);
uint16_t adcs_getValueRaw(AdcsName sensor);

#endif // ADCS_H__