/**
 * @file ad9361radio.c
 * 
 * @brief Driver and test source code for Analog Devices AD9361 on Mindspeed Transcede EVMs
 * 
 * @par Owner of Intellectual Property
 * Mindspeed Technologies, Inc. 
 * 4000 MACARTHUR BLVD - EAST TOWER
 * NEWPORT BEACH, CA 92660 
 *
 * @copyright
 * Copyright(c) 2009-2012 by Mindspeed Technologies, Inc., all rights reserved
 * @par
 *  ALL WORK CONTAINED IN THIS FILE IS COPYRIGHTED CONFIDENTIAL AND
 *  PROPRIETARY MINDSPEED INTELECTUAL PROPERTY AND IS PROVIDED UNDER THE
 *  TERMS OF A MINDSPEED LICENSE AND NON-DISCLOSURE AGREEMENT (NDA).
 * @par
 *  AUTHORIZED USE, COPYING, DISTRBUTION, AND/OR DERIVATIVE WORKS OF ANY PART
 *  OF THIS WORK ARE STRICTLY GOVERNED BY WRITTEN CONFIDENTIALITY AND LICENSE
 *  AGREEMENTS WITH MINDSPEED TECHNOLOGIES, INC.
 * @par
 *  UNAUTHORIZED USE, COPYPING, DISTRBUTION, AND/OR DERIVATIVE WORKS OF ANY
 *  PART OF THIS WORK ARE STRICTLY PROHIBITED AND MAY RESULT IN CRIMINAL
 *  AND/OR CIVIL PROSECUTION.
 */   
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "../ad9361radio.h"
#include "../spi.h"


#define DUMP_AD9361_REGISTERS

void Ad9361SetLoopback(int fd, uint32_t Loopback)
{
    if (Loopback)
    {
        //
        // Internal loopback requested.  Setup AD9361 for digital loopback
        // (data sent to it from FPGA echoed back on digitial interface)
        //
        Ad9361WriteReg(fd, 0x3f5, 0x01);
        //printf("Setting digital loopback mode on REG 0x3F5=1\n");
    }
    else
    {
        //
        // Internal loopback not requested.  Setup AD9361 for normal
        // TX/RX to and from the actual antenna(s)
        //
        Ad9361WriteReg(fd, 0x3f5, 0x00);
        //printf("Set digital loopback mode off (normal TX/RX)\n");
    }
}


#define AD9361_PRINT_REGS

void Ad9361RadioInit(int fd, PRADCONF pRadConfig)
{

    unsigned char Loopback;
    unsigned char NumAntennas;
    unsigned char BandNumber;
    unsigned int  rc = 0;
    unsigned int  i;
    unsigned char val;
    unsigned char valr;
    PADRADPROFILE pRadProfile;

    BandNumber = pRadConfig->Band;
    NumAntennas = pRadConfig->NumAntennas;
    Loopback = pRadConfig->TxLoopback;
    pRadProfile = (PADRADPROFILE)pRadConfig->pRadProfile;

    // Get revision ID (least significant 3 bits of Product ID register)

    //
    // Send first instruction (common to all revs) to enable reads and writes
    // to the chip
    //
    Ad9361WriteReg(fd, 0x3df, 0x01);
    //
    // Get the revision ID
    //
    val = Ad9361ReadReg(fd, 0x37);
    val &= 0x07; // Mask out all bits except D2 through D0
    printf("AD9361 Revision %u detected\n", val);

    i = 1; // SKIP 1ST WRITE COMMAND AS IT IS WRITTEN ABOVE NOW
    while (pRadProfile[i].OpCode != EORP)
    {
        switch (pRadProfile[i].OpCode)
        {
            case SPIRD:
                //
                // Register read operation
                //
                val = Ad9361ReadReg(fd, pRadProfile[i].Addr);
                //printf("0x%x=0x%x\n", Ad261Lte10Mhz[i].Addr, val);
                break;
            case SPIWR:
                //
                // Write operation, search patch table for
                // alternate value (if any)
                // And write out current data or patched
                // data if there is a match in the patch table
                //

                val = pRadProfile[i].Data;
                // TX antenna enable register
                if (pRadProfile[i].Addr == 2)
                {
                    //
                    // Antennas mask
                    // Bit 7:1 6:1
                    //
                    //val = (NumAntennas == 1) ? 0x5E : 0xDE;
                }
                // RX antenna enable register
                if (pRadProfile[i].Addr == 3)
                {
                    //
                    // Antennas mask
                    // Bit 7:1 6:1
                    //
                    //val = (NumAntennas == 1) ? 0x5E : 0xDE;
                }
                if (pRadProfile[i].Addr == 0x010)
                {
                    if (pRadConfig->RadioMode == RADMODE_PULSE)
                        val |= 1<<3; // Pulse Mode
                    // RX Frame signal Level mode, bit 3 equals 0
                    if (NumAntennas == 2)
                    {
                        val |= 1<<2; // 2R2T Timing
                    }
                }

                if (pRadProfile[i].Addr == 0x006)
                {
                    val = (pRadConfig->RadId == 0) ? 0x0F : 0x00;
                }
                if (pRadProfile[i].Addr == 0x007)
                {
                    val = 0x00;
                }
#ifdef CAL_TIA
				if(pRadProfile[i].Addr >= 0x1DB && pRadProfile[i].Addr <= 0x1DF)
				{
					tia_calibration(fd, pRadProfile[i].Addr, &val);
                }
#endif
#ifdef CAL_ADC
				if(pRadProfile[i].Addr >= 0x200 && pRadProfile[i].Addr <= 0x227)
				{
					adc_calibration(fd, pRadProfile[i].Addr, &val);
                }
#endif
				Ad9361WriteReg(fd, pRadProfile[i].Addr, val);

                break;
            case DELAY:
                //
                // Delay operation:
                //
                sleep(5 * pRadProfile[i].Data / 1000);
                break;
            case WCALDONE:
                //
                // Wait Calibration done operation
                //
                sleep(pRadProfile[i].Data / 1000);
                break;
        }
        i++;
    }

    // set loopback mode
    Ad9361SetLoopback(fd, (Loopback == 1) ? 1 : 0);

    printf("AD9361 driver initialization completed\n");

#ifdef DUMP_AD9361_REGISTERS
    Ad9361DumpRegs(fd);
#endif
}

unsigned int Ad9361ReadReg(int fd, unsigned short addr)
{
    unsigned int val;

    val = SpiDrvReadReg(fd, addr);

    return val;
}

void Ad9361WriteReg(int fd, unsigned short addr, unsigned char data)
{
    SpiDrvWriteReg(fd, addr, data);
}

void Ad9361DumpRegs(int fd)
{
    unsigned int addr;
    unsigned int val;

    printf("AD9361 Registers\n");

    for (addr = 0; addr < 0x400; addr++)
    {
        if ((addr % 16) == 0)
        {
            printf("\n%03x: ", addr);
            usleep(50*1000);
        }
        val = Ad9361ReadReg(fd, addr);
        printf("%02x ", val);
    }
    printf("\n\n");
}
void adc_calibration(int fd, unsigned short addr, unsigned char *pval)
{
	unsigned char data[40];
	unsigned char data0[40] = {0  ,
0  ,
0  ,
36 ,
36 ,
0  ,
0  ,
123,
42 ,
59 ,
74 ,
26 ,
77 ,
24 ,
0  ,
126,
125,
126,
72 ,
71 ,
72 ,
75 ,
74 ,
75 ,
46 ,
176,
49 ,
28 ,
176,
49 ,
28 ,
176,
49 ,
55 ,
56 ,
64 ,
64 ,
44 ,
0  ,
0  ,
};
	unsigned int rxbbf_c3_msb, rxbbf_c3_lsb, rxbbf_r2346;
	float scale_snr_dB, rc_timeConst, scale_res, scale_cap, scale_snr, maxsnr;
	float BBBW_MHz, FsADC;//需确定
	BBBW_MHz = 9;
	FsADC = 491.52;
	rxbbf_c3_msb = Ad9361ReadReg(fd, 0x1eb);
	rxbbf_c3_lsb = Ad9361ReadReg(fd, 0x1ec);
	rxbbf_r2346 = Ad9361ReadReg(fd, 0x1e6);
	if(BBBW_MHz > 28)
	{
		BBBW_MHz = 28;
	}
	else if(BBBW_MHz < 0.2)
	{
		BBBW_MHz = 0.2;
	}
	scale_snr_dB = (FsADC < 80) ? (0):(2);
	if(BBBW_MHz < 18)
	{
		rc_timeConst = 1/((1.4 * 2 * PI) * (18300 * rxbbf_r2346) * (160e-15 * rxbbf_c3_msb + 10e-15 * rxbbf_c3_lsb + 140e-15) * (BBBW_MHz * 1e6));
	}
	else
	{
		rc_timeConst = 1/((1.4 * 2 * PI) * (18300 * rxbbf_r2346) * (160e-15 * rxbbf_c3_msb + 10e-15 * rxbbf_c3_lsb + 140e-15) * (BBBW_MHz * 1e6) * (1 + 0.01 * (BBBW_MHz - 18)));
	}
	scale_res = sqrt(1 / rc_timeConst);
	scale_cap = sqrt(1 / rc_timeConst);
	scale_snr = pow(10, (scale_snr_dB/10));
	maxsnr = 640/160;
	data[0] = 0;
	data[1] = 0;
	data[2] = 0;
	data[3] = 0x24;
	data[4] = 0x24;
	data[5] = 0;
	data[6] = 0;
	data[7] = min(124, myfloor(-0.5 + 80 * scale_snr * scale_res * min(1, sqrt(maxsnr * FsADC / 640))));
	data[8] = min(255, myfloor(0.5 + 20 * (640 / FsADC) * (data[7] / 80.0) / (scale_res * scale_cap)));
	data[10] = min(127, myfloor(-0.5 + 77 * scale_res * min(1, sqrt(maxsnr * FsADC / 640))));
	data[9] = min(127, myfloor(0.8 * data[10]));
	data[11] = min(255, myfloor(0.5 + 20 * (640 / FsADC) * (data[10] / 77.0) / (scale_res * scale_cap)));
	data[12] = min(127, myfloor(-0.5 + 80 * scale_res * min(1, sqrt(maxsnr * FsADC / 640))));
	data[13] = min(255, myfloor(-1.5 + 20 * (640 / FsADC) * (data[12] / 80.0) / (scale_res * scale_cap)));
	data[14] = 21 * myfloor(0.1 * 640 / FsADC);
	data[15] = min(127, myround(1.025 * data[7]));
	data[16] = min(127, myfloor(data[15] * (0.98 + 0.02 * max(1, (640 / FsADC) / maxsnr))));
	data[17] = data[15];
	data[18] = min(127, myround(0.975 * data[10]));
	data[19] = min(127, myfloor(data[18] * (0.98 + 0.02 * max(1, (640 / FsADC) / maxsnr))));
	data[20] = data[18];
	data[21] = min(127, myround(0.975 * data[12]));
	data[22] = min(127, myfloor(data[21] * (0.98 + 0.02 * max(1, (640 / FsADC) / maxsnr))));
	data[23] = data[21];
	data[24] = 0x2e;
	data[25] = myfloor(128 + min(63, 63 * (FsADC / 640)));
	data[26] = myfloor(0 + min(63, 63 * (FsADC / 640) * (0.92 + 0.08 * (640 / FsADC))));
	data[27] = myfloor(0 + min(63, 32 * sqrt(FsADC / 640)));
	data[28] = myfloor(128 + min(63, 63 * (FsADC / 640)));
	data[29] = myfloor(0 + min(63, 63 * (FsADC / 640) * (0.92 + 0.08 * (640 / FsADC))));
	data[30] = myfloor(0 + min(63, 32 * sqrt(FsADC / 640)));
	data[31] = myfloor(128 + min(63, 63 * (FsADC / 640)));
	data[32] = myfloor(0 + min(63, 63 * (FsADC / 640) * (0.92 + 0.08 * (640 / FsADC))));
	data[33] = myfloor(0 + min(63, 63 * sqrt(FsADC / 640)));
	data[34] = min(127, myfloor(64 * sqrt(FsADC / 640)));
	data[35] = 0x40;
	data[36] = 0x40;
	data[37] = 0x2c;
	data[38] = 0x0;
	data[39] = 0x0;
	
	*pval = data[addr-0x200];
}
unsigned int myfloor(float val)
{
	return (int)(val);
}
unsigned int myround(float val)
{
	return (int)(val + 0.5);
}
unsigned int myceil(float val)
{
	return (int)(val + 1.0);
}
float min(float x, float y)
{
	return (x > y) ? (y):(x);
}
float max(float x, float y)
{
	return (x > y) ? (x):(y);
}

void tia_calibration(int fd, unsigned short addr, unsigned char *pval)
{
	unsigned char reg1DB, reg1DC, reg1DD, reg1DE, reg1DF;
	unsigned int reg1EB, reg1EC, reg1E6, Cbbf, R2346, temp;
	float BBBW_MHz, CTIA_fF;//需确定
	BBBW_MHz = 9;
	reg1EB = Ad9361ReadReg(fd, 0x1eb);
	reg1EC = Ad9361ReadReg(fd, 0x1ec);
	reg1E6 = Ad9361ReadReg(fd, 0x1e6);
	if(BBBW_MHz > 28)
	{
		BBBW_MHz = 28;
	}
	else if(BBBW_MHz < 0.2)
	{
		BBBW_MHz = 0.2;
	}
	Cbbf = (reg1EB * 160) + (reg1EC * 10) + 140;//fF
	R2346 = 18300 * (reg1E6 & 7);
	CTIA_fF = (Cbbf * R2346 * 0.56) / 3500;
	if(myceil(BBBW_MHz) <= 3)
	{
		reg1DB = 0xE0;
	}
	else if(myceil(BBBW_MHz) > 3 && myceil(BBBW_MHz) <= 10)
	{
		reg1DB = 0x60;
	}
	else if(myceil(BBBW_MHz) > 10)
	{
		reg1DB = 0x20;
	}
	if(CTIA_fF > 2920)
	{
		reg1DC = 0x40;
		reg1DE = 0x40;
		temp = min(127, myround((CTIA_fF - 400) / 320));
		reg1DD = temp;
		reg1DF = temp;
	}
	else
	{
		temp = myround((CTIA_fF - 400) / 40) + 0x40;
		reg1DC = temp;
		reg1DE = temp;
		reg1DD = 0;
		reg1DF = 0;
	}
	if(addr == 0x1DB)
		*pval = reg1DB;
	else if(addr == 0x1DC)
		*pval = reg1DC;
	else if(addr == 0x1DD)
		*pval = reg1DD;
	else if(addr == 0x1DE)
		*pval = reg1DE;
	else if(addr == 0x1DF)
		*pval = reg1DF;	
}
