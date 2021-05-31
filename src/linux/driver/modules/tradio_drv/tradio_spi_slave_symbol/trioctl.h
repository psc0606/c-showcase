/*
 *  tradio.h - the header file with the ioctl definitions.
 *
 */

#ifndef __TRIOCTL_H__
#define __TRIOCTL_H__

#include <linux/ioctl.h>

#ifndef __SYNCNET_H__
#define __SYNCNET_H__
typedef struct tSYNCNET_CONFIG {
    volatile unsigned int soft_reset;                      //0x0
    volatile unsigned int control;                         //0x4
    volatile unsigned int reserved[2];                     //0x8, 0xC
    volatile unsigned int ul_acc_clear;                    //0x10
    volatile unsigned int ul_acc_maxcount;                 //0x14
    volatile unsigned int ul_cpdma_acc_status;             //0x18
    volatile unsigned int ul_rpdif_acc_status;             //0x1C
    volatile unsigned int ul_bufcount_select;              //0x20
    volatile unsigned int ul_bufcount_window;              //0x24
    volatile unsigned int reserved1[6];                    //0x28..0x3C
    volatile unsigned int dl_acc_clear;                    //0x40
    volatile unsigned int dl_acc_maxcount;                 //0x44
    volatile unsigned int dl_cpdma_acc_status;             //0x48
    volatile unsigned int dl_rpdif_acc_status;             //0x4C
    volatile unsigned int dl_bufcount_select;              //0x50
    volatile unsigned int dl_bufcount_window;              //0x54
    volatile unsigned int dl_req_trigby_arm;               //0x58
    volatile unsigned int reserved2;                       //0x5C
    volatile unsigned int total_acc_maxcount;              //0x60
    volatile unsigned int total_acc_count_stat;            //0x64
    volatile unsigned int ul_irq_enable;                   //0x68
    volatile unsigned int dl_irq_enable;                   //0x6C
    volatile unsigned int ul_irq_status;                   //0x70
    volatile unsigned int dl_irq_status;                   //0x74
    volatile unsigned int reserved3[2];                    //0x78,0x7C
    volatile unsigned int ceva2arm_req_select;             //0x80
    volatile unsigned int map_req_select;                  //0x84
    volatile unsigned int update_iqcount_select;           //0x88
    volatile unsigned int irq_ceva0_select;                //0x8C
    volatile unsigned int irq_ceva1_select;                //0x90
    volatile unsigned int tdone2arm_and_req_select0;       //0x94
    volatile unsigned int tdone2arm_and_req_select1;       //0x98
    volatile unsigned int tdone2arm_or_req_select0;        //0x9C
    volatile unsigned int tdone2arm_or_req_select1;        //0xA0
    volatile unsigned int req_proc2psc0_select;            //0xA4
    volatile unsigned int req_proc2rpd_select;             //0xA8
    volatile unsigned int req_proc2cort0_select;           //0xAC
    volatile unsigned int req_proc2map_select;             //0xB0
}SYNCNET_CONFIG, *PSYNCNET_CONFIG;
#endif

typedef struct tagRADCONF
{
    unsigned int RadId;
    unsigned int NumAntennas;
    unsigned int SamplingRate;
    unsigned int TxLoopback;
    unsigned int RxLoopback;
    unsigned int Band;
    unsigned int RadioMode;
    unsigned int MapSyncMode;

    unsigned int TxPlayBufUserVirtAddr;
    unsigned int TxPlayBufVirtAddr;
    unsigned int TxPlayBufPhysAddr;
    unsigned int TxPlayBufSize;

    unsigned int TxLogBufVirtAddr;//ltedl5mhziqbuf
    unsigned int TxLogBufPhysAddr;//从内核返�?    unsigned int TxLogBufSize;

    unsigned int RxLogBufVirtAddr;
    unsigned int RxLogBufPhysAddr;
    unsigned int RxLogBufSize;

    void        *pRadProfile;
    unsigned int MapBootReq;
    unsigned int MapBufHeap;
    unsigned int MapDlInBufPhysAddr[2];
    unsigned int MapUlOutBufPhysAddr[2];
    unsigned int RadTimerIrqTrigger;
    unsigned int RadFrameSyncEnable;
    unsigned int RouteTtiIrqToCeva;
	unsigned int TypeCluster;
} RADCONF, *PRADCONF;

typedef struct tagRADBETDESC
{
    unsigned int Antenna;
    unsigned int StartIdx;
    unsigned int Mask;
    unsigned int DataBuf;
    unsigned int DataBufSize;
    unsigned int RefBuf;
    unsigned int RefBufSize;
    unsigned int ErrCount;
} RADBETDESC, *PRADBETDESC;

typedef struct tagRAD_STATS {
    unsigned int    BufOffset; //structure is using as tRAD_USER_CTX also, so this parameter is allowing don't do additional data coping
    unsigned long   tick; //current tick value
    unsigned long   lost; //while the last incomming irq is in process the new incomming irq to be lost
    unsigned long   irq;  //number of incomming irq
} RAD_STATS, *PRAD_STATS;

/*
 * Set the message of the device driver
 */
#define TRAD_IOCTL_ENABLE               _IOW('r', 1, unsigned int)
#define TRAD_IOCTL_DISABLE              _IOW('r', 2, unsigned int)
#define TRAD_IOCTL_SET_RAD_CONFIG       _IOWR('r', 3, PRADCONF)

#define TRAD_IOCTL_JDMA_REGS		_IOW('r', 4, unsigned int)
#define TRAD_IOCTL_JESD_REGS		_IOW('r', 5, unsigned int)
#define TRAD_IOCTL_TM_REGS		_IOW('r', 6, unsigned int)
#define TRAD_IOCTL_ESYNC_REGS		_IOW('r', 7, unsigned int)
#define TRAD_IOCTL_ESYNC_REGS2		_IOW('r', 8, unsigned int)
#define TRAD_IOCTL_ESYNC_REGS3		_IOW('r', 9, unsigned int)
#define TRAD_IOCTL_ESYNC_REGS4		_IOW('r', 10, unsigned int)
#define TRAD_IOCTL_INIT_SYS		_IOW('r', 11, unsigned int) 
#define TRAD_IOCTL_CLOSE_SYS		_IOW('r', 12, unsigned int) 
#define TRAD_IOCTL_JDMA_CONTROL		_IOW('r', 13, unsigned int) 
#define TRAD_IOCTL_COPY_RX_BUF		_IOW('r', 14, unsigned int) 
#define TRAD_IOCTL_TM_REGS_WRITE	_IOW('r', 15, unsigned int)
#define TRAD_IOCTL_IQ_1_CTR_READ_REGS	_IOW('r', 16, unsigned int)
#define TRAD_IOCTL_IQ_4_CTR_READ_REGS	_IOW('r', 17, unsigned int)
#define TRAD_IOCTL_IQ_FF_CTR_READ_REGS	_IOW('r', 18, unsigned int)
#define TRAD_IOCTL_GET_SYNCNET_REGS	_IOW('r', 19, unsigned int)
#define TRAD_IOCTL_SET_SYNCNET_REGS	_IOW('r', 20, unsigned int)
#define TRAD_IOCTL_COPY_IQ_IRQ_BUF	_IOW('r', 21, unsigned int)
#define TRAD_IOCTL_COPY_JDMA_IRQ_BUF	_IOW('r', 22, unsigned int)
#define TRAD_IOCTL_TEST_MDMA		_IOW('r', 23, unsigned int)
#define TRAD_IOCTL_COPY_CELL_SEARCH_JDMA0_DATA_12 _IOW('r',24, unsigned int)
#define TRAD_IOCTL_COPY_CELL_SEARCH_JDMA1_DATA_12 _IOW('r',25, unsigned int)
#define TRAD_IOCTL_COPY_CELL_SEARCH_JDMA0_DATA_40 _IOW('r',26, unsigned int)
#define TRAD_IOCTL_COPY_CELL_SEARCH_JDMA1_DATA_40 _IOW('r',27, unsigned int)
#define TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF		_IOW('r',28, unsigned int)
#define TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF		_IOW('r',29, unsigned int)
#define TRAD_IOCTL_MASTER_JDMA0_RX_SAMPLING_10SUBFRAME_DATA	_IOW('r',30, unsigned int)
#define TRAD_IOCTL_MASTER_JDMA1_RX_SAMPLING_10SUBFRAME_DATA	_IOW('r',31, unsigned int)
#define TRAD_IOCTL_SLAVE_JDMA0_RX_SAMPLING_5SUBFRAME_DATA	_IOW('r',32, unsigned int)
#define TRAD_IOCTL_SLAVE_JDMA1_RX_SAMPLING_5SUBFRAME_DATA	_IOW('r',33, unsigned int)
#define TRAD_IOCTL_MASTER_MAP0_OUTBOUNT_DATA			_IOW('r',34, unsigned int)
#define TRAD_IOCTL_MASTER_MAP1_OUTBOUNT_DATA			_IOW('r',35, unsigned int)
#define TRAD_IOCTL_SLAVE_MAP0_OUTBOUNT_DATA			_IOW('r',36, unsigned int)
#define TRAD_IOCTL_SLAVE_MAP1_OUTBOUNT_DATA			_IOW('r',37, unsigned int)
#define TRAD_IOCTL_JDMA_UL_REGS					_IOW('r',38, unsigned int)
#define SWITCH_GPIO					_IOW('r',39, unsigned int)
#define WHEREAMI					_IOW('r',40, unsigned int)

#define LOG_TX0_ENABLED                 (1 << 0)    // TX0 log is enabled
#define LOG_TX1_ENABLED                 (1 << 1)    // TX1 log is enabled
#define LOG_RX0_ENABLED                 (1 << 2)    // RX0 log is enabled
#define LOG_RX1_ENABLED                 (1 << 3)    // RX1 log is enabled
#define LOG_TX_LOG_CONT                 (1 << 4)    // Continious Capturing
#define LOG_RX_LOG_CONT                 (1 << 5)    // Continious Capturing

#define RADMODE_PULSE                   0    // Pulse Radio Mode
#define RADMODE_LEVEL                   1    // Level Radio Mode

#define MAPSYNCMODE_OFF                 0    // MAP Sync Disabled
#define MAPSYNCMODE_4G                  1    // MAP Sync Enabled for 4G
#define MAPSYNCMODE_3G                  2    // MAP Sync Enabled for 3G

#define SAMPLING_RATE_5MHZ              7680
#define SAMPLING_RATE_10MHZ             15360
#define SAMPLING_RATE_20MHZ             30720

#define LTE_LCP_SYMLEN_10MHZ            1152 //1104 aligned up to 16 words boundary
#define LTE_SCP_SYMLEN_10MHZ            768  //1096 actually used as last, reduced size symbol

#define SYMLEN_3G_DL                    256
#define SYMLEN_3G_UL                    64

#define HEAPID_DDR                      0
#define HEAPID_CRAM                     1

#define PHY_COM_AREA                    0x60200000
#define MAP_IN_BUF(n)                   (PHY_COM_AREA + (n)*1200*2*14*sizeof(UINT32))
#define MAP_OUT_BUF(n)                  (PHY_COM_AREA + 0x42000 + (n)*1200*2*14*sizeof(UINT32)) //0x60242000
#define TX_LOG_BUF(n)                   (PHY_COM_AREA + 0x84000 + (n)*RX_LOG_BUFFER_SIZE_MSEC*30720*sizeof(UINT32)) // 0x60284000
#define RX_LOG_BUF(n)                   (PHY_COM_AREA + 0x84000 + 0x3C00000 + (n)*TX_LOG_BUFFER_SIZE_MSEC*30720*sizeof(UINT32)) // 0x63E84000

/*
 * The name of the device file
 */
#define DEVICE_NAME "tradio"

#endif
