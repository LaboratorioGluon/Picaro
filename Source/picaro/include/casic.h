#ifndef CASIC_H__
#define CASIC_H__

#include <stdint.h>

typedef enum{
    CASIC_BR_4800 = 0,
    CASIC_BR_9600,
    CASIC_BR_19200,
    CASIC_BR_38400,
    CASIC_BR_57600,
    CASIC_BR_115200,
} CasicBaudRate;

typedef enum{
    CASIC_POS_RATE_1HZ = 1000,
    CASIC_POS_RATE_2HZ = 500,
    CASIC_POS_RATE_4HZ = 250,
    CASIC_POS_RATE_5HZ = 200,
    CASIC_POS_RATE_10HZ = 100,
} CasicPosRate;

typedef enum{
    CASIC_RESET_HOT = 0,
    CASIC_RESET_WARM,
    CASIC_RESET_COLD,
    CASIC_RESET_FACTORY,
    CASIC_RESET_TURNOFFSERIAL = 8,
    CASIC_RESET_TURNONSERIAL = 9,
} CasicReset;

typedef enum{
    CASIC_INFO_FW = 0,
    CASIC_INFO_HW_SERIALNB,
    CASIC_INFO_MODE,
    CASIC_INFO_CUSTOM_NUMBER,
    CASIC_UPGRADE_CODE = 5
} CasicInfo;

typedef enum{
    CASIC_SYS_GPS = 0,
    CASIC_SYS_BDS,
    CASIC_SYS_GPS_BDS,
    CASIC_SYS_GLONASS,
    CASIC_SYS_GPS_GLONASS,
    CASIC_SYS_BDS_GLONASS,
    CASIS_SYS_GPS_BDS_GLONASS
} CasicSystem;

typedef struct{
    uint8_t nGGA;
    uint8_t nGLL;
    uint8_t nGSA;
    uint8_t nGSV;
    uint8_t nRMC;
    uint8_t nVTG;
    uint8_t nZDA;
    uint8_t nTXT;
} CasicNmeaOutput;

// CAS00
void pcas_saveToFlash(uint8_t * msg);

// CAS01
void pcas_setBaudRate(uint8_t *msg, CasicBaudRate baudrate);

// CAS02
void pcas_setPosUpdateRate(uint8_t *msg, CasicPosRate rate);

// CAS03
void pcas_setNmeaOutput(uint8_t *msg, CasicNmeaOutput nmeaoutput);

// CAS04
void pcas_configureSystem(uint8_t *msg, CasicSystem system);

// CAS06
void pcas_queryInformation(uint8_t *msg, CasicInfo info);

// CAS10
void pcas_reset(uint8_t *msg, CasicReset reset);
#endif //CASIC_H__