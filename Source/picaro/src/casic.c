#include "casic.h"

#include <string.h>
#include <stdio.h>

#define LOCAL_BUF_LEN 100

static const char *prefixSaveToFlash = "$PCAS00";
static const char *prefixSetBaudRate = "$PCAS01";
static const char *prefixSetUpdateRate = "$PCAS02";
static const char *prefixNmeaSetOutput = "$PCAS03";
static const char *prefixConfigSystem = "$PCAS04";
static const char *prefixQueryInfo = "$PCAS06";
static const char *prefixResetChip = "$PCAS10";

static char localBuf[LOCAL_BUF_LEN] = "";

uint8_t pcas_checksum(char * msg)
{
	/* Support senteces with or without the starting dollar sign. */
	if (*msg == '$') msg++;

	uint8_t checksum = 0x00;

	/*  The optional checksum is an XOR of all bytes between "$" and "*". */
	while (*msg && (*msg != '*')) checksum ^= *msg++;

	return checksum;
}

// CAS00
void pcas_saveToFlash(uint8_t * msg)
{
    strcpy(localBuf, prefixSaveToFlash);
    strcat(localBuf, "*");
    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);
}

// CAS01
void pcas_setBaudRate(uint8_t *msg, CasicBaudRate baudrate)
{
    sprintf(localBuf, "%s,%d*", prefixSetBaudRate, baudrate);
    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);
}

// CAS02
void pcas_setPosUpdateRate(uint8_t *msg, CasicPosRate rate)
{
    sprintf(localBuf, "%s,%d*", prefixSetUpdateRate, rate);

    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);
}

// CAS03
void pcas_setNmeaOutput(uint8_t *msg, CasicNmeaOutput nmeaoutput)
{
    sprintf(localBuf, "%s,%d,%d,%d,%d,%d,%d,%d,%d*", prefixNmeaSetOutput, 
        nmeaoutput.nGGA,
        nmeaoutput.nGLL,
        nmeaoutput.nGSA,
        nmeaoutput.nGSV,
        nmeaoutput.nRMC,
        nmeaoutput.nVTG,
        nmeaoutput.nZDA,
        nmeaoutput.nTXT);

    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);
}

// CAS04
void pcas_configureSystem(uint8_t *msg, CasicSystem system)
{
    sprintf(localBuf, "%s,%d*", prefixConfigSystem, system);

    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);
}

// CAS06
void pcas_queryInformation(uint8_t *msg, CasicInfo info)
{
    sprintf(localBuf, "%s,%d*", prefixQueryInfo, info);

    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);

}

// CAS10
void pcas_reset(uint8_t *msg, CasicReset reset)
{
    sprintf(localBuf, "%s,%d*", prefixResetChip, reset);

    uint8_t check = pcas_checksum(localBuf);
    sprintf((char*)msg, "%s%02X\r\n", localBuf, check);

}
