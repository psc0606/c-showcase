/**
 * @file tradio.h
 *
 * @brief Minsdpeed Transcede 2200 (T2K) radio device driver (header file)
 *
 */

#ifndef __TRADIO_H__
#define __TRADIO_H__

#include "ttypes.h"
//#include "t2200-hal.h"


#define NUM_RADIO_AC    (2)
#define RAD_ID0         (0)
#define RAD_ID1         (1)


typedef struct tJDMADESC
{
    unsigned TrSize:14;
    unsigned TrCtrl:2;
    unsigned SymOffset:14;
    unsigned Res0:2;
} JDMADESC, *PJDMADESC;

typedef struct tJDMATEMPLATE
{
    volatile JDMADESC Desc[4];
} JDMATEMPLATE, *PJDMATEMPLATE;

typedef struct tTSLDESC
{
    unsigned TrSize:14;
    unsigned TrCtrl:2;
    unsigned Res0:1;
    unsigned SymOffset:14;
} TSLDESC, *PTSLDESC;

typedef struct tDMASYMCTRL
{
    VUINT32 TslEntry[4];
    VUINT32 BasePtr;
    VUINT32 CurSymBufPtr;
    VUINT32 ShortStride;
    VUINT32 LongStride;
    VUINT32 Res;
    VUINT32 IrqSync;
    VUINT32 Res1[2];
} DMASYMCTRL, *PDMASYMCTRL;

typedef struct tJDMAREGS
{
    VUINT32 TxControl;
    VUINT32 TxStatus;
    VUINT32 TxFiller;
    VUINT32 TxRes0;
    volatile JDMATEMPLATE TxTemplate0;
    volatile JDMATEMPLATE TxTemplate1;
    volatile JDMATEMPLATE TxTemplate2;
    volatile JDMATEMPLATE TxTemplate3;
    volatile DMASYMCTRL TxSymCtrl0;
    VUINT32 TxIrqEna;
    VUINT32 TxRes2;
    VUINT32 TxIrqStat;
    VUINT32 TxRes3;
    VUINT32 TxArbiterRoundRobin;
    VUINT32 TxArbiterPriority;
    VUINT32 TxAxiBurstLength;
    VUINT32 TxFifoDataAccessWidth;
    VUINT32 TxMCacheCtrl;
    VUINT32 TxFifoUnderrun;
    VUINT32 TxFifoClearUnderrun;
    VUINT32 TxRes4[21];

    VUINT32 RxControl;
    VUINT32 RxStatus;
    VUINT32 RxRes0[2];
    volatile JDMATEMPLATE RxTemplate0;
    volatile JDMATEMPLATE RxTemplate1;
    volatile JDMATEMPLATE RxTemplate2;
    volatile JDMATEMPLATE RxTemplate3;
    volatile DMASYMCTRL RxSymCtrl0;
    VUINT32 RxIrqEna;
    VUINT32 RxRes2;
    VUINT32 RxIrqStat;
    VUINT32 RxRes3;
    VUINT32 RxArbiterRoundRobin;
    VUINT32 RxArbiterPriority;
    VUINT32 RxAxiBurstLength;
    VUINT32 RxFifoDataAccessWidth;
    VUINT32 RxMCacheCtrl;
    VUINT32 RxFifoOverrun;
    VUINT32 RxFifoClearOverrun;
    VUINT32 RxRes4[985];
} JDMAREGS, *PJDMAREGS;

typedef struct tJDMA_MAP1
{
    volatile JDMATEMPLATE TxTemplate0;
    volatile JDMATEMPLATE TxTemplate1;
    volatile JDMATEMPLATE RxTemplate0;
    volatile JDMATEMPLATE RxTemplate1;
    volatile DMASYMCTRL TxSymCtrl0;
    volatile DMASYMCTRL RxSymCtrl0;
} JDMA_MAP1, *PJDMA_MAP1;

typedef struct tPJESDREGS
{
    VUINT32 Ad356Comp;
    VUINT32 DataDelayCtrl;
    VUINT32 JesdCust;
    VUINT32 JesdClockDiv;
    VUINT32 ClockSync;
    VUINT32 CtrlCurState;
    VUINT32 MultByteBufCtrl;
    VUINT32 MultByteBufStat;
    VUINT32 MultByteBurstLen;
    VUINT32 CtrlPlanMode;
    VUINT32 MultByteBuf;
    VUINT32 Res0[5];
    VUINT32 DrvOffsetPos;
    VUINT32 DrvOffsetNeg;
    VUINT32 SampleOffsetPos;
    VUINT32 SampleOffsetNeg; // 0x4c
    VUINT32 Res1[12];
    VUINT32 InterfaceEnable;
    VUINT32 Mode;
    VUINT32 NumAntennas;
    VUINT32 FrameSignalingCtrl;
    VUINT32 TxShiftWidth;
    VUINT32 RxShiftWidth;
    VUINT32 TxBurstStartLatency;
    VUINT32 TxBurstFinishLatency;
    VUINT32 RxBurstStartLatency;
    VUINT32 RxBurstFinishLatency;
    VUINT32 TxBurstLength;
    VUINT32 RxBurstLength;
    VUINT32 BurstToBurstLatency;
    VUINT32 RPDIFState;
    VUINT32 RPDIFCtrl;
    VUINT32 RPDIFDllCalibrationCtrl;
    VUINT32 RPDIFDllCalDoneStat;
    VUINT32 RPDIFDllManualCtrl;
    VUINT32 RPDIFDllExtDelayCtrl;
    VUINT32 RPDIFDllCalDelay;
    VUINT32 RPDIFFrameSyncCtrl;
    VUINT32 RPDIFSubFrameLength;
    VUINT32 RPDIFTxBurstCountStat;
    VUINT32 RPDIFRxBurstCountStat;
    VUINT32 RPDIFSubFrameCountStat;
    VUINT32 RPDIFIntEnable;
    VUINT32 RPDIFIntStatus;
    VUINT32 Res2[5];
    VUINT32 Status; // 0x100
    PVUINT32 pBufferAddr;
    PVUINT32 pCtrlBufferAddr;
    VUINT32 IntStatus;
    VUINT32 IntEnable;
} JESDREGS, *PJESDREGS;

typedef struct tRADCTX
{
    PJESDREGS       pJesdRegs;
    PJDMAREGS       pJdmaRegs;
    unsigned int *  piq_time_buf;
    unsigned int *  pjdma_time_buf;
    PUINT8          pTxDmaBuf;
    PUINT8          pTxDmaBufVirt;
    PUINT8          pRxDmaBuf;
    PUINT8          pRxDmaBufVirt;
    PUINT8          pRxDmaBuf_12ms;
    PUINT8          pRxDmaBufVirt_12ms;
    PUINT8          pTxIqBufVirt;
    UINT32          TxBasePtr;
    PUINT8          pTxIqBuf;
    PUINT32         pTrace;
    PUINT32         pTraceStart;
    PUINT32         pTraceEnd;
    UINT32          StartTick;
    UINT32          LastTxTick;
    UINT32          LastRxTick;
    PUINT8          pTxLogBufVirt;
    PUINT8          pTxLogBuf;
    UINT32          TxLogBufSize;
    UINT32          TxLogBufPos[NUM_RADIO_AC];
    PUINT8          pRxLogBufVirt;
    PUINT8          pRxLogBuf;
    UINT32          RxLogBufSize;
    UINT32          RxLogBufPos[NUM_RADIO_AC];
    UINT32          TxPlayBufSize;
    UINT32          TxPlayBufPos[NUM_RADIO_AC];
    UINT32          NumAntennas;
    UINT32          DmaIntSize;
    UINT32          DmaBufSize;
    UINT32          SamplingRate;
    UINT32          TxIntCnt[NUM_RADIO_AC];
    UINT32          TxUnderrunCnt[NUM_RADIO_AC];
    UINT32          RxIntCnt[NUM_RADIO_AC];
    UINT32          RxOverrunCnt[NUM_RADIO_AC];
    UINT32          MapUlInSyms;
    UINT32          MapUlOutSyms;
    UINT32          MapUlLogSyms;
    PMAPDMADESC     pMapUlInDmaDesc[NUM_RADIO_AC];
    PMAPDMADESC     pMapUlOutDmaDesc[NUM_RADIO_AC];
    PUINT8          pMapUlOutBufVirt[NUM_RADIO_AC];
    PUINT8          pMapUlOutBuf[NUM_RADIO_AC];
    UINT32          MapUlOutBufSize;
    UINT32          MapDlInSyms;
    UINT32          MapDlOutSyms;
    UINT32          MapDlLogSyms;
    PMAPDMADESC     pMapDlInDmaDesc[NUM_RADIO_AC];
    PMAPDMADESC     pMapDlOutDmaDesc[NUM_RADIO_AC];
    PUINT8          pMapDlInBufVirt[NUM_RADIO_AC];
    PUINT8          pMapDlInBuf[NUM_RADIO_AC];
    UINT32          MapDlInBufSize;
    UINT32          LastRxIntCnt;
    UINT32          RadioMode;
    UINT32          MapSyncMode;
    int             TxIntId;
    int             RxIntId;
    int             TxWaitFlag;
    int             RxWaitFlag;
    UINT32          LogControl;
    UINT32          RFSyncGpioMask;
    UINT32          SubFrameCount;
    DMABDESC        MdmaInBDesc[3*NUM_RADIO_AC];
    DMABDESC        MdmaOutBDesc[3*NUM_RADIO_AC];
    UINT32          BDescIdx;
    BOOL            TxLogControl[NUM_RADIO_AC];
    BOOL            RxLogControl[NUM_RADIO_AC];
    BOOL            RxLoopback;
    BOOL            MapBootReq;
    BOOL            MapBufHeap;
    UINT8           RadId;
    UINT8           RadOpen;
    UINT8           RadEnabled;
    BOOL            RadFrameSyncEnable;
    unsigned int    RadTimerIrqTrigger;
    unsigned int    RadTimerIrqCounter;
    unsigned int    RouteTtiIrqToCeva;
	unsigned int 	MasterOrSlave;
}RADCTX, *PRADCTX;

UINT32 MapLteDlInit(
    UINT32 MapId,
    BOOL BootReq,
    BOOL BypassMode,
    UINT32 TdoneId,
    UINT32 SamplingRate,
    UINT32 NumInSyms,
    PMAPDMADESC pInDmaDesc,
    PUINT8 pMapInBuf,
    UINT32 NumOutSyms,
    PMAPDMADESC pOutDmaDesc,
    PUINT8 pMapOutBuf,
    UINT32 NumLogSyms,
    PUINT8 pLogBuf);
UINT32 MapLteUlInit(
    UINT32 MapId,
    BOOL BootReq,
    UINT32 TdoneId,
    UINT32 SamplingRate,
    UINT32 NumInSyms,
    PMAPDMADESC pInDmaDesc,
    PUINT8 pMapInBuf,
    UINT32 NumOutSyms,
    PMAPDMADESC pOutDmaDesc,
    PUINT8 pMapOutBuf,
    UINT32 NumLogSyms,
    PUINT8 pLogBuf);

UINT32 Map3gDlInit(
    UINT32 MapId,
    BOOL BootReq,
    UINT32 TdoneId,
    UINT32 SamplingRate,
    UINT32 NumInSyms,
    PMAPDMADESC pInDmaDesc,
    PUINT8 pMapInBuf,
    UINT32 NumOutSyms,
    PMAPDMADESC pOutDmaDesc,
    PUINT8 pMapOutBuf,
    UINT32 NumLogSyms,
    PUINT8 pLogBuf);
UINT32 Map3gUlInit(
    UINT32 MapId,
    BOOL BootReq,
    UINT32 TdoneId,
    UINT32 SamplingRate,
    UINT32 NumInSyms,
    PMAPDMADESC pInDmaDesc,
    PUINT8 pMapInBuf,
    UINT32 NumOutSyms,
    PMAPDMADESC pOutDmaDesc,
    PUINT8 pMapOutBuf,
    UINT32 NumLogSyms,
    PUINT8 pLogBuf);

void MapDmaReset(void);

#endif
