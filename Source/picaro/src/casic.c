#include "casic.h"

#include <string.h>
#include <stdio.h>

#define LOCAL_BUF_LEN 100

static uint8_t *prefixSaveToFlash = "$PCAS00";
static uint8_t *prefixSetBaudRate = "$PCAS01";
static uint8_t *prefixNmeaSetOutput = "$PCAS03";
static uint8_t *prefixQueryInfo = "$PCAS06";
static uint8_t *prefixResetChip = "$PCAS10";

uint8_t pcas_checksum(uint8_t * msg)
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
    uint8_t buf[LOCAL_BUF_LEN];
    strcpy(buf, prefixSaveToFlash);
    strcat(buf, "*");
    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);
}

// CAS01
void pcas_setBaudRate(uint8_t *msg, CasicBaudRate baudrate)
{
    uint8_t buf[LOCAL_BUF_LEN];
    sprintf(buf, "%s,%d*", prefixSetBaudRate, baudrate);
    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);
}

// CAS02
void pcas_setPosUpdateRate(uint8_t *msg, CasicPosRate rate)
{
    uint8_t buf[LOCAL_BUF_LEN];
    sprintf(buf, "%s,%d*", prefixQueryInfo, rate);

    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);
}

// CAS03
void pcas_setNmeaOutput(uint8_t *msg, CasicNmeaOutput nmeaoutput)
{
    uint8_t buf[LOCAL_BUF_LEN];
    sprintf(buf, "%s,%d,%d,%d,%d,%d,%d,%d,%d*", prefixNmeaSetOutput, 
        nmeaoutput.nGGA,
        nmeaoutput.nGLL,
        nmeaoutput.nGSA,
        nmeaoutput.nGSV,
        nmeaoutput.nRMC,
        nmeaoutput.nVTG,
        nmeaoutput.nZDA,
        nmeaoutput.nTXT);
    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);
}

void pcas_configureSystem(uint8_t *msg, CasicSystem system)
{
    uint8_t buf[LOCAL_BUF_LEN];
    sprintf(buf, "%s,%d*", prefixQueryInfo, system);

    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);
}

void pcas_queryInformation(uint8_t *msg, CasicInfo info)
{
    uint8_t buf[LOCAL_BUF_LEN];
    sprintf(buf, "%s,%d*", prefixQueryInfo, info);

    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);

}


void pcas_reset(uint8_t *msg, CasicReset reset)
{
    uint8_t buf[LOCAL_BUF_LEN];
    sprintf(buf, "%s,%d*", prefixQueryInfo, prefixResetChip);

    uint8_t check = pcas_checksum(buf);
    sprintf(msg, "%s%02X\r\n", buf, check);

}