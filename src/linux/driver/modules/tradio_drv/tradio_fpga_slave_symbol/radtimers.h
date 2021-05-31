
/**
 * @file radtimers.h
 *
 * @brief Constants/Typers Defnition and Function Prototypes of T2200 Radio Timers
 *
 * @copyright
 * Copyright(c) 2011-2012 by Mindspeed Technologies, Inc., all rights reserved
 *
 * Mindspeed Technologies, Inc.     \n
 * 4000 MACARTHUR BLVD - EAST TOWER \n
 * NEWPORT BEACH, CA 92660 
 *
 * @par License
 *  ALL WORK CONTAINED IN THIS FILE IS COPYRIGHTED CONFIDENTIAL AND
 *  PROPRIETARY MINDSPEED INTELECTUAL PROPERTY AND IS PROVIDED UNDER THE
 *  TERMS OF A MINDSPEED LICENSE AND NON-DISCLOSURE AGREEMENT (NDA).
 *  
 *  AUTHORIZED USE, COPYING, DISTRBUTION, AND/OR DERIVATIVE WORKS OF ANY PART
 *  OF THIS WORK ARE STRICTLY GOVERNED BY WRITTEN CONFIDENTIALITY AND LICENSE
 *  AGREEMENTS WITH MINDSPEED TECHNOLOGIES, INC.
 *
 *  UNAUTHORIZED USE, COPYPING, DISTRBUTION, AND/OR DERIVATIVE WORKS OF ANY
 *  PART OF THIS WORK ARE STRICTLY PROHIBITED AND MAY RESULT IN CRIMINAL
 *  AND/OR CIVIL PROSECUTION.
 */

#ifndef __RADTIMERS_H__
#define __RADTIMERS_H__

#define RADMOD_3G                           0
#define RADMOD_4G                           1

#define CHIP_SRATE                          3840

#define SET_FSYNC_CLK_SRC(src)              (((src)<<2) | 1)
#define FSYNC_CLK_AXI                       0 // AXI clock. Used for reset and testmode.
#define FSYNC_RPDIF0_MCLK                   1 // RPDIF0 MCLK
#define FSYNC_RPDIF1_MCLK                   2 // RPDIF1 MCLK
#define FSYNC_CPRI_CLK                      3 // CPRI clock
#define FSYNC_CPRI_TX_CLK                   4 // CPRI TX clock
#define FSYNC_CPRI_RX_CLK                   5 // CPRI RX clock
#define FSYNC_CLK_EXT_SYNC                  6 // External Sync Clock

#define FSYNCTMID_CHIP                     0
#define FSYNCTMID_SLOT                     1
#define FSYNCTMID_FRAME                    2

#define NUM_FSYNC_TIMERS                   6

#define RAD_CFG_BASEADDR                   0xF5E00000
#define RAD_TM_BASEADDR                    0xF5E320C0
#define PLL_M_LSB(n)                       (0xF4CF01C0+0x20*n+0x00)
#define PLL_M_MSB(n)                       (0xF4CF01C0+0x20*n+0x04)

#define SYS_REF_CLK     25
#define AXI_CLK_DIV_CNTRL               0xF4CF004C  /**< AXI Clock Divider Control Register */


//psc
#define RADTIMER_BASE_ADDR 0xf5e30000
#define RADTIMER_MAP_SIZE  0x10000
#define RADGROUP_BASE_ADDR 0xf5b00000
#define RADGROUP_MAP_SIZE  0x400000
typedef struct tFSYNCTMREGS
{
    volatile unsigned int PRESCALE_CLK_CNTRL;           /**< 0x00 Prescale Clock Control */
    volatile unsigned int PRESCALE_DIV_CNTRL;           /**< 0x04 Prescale Clock Divider Control */
    volatile unsigned int TIMER_CNTRL;                  /**< 0x08 Timer Control */
    volatile unsigned int BASIC_CNTR_TC_VAL;            /**< 0x0C Basic Counter Terminal Count */
    volatile unsigned int CHIP_CNTR_TC_VAL_A;           /**< 0x10 Chip Counter Terminal Count A */
    volatile unsigned int SLOT_CNTR_TC_VAL_A;           /**< 0x14 Slot Counter Terminal Count A */
    volatile unsigned int FRAME_CNTR_TC_VAL_A;          /**< 0x18 Frame Counter Terminal Count A */
    volatile unsigned int CHIP_CNTR_COMP_VAL_A;         /**< 0x1C Chip Counter Compare Value A */
    volatile unsigned int SLOT_CNTR_COMP_VAL_A;         /**< 0x20 Slot Counter Compare Value A */
    volatile unsigned int FRAME_CNTR_COMP_VAL_A;        /**< 0x24 Frame Counter Compare Value A */
    volatile unsigned int CHIP_CNTR_MASK_VAL_A;         /**< 0x28 Chip Counter Compare Mask A */
    volatile unsigned int SLOT_CNTR_MASK_VAL_A;         /**< 0x2C Slot Counter Compare Mask A */
    volatile unsigned int FRAME_CNTR_MASK_VAL_A;        /**< 0x30 Frame Counter Compare Mask A */
    volatile unsigned int CHIP_CNTR_TC_VAL_B;           /**< 0x34 Chip Counter Terminal Count B */
    volatile unsigned int SLOT_CNTR_TC_VAL_B;           /**< 0x38 Slot Counter Terminal Count B */
    volatile unsigned int FRAME_CNTR_TC_VAL_B;          /**< 0x3C Frame Counter Terminal Count B */
    volatile unsigned int CHIP_CNTR_COMP_VAL_B;         /**< 0x40 Chip Counter Compare Value B */
    volatile unsigned int SLOT_CNTR_COMP_VAL_B;         /**< 0x44 Slot Counter Compare Value B */
    volatile unsigned int FRAME_CNTR_COMP_VAL_B;        /**< 0x48 Frame Counter Compare Value B */
    volatile unsigned int CHIP_CNTR_MASK_VAL_B;         /**< 0x4C Chip Counter Compare Mask B */
    volatile unsigned int SLOT_CNTR_MASK_VAL_B;         /**< 0x50 Slot Counter Compare Mask B */
    volatile unsigned int FRAME_CNTR_MASK_VAL_B;        /**< 0x54 Frame Counter Compare Mask B */
    volatile unsigned int CHIP_CNTR_OFFSET_VAL;         /**< 0x58 Chip Counter Offset Value */
    volatile unsigned int SLOT_CNTR_OFFSET_VAL;         /**< 0x5C Slot Counter Offset Value */
    volatile unsigned int FRAME_CNTR_OFFSET_VAL;        /**< 0x60 Frame Counter Offset Value */
    volatile unsigned int BASIC_CNTR_INIT_VAL;          /**< 0x64 Basic Counter Initial Value */
    volatile unsigned int CHIP_CNTR_INIT_VAL;           /**< 0x68 Chip Counter Initial Value */
    volatile unsigned int SLOT_CNTR_INIT_VAL;           /**< 0x6C Slot Counter Initial Value */
    volatile unsigned int FRAME_CNTR_INIT_VAL;          /**< 0x70 Frame Counter Initial Value */
    volatile unsigned int SUPER_FRAME_CNTR_INIT_VAL;    /**< 0x74 Super Frame Counter Initial Value */
    volatile unsigned int SUPER_FRAME_CNTR_TC_VAL_A;    /**< 0x78 Super Frame Counter Terminal Value A */
    volatile unsigned int SUPER_FRAME_CNTR_COMP_VAL_A;  /**< 0x7C Super Frame Counter Compare Value A */
    volatile unsigned int SUPER_FRAME_CNTR_MSK_VAL_A;   /**< 0x80 Super Frame Counter Mask Value A */
    volatile unsigned int SUPER_FRAME_CNTR_TC_VAL_B;    /**< 0x84 Super Frame Counter Terminal Value B */
    volatile unsigned int SUPER_FRAME_CNTR_COMP_VAL_B;  /**< 0x88 Super Frame Counter Compare Value B */
    volatile unsigned int SUPER_FRAME_CNTR_MSK_VAL_B;   /**< 0x8C Super Frame Counter Mask Value B */
    volatile unsigned int SUPER_FRAME_CNTR_OFFSET_VAL;  /**< 0x90 Super Frame Counter Offset Value */
    volatile unsigned int INTRPTS_CNTRL;                /**< 0x94 Interrupt Control */
    volatile unsigned int INTRPTS_STATUS;               /**< 0x98 Timer Interrupt Status */
    volatile unsigned int TIMER_STATUS;                 /**< 0x9C Timer General Status */
    volatile unsigned int COUNTER_CURR_VAL;             /**< 0xA0 Counter Current Count Value */
    volatile unsigned int RESERVED[(0x100-0xA4)/4];     /**< 0xA4-0xFF */
} FSYNCTMREGS, *PFSYNCTMREGS;

typedef struct tRADTMREGS
{
    volatile unsigned int INTRPTS_CNTRL;                /**< 0x00 Interrupt Control */
    volatile unsigned int CPU_INTRPTS_CNTRL[4];         /**< 0x04 CPU0-CPU3 Interrupt Control */
    volatile unsigned int TIMER_INTRPTS_STATUS;         /**< 0x14 Timer Interrupts Status */
    volatile unsigned int CHIP_INTRPTS_STATUS;          /**< 0x18 Chip Interrupts Status */
    volatile unsigned int SLOT_INTRPTS_STATUS;          /**< 0x1C Slot Interrupts Status */
    volatile unsigned int FRAME_INTRPTS_STATUS;         /**< 0x20 Frame Interrupts Status */
    volatile unsigned int CHIP_ERROR_STATUS;            /**< 0x24 Chip Error Status */
    volatile unsigned int SLOT_ERROR_STATUS;            /**< 0x28 Slot Error Status */
    volatile unsigned int FRAME_ERROR_STATUS;           /**< 0x2C Frame Error Status */
    volatile unsigned int SUPER_FRAME_INTRPTS_STATUS;   /**< 0x30 Super Frame Interrupts Status */
    volatile unsigned int SUPER_FRAME_ERROR_STATUS;     /**< 0x34 Super Frame Error Status */
    volatile unsigned int RESERVED[2];                  /**< 0x38-0x3F */
    FSYNCTMREGS FSyncTimers[NUM_FSYNC_TIMERS];
} RADTMREGS, *PRADTMREGS;

typedef struct tESYNCTMREGS
{
    volatile unsigned int INTRPTS_CNTRL;                /**< 0x00 Interrupt Control */
    volatile unsigned int CPU_INTRPTS_CNTRL[4];         /**< 0x04 CPU0-CPU3 Interrupt Control */
    volatile unsigned int TIMER_INTRPTS_STATUS;         /**< 0x14 Timer Interrupts Status */
    volatile unsigned int CHIP_INTRPTS_STATUS;          /**< 0x18 Chip Interrupts Status */
    volatile unsigned int SLOT_INTRPTS_STATUS;          /**< 0x1C Slot Interrupts Status */
    volatile unsigned int FRAME_INTRPTS_STATUS;         /**< 0x20 Frame Interrupts Status */
    volatile unsigned int CHIP_ERROR_STATUS;            /**< 0x24 Chip Error Status */
    volatile unsigned int SLOT_ERROR_STATUS;            /**< 0x28 Slot Error Status */
    volatile unsigned int FRAME_ERROR_STATUS;           /**< 0x2C Frame Error Status */
    volatile unsigned int SUPER_FRAME_INTRPTS_STATUS;   /**< 0x30 Super Frame Interrupts Status */
    volatile unsigned int SUPER_FRAME_ERROR_STATUS;     /**< 0x34 Super Frame Error Status */
    volatile unsigned int RESERVED[2];                  /**< 0x38-0x3F */
    FSYNCTMREGS FSyncTimers;
} ESYNCTMREGS, *PESYNCTMREGS;

typedef struct tIQCONFIG
{
	volatile unsigned int IntEnable:1;
	volatile unsigned int OvfIntEnable:1;
	volatile unsigned int EnableCounter:1;
	volatile unsigned int EnableClearingCounters:1;
	volatile unsigned int res0:28;
	volatile unsigned int OvfInd:1;
	volatile unsigned int res1:31;
	volatile unsigned int TrCnt;
	volatile unsigned int TrCntMaxVal;
	volatile unsigned int IntTrigVal;
	volatile unsigned int IntTrigMask;
	volatile unsigned int ProgDelayCount:8;
	volatile unsigned int res2:24;
	volatile unsigned int CntInitVal;
} IQCONFIG, *PIQCONFIG;
typedef struct tFFIQCONFIG
{
	volatile unsigned int en_counter:7;
	volatile unsigned int res0:1;
	volatile unsigned int sel_radioclk:7;
	volatile unsigned int res1:1;
	volatile unsigned int fs2en_counter:7;
	volatile unsigned int res2:1;
	volatile unsigned int fs2init_counter:7;
	volatile unsigned int res3:1;
	volatile unsigned int ovrflw_indc_top:7;
	volatile unsigned int res4:25;
} FFIQCONFIG, *PFFIQCONFIG;

void RadTmInit(void);
void RadTmCleanup(void);
void RadTmRun(int FsyncTmId, int NumAntennas, int SamplingRate);
void RadTmIrqHandler(unsigned int *pTm0Count, unsigned int *pTm1Count);
void RadTmEnInt(int FsyncTmId);
void RadTmDisInt(int FsyncTmId);
void RadTmSetReg(volatile unsigned int *pReg, unsigned int Value);
void EsyncFsyncConfig(void);
void EsyncTest(void);
void EsyncHold(void);
//void *p2vconv(unsigned int paddr, unsigned int size);
unsigned int SysTimerGetTick(void);
PRADTMREGS get_rad_tm_regs(void);

#endif /* __RADTIMERS_H__ */

