#ifndef __AD9361RADIO_H__
#define __AD9361RADIO_H__

#include <stdint.h>
#include <stdbool.h>
#include "trioctl.h"

#define PI (3.1415)
#define CAL_ADC
#define CAL_TIA

#define EORP        0
#define SPIRD       1
#define SPIWR       2
#define DELAY       3
#define WCALDONE    4

#define BBPLL       0x10
#define RXCP        0x11
#define TXCP        0x12
#define RXFILTER    0x13
#define TXFILTER    0x14
#define RFDC        0x15
#define TXQUAD      0x16
#define BBDC        0x17

/** @brief Structure for script based programming of AD9361 */
typedef struct tagADRADPROFILE
{
    unsigned short  OpCode;
    unsigned short  Addr;
    unsigned short  Data;
} ADRADPROFILE, *PADRADPROFILE; 

void Ad9361SetLoopback(int fd, unsigned int Loopback);
void Ad9361RadioReset(void);
void Ad9361RadioInit(int fd, PRADCONF pRadConfig);
unsigned int Ad9361ReadReg(int fd, unsigned short addr);
void Ad9361WriteReg(int fd, unsigned short addr, unsigned char data);
void Ad9361DumpRegs(int fd);
void adc_calibration(int fd, unsigned short Addr, unsigned char *pval);
void tia_calibration(int fd, unsigned short addr, unsigned char *pval);
// uint32_t Ad9361ApiHandler(PAPIHEADER pApi, PAPIHEADER pResp);
float max(float x, float y);
float min(float x, float y);
unsigned int myceil(float val);
unsigned int myround(float val);
unsigned int myfloor(float val);
#endif
