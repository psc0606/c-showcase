/**
 * @file tradio.c
 *
 * @brief Minsdpeed Transcede 2200 (T2K) radio device driver
 *
 */

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irqreturn.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <mach/irqs.h>
#include <mach/sysheap.h>
#include <asm/timex.h>
#include <linux/jiffies.h>

#include "../tradio.h"
#include "../trioctl.h"
#include "../radtimers.h"
#include "../mdma.h"

#define CEVA_DL 0
#define CEVA_UL 1

#define USE_DLL_CAL
#define ARM_DBG
#define GETTICKS                        *(volatile unsigned *)0xFE050004

#define MDMA_CTRL_BASE_ADDR0 0xF4C80000
#define MDMA_CTRL_BASE_ADDR1 0xF4C90000

#define HAL_IRQ_RADTM_CHIP   (IRQ_GIC_START + 76)
#define HAL_IRQ_IQ_COUNTER   (IRQ_GIC_START + 69)
#define RX_BUF_COPY_LENGTH SAMPLING_RATE_20MHZ*8             
#define TRANSMIT_PER_SUBFRAME_20MHZ 		(15360 * 2 * 4)
#define TRANSMIT_PER_SYMBOL_20MHZ   		((2048+160)* 4)
#define TRAMSMIT_PER_SUBFRAME_MAP_20MHZ 	(1216 * 14 * 4)
#define TRAMSMIT_PER_SYMBOL_MAP_20MHZ 		(1216 * 4)

#define TRANSMIT_DATA_K	   100 
//ARM DEBUG
#define GCDEBUG0								0xf3000500
#define GCDEBUG2								0xf3000508
#define GCDEBUG3								0xf300050C
#define PINGPONGINDEX							0xF300060C
#define GXXU8FN									0xF3000608
/*Master Cluster JDMA0/1*/
#define CELL_SEARCH_JDMA0_BASEADDR				0x70E20000
#define CELL_SEARCH_JDMA1_BASEADDR				0x712D0000
#define MASTER_JDMA0_PING						0x71780000
//#define MASTER_JDMA0_PONG						0x7179e000
#define MASTER_JDMA0_PONG						0x71816000
#define MASTER_JDMA1_PING						0x718ac000
//#define MASTER_JDMA1_PONG						0x718ca000
#define MASTER_JDMA1_PONG						0x71942000
/*Master Cluster MAP0/1*/
#define MASTER_MAP0_PING						0xf3011400
#define MASTER_MAP0_PONG						0xf3032800
#define MASTER_MAP0_PING2						0xf3053c00
#define MASTER_MAP1_PING						0xf3021e00
#define MASTER_MAP1_PONG						0xf3043200
#define MASTER_MAP1_PING2						0xf3064600
/*Slave Cluster JDMA0/1*/
#define SLAVE_JDMA0_PING						0xf3066000
#define SLAVE_JDMA0_PONG						0xf306a500
#define SLAVE_JDMA1_PING						0xf3068280
#define SLAVE_JDMA1_PONG						0xf306c780
/*Slave Cluster MAP0/1*/
#define SLAVE_MAP0_PING							0xf3021c00
#define SLAVE_MAP0_PONG							0xf3043000
#define SLAVE_MAP1_PING							0xf3032600
#define SLAVE_MAP1_PONG							0xf3053a00
//catch data addr
#define JDMA_MAP0_TX_BUF_BASEADDR				0x50000000
#define JDMA_MAP1_TX_BUF_BASEADDR				(JDMA_MAP0_TX_BUF_BASEADDR + TRANSMIT_PER_SUBFRAME_20MHZ)						//0x5001e000
#define JDMA0_SAMPLING_10SUBFRAME_DATA_BASEADDR		(JDMA_MAP1_TX_BUF_BASEADDR + TRANSMIT_PER_SUBFRAME_20MHZ)					//0x5003c000
#define JDMA1_SAMPLING_10SUBFRAME_DATA_BASEADDR		(JDMA0_SAMPLING_10SUBFRAME_DATA_BASEADDR + JDMA_10MS_SUBFRAME_DATA_SIZE)	//0x50168000
#define MAP0_OUT_BOUND_10SUBFRAME_DATA_BASEADDR		(JDMA1_SAMPLING_10SUBFRAME_DATA_BASEADDR + JDMA_10MS_SUBFRAME_DATA_SIZE)	//0x50294000
#define MAP1_OUT_BOUND_10SUBFRAME_DATA_BASEADDR		(MAP0_OUT_BOUND_10SUBFRAME_DATA_BASEADDR + MAP_10MS_SUBFRAME_DATA_SIZE)		//0x5033A400-0x503E07FF
																																
//dingchen
//#define TEMP												0x71000000
//#define TEMP_SIZE										68096


// #define JDMA_MAP0_PING_BUF_SIZE					0x2280
// #define JDMA_MAP0_PONG_BUF_SIZE					0x2280
// #define JDMA_MAP1_PING_BUF_SIZE					TRANSMIT_PER_SUBFRAME_20MHZ
// #define JDMA_MAP1_PONG_BUF_SIZE					TRANSMIT_PER_SUBFRAME_20MHZ
#define JDMA_MAP0_TX_BUF_SIZE					(TRANSMIT_PER_SUBFRAME_20MHZ)
#define JDMA_MAP1_TX_BUF_SIZE					(TRANSMIT_PER_SUBFRAME_20MHZ)
#define JDMA_5MS_SUBFRAME_DATA_SIZE				(TRANSMIT_PER_SUBFRAME_20MHZ  * 5)
#define JDMA_10MS_SUBFRAME_DATA_SIZE			(TRANSMIT_PER_SUBFRAME_20MHZ  * 10)//0x12c000
#define MAP_10MS_SUBFRAME_DATA_SIZE				(TRAMSMIT_PER_SUBFRAME_MAP_20MHZ * 10)//0xA6400
#define CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE		(TRANSMIT_PER_SUBFRAME_20MHZ * 12)
#define CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE		(TRANSMIT_PER_SUBFRAME_20MHZ * 40)


#define SYS_CONFIG_NO_SECURE 0XF4C00000
#define IRQ_ARM_ACK_IQ	     (SYS_CONFIG_NO_SECURE + 0x128)
/*
#define CEVA_RESET 0xf0d00000
#define SET_INT0_REG 0xf0d20000
#define CEVACTRL(mapped_base, regaddr) *(mapped_base + ((regaddr) - CEVA_RESET) / 4)
*/

/*
 * IQ COUNTER
 */
#define IQCOUNTERS_BASEADDR 0xF3D30000 /**IQ Counters base address */
#define IQCOUNTER_ADDR(n) (IQCOUNTERS_BASEADDR + ((n)<<8))

/*
 * to read current timer
 */
#define TRANSCEDE_SCU_BASE		0xFFF00000
#define TRANSCEDE_GLOBAL_TIMER_BASE	(TRANSCEDE_SCU_BASE + 0x200)
//#define TRANSCEDE_LOW_TIMER_COUNTER	(TRANSCEDE_GLOBAL_TIMER_BASE + 0x00)
#define TRANSCEDE_LOW_TIMER_COUNTER_5	0XFE050004
//static volatile unsigned int *pCevaCtrlRegs;

static int tradio_major[NUM_RADIO_AC];      /* Major number assigned to our device driver */
static struct class *tradio_class;
static struct device *tradio_device[NUM_RADIO_AC];
static RADCTX RadDrvCtx[NUM_RADIO_AC];

static unsigned int cell_jdma0_data_baseaddr_virt = 0;
static unsigned int cell_jdma1_data_baseaddr_virt = 0;
static unsigned int jdma_map0_tx_buf_virt = 0;
static unsigned int jdma_map1_tx_buf_virt = 0;
static unsigned int jdma0_sampling_10ms_data_virt  = 0;
static unsigned int jdma1_sampling_10ms_data_virt  = 0;
static unsigned int map0_out_boundsampling_10ms_data_virt  = 0;
static unsigned int map1_out_boundsampling_10ms_data_virt  = 0;

static int RadDrvOpen(int RadId);
static int RadDrvJesdSetup(int RadId, int NumAntennas);
static void RadDrvJdmaSetup(UINT32 RadId, UINT32 NumAntennas);
static int RadDrvConfig(int RadId, PRADCONF pConfig);
static void RadDrvEnable(int RadId);
static void RadDrvDisable(int RadId);
static int RadDrvRelease(int RadId);

volatile struct MDMA_CTRL_REGS *pMdma_ctrl_regs0; 
volatile struct MDMA_CTRL_REGS *pMdma_ctrl_regs1; 
static struct MDMA_FDESC *pMdma_Desc_virt;
static struct MDMA_FDESC *pMdma_Desc_phys;
volatile PMAPEXPDMAREGS pmap_expdma_regs = NULL;

static int tradio0_open(struct inode *inode, struct file *file);
static int tradio1_open(struct inode *inode, struct file *file);
static int tradio0_release(struct inode *inode, struct file *file);
static int tradio1_release(struct inode *inode, struct file *file);
static long tradio0_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static long tradio1_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

static int __init tradio_init(void);
static void __exit tradio_exit(void);

static DECLARE_WAIT_QUEUE_HEAD(tradio0_tx_wait_queue);
static DECLARE_WAIT_QUEUE_HEAD(tradio0_rx_wait_queue);
static DECLARE_WAIT_QUEUE_HEAD(tradio1_tx_wait_queue);
static DECLARE_WAIT_QUEUE_HEAD(tradio1_rx_wait_queue);
static DECLARE_WAIT_QUEUE_HEAD(receive_40ms_data_queue);

static volatile RAD_STATS rad_stats[NUM_RADIO_AC];

static unsigned int *vir_addr;

static ssize_t tradio_read(struct file *filp, char __user *buf, size_t count, loff_t *f_ops) {
        //struct fpga_dev *dev = filp->private_data;
        unsigned long p = *f_ops;

        if(copy_to_user(buf, (void *)((char *)vir_addr + p), count))
                return -EFAULT;
        else {
              *f_ops += count;
        }

        return count;
}

static ssize_t tradio_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_ops)
{
       // struct fpga_dev *dev = filp->private_data;
        unsigned long p = *f_ops;

        if(copy_from_user((void *)((char *)vir_addr + p), buf, count))
                return -EFAULT;
        else
               *f_ops += count;

        return count;
}


static loff_t tradio_llseek(struct file *filp, loff_t offset, int orig)
{
        loff_t ret = 0;
        if(offset < 0)
        {
                return -EINVAL;
        }
        switch(orig){
        case 0:
		ret = offset;
		break;
	case 1:
		ret = filp->f_pos + offset;
		break;
	default:
		return -EINVAL;
	}

	filp->f_pos = ret;
	printk("llseek:%x\n",(unsigned int)filp->f_pos);
	return ret;
}

static struct file_operations tradio_fops[NUM_RADIO_AC] = {
    {
    .owner = THIS_MODULE,
    .open = tradio0_open,
    .release = tradio0_release,
    .read = tradio_read,
    .write = tradio_write,
    .llseek = tradio_llseek,
    .unlocked_ioctl = tradio0_ioctl,
    },
    {
    .open = tradio1_open,
    .release = tradio1_release,
    .read = tradio_read,
    .write = tradio_write,
    .llseek = tradio_llseek,
    .unlocked_ioctl = tradio1_ioctl,
    },
};



static int RadDrvOpen(int RadId)
{
    PRADCTX pRadDrvCtx;

    pRadDrvCtx = &RadDrvCtx[RadId];

    memset(pRadDrvCtx, 0, sizeof(RADCTX));

    if (RadId == RAD_ID0)
        REG32(TRANSCEDE_GPIO_BOOTSTRAP_OVERRIDE) = 0x10; // override Radio

    pRadDrvCtx->RadId = RadId;

    pRadDrvCtx->pJesdRegs = (PJESDREGS)ioremap_nocache(JESD_BASEADDR(RadId), 0x10000);
    pRadDrvCtx->pJdmaRegs = (PJDMAREGS)ioremap_nocache(JDMA_BASEADDR(RadId), 0x10000);
    /*
     * 几种模式下ＪＤＭＡ的输出数据地址
     */
    cell_jdma0_data_baseaddr_virt = (unsigned int)ioremap_nocache(CELL_SEARCH_JDMA0_BASEADDR,CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE);
    memset((unsigned char *)cell_jdma0_data_baseaddr_virt,0x0,CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE);
	
    cell_jdma1_data_baseaddr_virt = (unsigned int)ioremap_nocache(CELL_SEARCH_JDMA1_BASEADDR,CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE);
    memset((unsigned char *)cell_jdma1_data_baseaddr_virt,0x0,CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE);

    jdma_map0_tx_buf_virt  = (unsigned int)ioremap_nocache(JDMA_MAP0_TX_BUF_BASEADDR,JDMA_MAP0_TX_BUF_SIZE);
    memset((unsigned char *)jdma_map0_tx_buf_virt,0x0,JDMA_MAP0_TX_BUF_SIZE);

    jdma_map1_tx_buf_virt = (unsigned int)ioremap_nocache(JDMA_MAP1_TX_BUF_BASEADDR,JDMA_MAP1_TX_BUF_SIZE);
    memset((unsigned char *)jdma_map1_tx_buf_virt ,0x0,JDMA_MAP1_TX_BUF_SIZE);

    jdma0_sampling_10ms_data_virt = (unsigned int)ioremap_nocache(JDMA0_SAMPLING_10SUBFRAME_DATA_BASEADDR,JDMA_10MS_SUBFRAME_DATA_SIZE);
    memset((unsigned char *)jdma0_sampling_10ms_data_virt,0x0,JDMA_10MS_SUBFRAME_DATA_SIZE);

    jdma1_sampling_10ms_data_virt = (unsigned int)ioremap_nocache(JDMA1_SAMPLING_10SUBFRAME_DATA_BASEADDR,JDMA_10MS_SUBFRAME_DATA_SIZE);
    memset((unsigned char *)jdma1_sampling_10ms_data_virt,0x0,JDMA_10MS_SUBFRAME_DATA_SIZE);

    map0_out_boundsampling_10ms_data_virt = (unsigned int)ioremap_nocache(MAP0_OUT_BOUND_10SUBFRAME_DATA_BASEADDR,MAP_10MS_SUBFRAME_DATA_SIZE);
    memset((unsigned char *)map0_out_boundsampling_10ms_data_virt,0x0,MAP_10MS_SUBFRAME_DATA_SIZE);

    map1_out_boundsampling_10ms_data_virt  = (unsigned int)ioremap_nocache(MAP1_OUT_BOUND_10SUBFRAME_DATA_BASEADDR,MAP_10MS_SUBFRAME_DATA_SIZE);
    memset((unsigned char *)map1_out_boundsampling_10ms_data_virt ,0x0,MAP_10MS_SUBFRAME_DATA_SIZE);
	
    pRadDrvCtx->RadOpen = 1;

    return 0;
}

static int RadDrvJesdSetup(int RadId, int NumAntennas)
{
    PRADCTX pRadDrvCtx = &RadDrvCtx[RadId];
    PJESDREGS pJesdRegs;
    UINT32 AntMask;
    UINT32 CalDelay;
    UINT32 FirstTimeInit = FALSE;
    UINT32 wait_time;

    pJesdRegs = pRadDrvCtx->pJesdRegs;
    NumAntennas = 2;
    AntMask = (NumAntennas<<1) - 1;

    pJesdRegs->Mode = 0x0;
    pJesdRegs->InterfaceEnable = 1;
    udelay(100);
    pJesdRegs->InterfaceEnable = 0;
    udelay(100);

    if (RadId == RAD_ID0) { // CMOS
        pJesdRegs->Mode = 0x5B;
    }
    else
        pJesdRegs->Mode = 0x2B; // (1<<10) for IQ swapping

    pJesdRegs->FrameSignalingCtrl = 1<<2; // AD9361 Mode
    if (pRadDrvCtx->RadioMode == RADMODE_LEVEL) {
        pJesdRegs->FrameSignalingCtrl |= 1<<4; // Frame Signal Level Mode
    }
    pJesdRegs->TxBurstStartLatency = 7;
    pJesdRegs->TxBurstFinishLatency = 7;
    pJesdRegs->RxBurstStartLatency = 7;
    pJesdRegs->RxBurstFinishLatency = 7;
    pJesdRegs->BurstToBurstLatency = 100;
    pJesdRegs->TxBurstLength = 0;
    pJesdRegs->RxBurstLength = 0;
    pJesdRegs->NumAntennas = ((NumAntennas-1) << 8) | (AntMask<<4) | AntMask;
    pJesdRegs->TxShiftWidth = 4;
    pJesdRegs->RxShiftWidth = 4;
	//esync
    //pJesdRegs->RPDIFFrameSyncCtrl = 1;	//using frame sync
	//pJesdRegs->RPDIFSubFrameLength = 1; //subframe length
    // DLL calibration
#ifdef USE_DLL_CAL
    if ((pJesdRegs->RPDIFDllManualCtrl & 1) == 0) {
        pJesdRegs->RPDIFDllCalibrationCtrl = 1;
        wait_time = 0;
        while ((pJesdRegs->RPDIFDllCalDoneStat & 1) == 0 && wait_time < 1000000) {
            wait_time++;
        }
        if ((pJesdRegs->RPDIFDllCalDoneStat & 1) == 0)
            printk("Calibration timeout\n");

        if (pJesdRegs->RPDIFDllCalDelay & 1)
            printk("DLL calibration error\n");
        else
            printk("DLL delay value = %d\n", pJesdRegs->RPDIFDllCalDelay >> 1);

        CalDelay = pJesdRegs->RPDIFDllCalDelay >> 1;

        pJesdRegs->RPDIFDllManualCtrl = CalDelay | 1;

        FirstTimeInit = TRUE;
    }
#else
    FirstTimeInit = (pJesdRegs->RPDIFDllManualCtrl & 1) ? 0 : 1;
    CalDelay = 61;
    pJesdRegs->RPDIFDllManualCtrl = CalDelay;
#endif

    return FirstTimeInit;
}

static void RadDrvJdmaSetup(UINT32 RadId, UINT32 NumAntennas)
{
    PRADCTX pRadDrvCtx = &RadDrvCtx[RadId];
    PJDMAREGS pJdmaRegs;
    UINT32 TslEntry0,TslEntry1,TslEntry2;
    PDMASYMCTRL pTxSymDesc;
    PDMASYMCTRL pRxSymDesc;
    PJDMATEMPLATE pTxJdmaTemplate0;
    PJDMATEMPLATE pRxJdmaTemplate0;
    PJDMATEMPLATE pRxJdmaTemplate1;
    UINT32 LongCpLen, ShortCpLen, HalfSubframeLen, SymLen;
	UINT32 type_cluster;
    pJdmaRegs = pRadDrvCtx->pJdmaRegs;
	type_cluster = pRadDrvCtx->MasterOrSlave;

    /*
     * HalfSubframeLen 15360 :half of subframe
     */
    HalfSubframeLen = pRadDrvCtx->DmaIntSize/sizeof(UINT32);
    //if (HalfSubframeLen > 0x4000)
	HalfSubframeLen /= 2;
	SymLen     = HalfSubframeLen * 2 / 15;									//2048/1024/512
	LongCpLen  = HalfSubframeLen / 96;										//160 /80  /40
    ShortCpLen = (HalfSubframeLen / 320) * 3;								//144 /72  /36

    /*
     * JDMA MAP PORT1 基地址
     */
    pTxJdmaTemplate0 = (PJDMATEMPLATE)((UINT32)&pJdmaRegs->TxTemplate0 + 0x1000);
    pRxJdmaTemplate0 = (PJDMATEMPLATE)((UINT32)&pJdmaRegs->RxTemplate0 + 0x1000);
    pRxJdmaTemplate1 = (PJDMATEMPLATE)((UINT32)&pJdmaRegs->RxTemplate1 + 0x1000);

    /*
     * JDMA1 MAP PORT 0的Tx Template0,每个Tsl将Tx buf中的1个subframe发送出\E5\8E?     */
    pJdmaRegs->TxTemplate0.Desc[0].SymOffset = 0;
    pJdmaRegs->TxTemplate0.Desc[0].TrCtrl = 0;
    pJdmaRegs->TxTemplate0.Desc[0].TrSize = 15360;//HalfSubframeLen;
    pJdmaRegs->TxTemplate0.Desc[1].SymOffset = 15360;//HalfSubframeLen;
    pJdmaRegs->TxTemplate0.Desc[1].TrCtrl = 0;
    pJdmaRegs->TxTemplate0.Desc[1].TrSize = 15360;//HalfSubframeLen;

    /*
     * JDMA1 MAP PORT 0的Rx Template0,每个tsl跳过0.5subframe数据
     */
    pJdmaRegs->RxTemplate0.Desc[0].SymOffset = 0;
    pJdmaRegs->RxTemplate0.Desc[0].TrCtrl = 1;
    pJdmaRegs->RxTemplate0.Desc[0].TrSize = HalfSubframeLen;
    pJdmaRegs->RxTemplate0.Desc[1].SymOffset = 0;
    pJdmaRegs->RxTemplate0.Desc[1].TrCtrl = 1;
    pJdmaRegs->RxTemplate0.Desc[1].TrSize = 0;

    /*
     * JDMA1 MAP PORT 0的Rx Template1,每个Tsl\E5\B0?个subframe接收
     * 小区搜索阶段使用这个Template
     */
    pJdmaRegs->RxTemplate1.Desc[0].SymOffset = 0;
    pJdmaRegs->RxTemplate1.Desc[0].TrCtrl = 0;
    pJdmaRegs->RxTemplate1.Desc[0].TrSize = HalfSubframeLen;
    pJdmaRegs->RxTemplate1.Desc[1].SymOffset = HalfSubframeLen;
    pJdmaRegs->RxTemplate1.Desc[1].TrCtrl = 0;
    pJdmaRegs->RxTemplate1.Desc[1].TrSize = HalfSubframeLen;

    /*
     * 处理正常业务的ShareTemplate2~3,
     * ShareTemplate2 for longCp
     * ShareTemplate3 for shortCp
     */
    pJdmaRegs->RxTemplate2.Desc[0].SymOffset = 0;
    pJdmaRegs->RxTemplate2.Desc[0].TrCtrl = 0;
    pJdmaRegs->RxTemplate2.Desc[0].TrSize = SymLen;
    pJdmaRegs->RxTemplate2.Desc[1].SymOffset = SymLen;
    pJdmaRegs->RxTemplate2.Desc[1].TrCtrl = 0;
    pJdmaRegs->RxTemplate2.Desc[1].TrSize = LongCpLen;

    pJdmaRegs->RxTemplate3.Desc[0].SymOffset = 0;
    pJdmaRegs->RxTemplate3.Desc[0].TrCtrl = 0;
    pJdmaRegs->RxTemplate3.Desc[0].TrSize = SymLen;
    pJdmaRegs->RxTemplate3.Desc[1].SymOffset = SymLen;
    pJdmaRegs->RxTemplate3.Desc[1].TrCtrl = 0;
    pJdmaRegs->RxTemplate3.Desc[1].TrSize = ShortCpLen;

    /*
     * JDMA1 MAP PORT 1的Tx Template0,每个Tsl将Tx buf中的1个subframe发送出\E5\8E?     */
    pTxJdmaTemplate0->Desc[0].SymOffset = 0; 
    pTxJdmaTemplate0->Desc[0].TrCtrl = 0; 
    pTxJdmaTemplate0->Desc[0].TrSize = 15360;//HalfSubframeLen; 
    pTxJdmaTemplate0->Desc[1].SymOffset = 15360;//HalfSubframeLen; 
    pTxJdmaTemplate0->Desc[1].TrCtrl = 0; 
    pTxJdmaTemplate0->Desc[1].TrSize = 15360;//HalfSubframeLen; 

    /*
     * JDMA1 MAP PORT 1的Rx Template0,每个tsl跳过0.5subframe数据
     */
    pRxJdmaTemplate0->Desc[0].SymOffset = 0; 
    pRxJdmaTemplate0->Desc[0].TrCtrl = 1; 
    pRxJdmaTemplate0->Desc[0].TrSize = HalfSubframeLen; 
    pRxJdmaTemplate0->Desc[1].SymOffset = 0; 
    pRxJdmaTemplate0->Desc[1].TrCtrl = 1; 
    pRxJdmaTemplate0->Desc[1].TrSize = 0; 
	
	pRxJdmaTemplate1->Desc[0].SymOffset = 0;
    pRxJdmaTemplate1->Desc[0].TrCtrl = 0;
    pRxJdmaTemplate1->Desc[0].TrSize = HalfSubframeLen;
    pRxJdmaTemplate1->Desc[1].SymOffset = HalfSubframeLen;
    pRxJdmaTemplate1->Desc[1].TrCtrl = 0;
    pRxJdmaTemplate1->Desc[1].TrSize = HalfSubframeLen;

    /*
     * JDMA1 PORT 0 for DL
     */
    pTxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->TxSymCtrl0);
    pRxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0);

    TslEntry0 = (0<<7) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | 1; /*Use template 0,1 subframe,repeat*/
    pTxSymDesc->TslEntry[0] = TslEntry0;

    pTxSymDesc->LongStride = sizeof(UINT32)*2*15360;//HalfSubframeLen;/*1 subframe*/

    pTxSymDesc->BasePtr = (UINT32)JDMA_MAP0_TX_BUF_BASEADDR;
    printk("Tx Base Ptr is 0x%08x\n",pTxSymDesc->BasePtr);
	//现在不是skip
    TslEntry1 = (0<<7) | (0<<4) | (0<<3) | (0<<2) | (1<<1) | 0; /*Use template 0 skip,0.5 subframe*/
    TslEntry2 = (0<<7) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | 0; /*Use template 0 skip,0.5 subframe,repeat*/
    pRxSymDesc->TslEntry[0] =  (TslEntry2 << 8) | TslEntry1;
//dingchen
//type_cluster = 0;
	pRxSymDesc->ShortStride = (type_cluster == 1) ? ((SymLen + ShortCpLen) * 4) : 0;
    pRxSymDesc->LongStride = (type_cluster == 1) ? ((SymLen + LongCpLen) * 4) : (sizeof(UINT32)*HalfSubframeLen*2);/*1 subframe*/
    pRxSymDesc->BasePtr = (type_cluster == 1) ? (UINT32)SLAVE_JDMA0_PING : (UINT32)CELL_SEARCH_JDMA0_BASEADDR;
	
    // pRxSymDesc->ShortStride =  0;
    // pRxSymDesc->LongStride =  (sizeof(UINT32)*HalfSubframeLen*2);/*1 subframe*/
    // pRxSymDesc->BasePtr =  (UINT32)CELL_SEARCH_JDMA0_BASEADDR;  //bicl,20140214

    printk("Rx Base Ptr is 0x%08x\n",pRxSymDesc->BasePtr);

    /*
     * JDMA1 PORT 1 for UL
     */
    pTxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->TxSymCtrl0 + 1 * 0x1000);
    pRxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0 + 1 * 0x1000);

    TslEntry0 = (0<<7) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | 1; // Use template 0,1 subframe,repeat
    pTxSymDesc->TslEntry[0] = TslEntry0;

    pTxSymDesc->LongStride = sizeof(UINT32)*2*15360;//HalfSubframeLen;

    pTxSymDesc->BasePtr = (UINT32)JDMA_MAP1_TX_BUF_BASEADDR;
    printk("Tx Base Ptr is 0x%08x\n",pTxSymDesc->BasePtr);

    TslEntry1 = (0<<7) | (0<<4) | (0<<3) | (0<<2) | (1<<1) | 0; /*Use template 0 skip,0.5 subframe*/
    TslEntry2 = (0<<7) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | 0; /*Use template 0 skip,0.5 subframe,repeat*/
    pRxSymDesc->TslEntry[0] =  (TslEntry2 << 8) | TslEntry1;

    /*
     * 正常业务阶段的步\E9\95?     */
    pRxSymDesc->ShortStride = (type_cluster == 1) ? ((SymLen + ShortCpLen) * 4) : 0;
    pRxSymDesc->LongStride = (type_cluster == 1) ? ((SymLen + LongCpLen) * 4) : (sizeof(UINT32)*HalfSubframeLen*2);
    pRxSymDesc->BasePtr = (type_cluster == 1) ? (UINT32)SLAVE_JDMA1_PING : (UINT32)CELL_SEARCH_JDMA1_BASEADDR;
	
    // pRxSymDesc->ShortStride = 0;
    // pRxSymDesc->LongStride = (sizeof(UINT32)*HalfSubframeLen*2);
    // pRxSymDesc->BasePtr   = (UINT32)CELL_SEARCH_JDMA1_BASEADDR;  //bicl,20140214
    printk("Rx Base Ptr is 0x%08x\n",pRxSymDesc->BasePtr);

    pJdmaRegs->TxAxiBurstLength = 4;
    pJdmaRegs->RxAxiBurstLength = 4;
    printk("JDMA Burst Size: %d\n", pJdmaRegs->TxAxiBurstLength);
    printk("I am in : %d (0-Master, 1-Slave)\n",type_cluster);

    //pJdmaRegs->TxIrqEna = (pRadDrvCtx->NumAntennas<<1) - 1;
    //pJdmaRegs->RxIrqEna = (1<<1) - 1;
	
	//esync test
	//pJdmaRegs->RxIrqEna = 0x3;
	//pJdmaRegs->RxControl |= /*(1 << 8) |*/ 0x3;
}

static int RadDrvConfig(int RadId, PRADCONF pConfig)
{
    PRADCTX pRadDrvCtx;
    UINT32 FirstTimeInit;
    //int i, rc;

	//printk("MasterOrSlave is %d\n",pConfig->TypeCluster);
    if (RadId >= NUM_RADIO_AC)
        return -1;
    // support only 1 and 2 antennas
    if (pConfig->NumAntennas < 1 || pConfig->NumAntennas > 2)
        return -2;
    // support only 5, 10 and 20 Mhz
    if (pConfig->SamplingRate != 7680 && pConfig->SamplingRate != 15360 && pConfig->SamplingRate != 30720)
        return -3;

    pRadDrvCtx = &RadDrvCtx[RadId];
    pRadDrvCtx->SamplingRate = pConfig->SamplingRate;
    pRadDrvCtx->NumAntennas = pConfig->NumAntennas;
    pRadDrvCtx->RadioMode = pConfig->RadioMode;
	pRadDrvCtx->MasterOrSlave = pConfig->TypeCluster;
	//printk("MasterOrSlave is %d\n", pRadDrvCtx->MasterOrSlave);

    printk("Route TTI IRQ to ceva: %d\n", pRadDrvCtx->RouteTtiIrqToCeva);

    // initialize Tx and Rx JDMA buffers
    pRadDrvCtx->DmaIntSize = sizeof(UINT32)*pRadDrvCtx->SamplingRate; // interrupt ARM each 1 ms (LTE subframe)

    FirstTimeInit = RadDrvJesdSetup(RadId, pRadDrvCtx->NumAntennas);
    RadDrvJdmaSetup(RadId, pRadDrvCtx->NumAntennas);

    return 0;
}

static void RadDrvEnable(int RadId)
{
    PRADCTX pRadDrvCtx = &RadDrvCtx[RadId];
    PJESDREGS pJesdRegs;
    PJDMAREGS pJdmaRegs;
    //UINT32 i;
    volatile PSYNCNET_CONFIG pSyncnetConf;
    //UINT32 AntMask = (pRadDrvCtx->NumAntennas<<1) - 1;
    PIQCONFIG pIqCntCfg_ceva_1 = NULL;
    PIQCONFIG pIqCntCfg_arm_0 = NULL;
    unsigned int loval = 0;
    unsigned int hival = 0;
	
    unsigned long flags;
    spinlock_t mr_lock = __SPIN_LOCK_UNLOCKED();

    pJesdRegs = pRadDrvCtx->pJesdRegs;
    pJdmaRegs = pRadDrvCtx->pJdmaRegs;
    pRadDrvCtx->RadEnabled = 0;

    pIqCntCfg_ceva_1 = (PIQCONFIG)IQCOUNTER_ADDR(1);
    pIqCntCfg_arm_0 = (PIQCONFIG)IQCOUNTER_ADDR(4);
    loval = 0x1f;
    //hival = 0x77f;
		hival = (pRadDrvCtx->SamplingRate)/ (loval + 1) - 1;//\D3\EBREG32(RAD_CFG_BASEADDR + 0x184) = 1;//0\CF\E0\B9\D8
    /*
     * syncnet
     */
    pSyncnetConf = (PSYNCNET_CONFIG)SYNCNET_BASEADDR;
    printk("Setting syncnet registers...\n");

    pSyncnetConf->map_req_select = 0;
    // pSyncnetConf->ul_bufcount_select = 0x00001000; //select RPDIF0 for CMOS or RPDIF4 for LVDS interface
	pSyncnetConf->ul_bufcount_select = 0x20001000; //select RPDIF0 for CMOS or RPDIF4 for LVDS interface	//rpdif4 for UL0 rpdif5 for UL1
    pSyncnetConf->ul_acc_maxcount = 0x000f0000; //max RPDIF UL accum. capacity - should match to number of symbols in radio buffer
    pSyncnetConf->ul_bufcount_window = 0xffff0101;// setup window 2..ff for UL0 and UL1

    // pSyncnetConf->total_acc_maxcount = 0x00ff0000;
    pSyncnetConf->total_acc_maxcount = 0xffff0000;
    // pSyncnetConf->reserved1[4] = 0x00000001;
    pSyncnetConf->reserved1[4] = 0x00000011;
    // pSyncnetConf->irq_ceva0_select = 2 << 12;//4'h2: selects irq_tdone_fpdma_dl to irq_cv0[3]
    // pSyncnetConf->irq_ceva0_select = 2 << 8;

    pSyncnetConf->soft_reset = 1;
    pSyncnetConf->soft_reset = 0;

    pSyncnetConf->ul_acc_clear = 0xFFFF;//clear all accums
    pSyncnetConf->dl_acc_clear = 0xFFFF;//clear all accums
    pSyncnetConf->ul_acc_clear = 0x0;
    pSyncnetConf->dl_acc_clear = 0x0;


    /*
     * 配置ceva和arm的iqcounter
     */
    REG32(RAD_CFG_BASEADDR + 0x184) = 1;//0
    // Selects MCLK0 or MCLK1 as a clock source for IQ counters. If RADID ==0; means MCLK0; if RADID ==1, means MCLK1 is clock source
    REG32(RAD_CFG_BASEADDR + 0x180) &= ~(3 << 19); //Clear bit 20:19,
    REG32(RAD_CFG_BASEADDR + 0x180) |= 1 << (19 + RadId);
	//REG32(RAD_CFG_BASEADDR + 0x184) = 1;
	//REG32(RAD_CFG_BASEADDR + 0x180) = 1 << (19 + RadId);
	REG32(IQCOUNTER_ADDR(0xff))  = 0x0;
	
    pIqCntCfg_ceva_1->TrCntMaxVal = hival<<16 | loval;
    pIqCntCfg_ceva_1->IntTrigVal  = hival<<16 | loval;
    pIqCntCfg_ceva_1->IntTrigMask = 0;
    pIqCntCfg_ceva_1->TrCnt = 0;
    /* Offset is after how many counters you want to trigger an interrupt */
    pIqCntCfg_ceva_1->ProgDelayCount = 1;
    pIqCntCfg_ceva_1->IntEnable = 1;
    pIqCntCfg_ceva_1->EnableCounter = 1;

    pIqCntCfg_arm_0->TrCntMaxVal = hival<<16 | loval;
    pIqCntCfg_arm_0->IntTrigVal  =  hival<<16 | loval;
    pIqCntCfg_arm_0->IntTrigMask = 0;
    pIqCntCfg_arm_0->TrCnt = 0;
    /* Offset is after how many counters you want to trigger an interrupt */
    pIqCntCfg_arm_0->ProgDelayCount = 1;
    pIqCntCfg_arm_0->IntEnable = 1;
    pIqCntCfg_arm_0->EnableCounter = 1;

    // start critical section
    spin_lock_irqsave(&mr_lock, flags);
		
	//ceva上配置的iqcounter
	REG32(IQCOUNTER_ADDR(0xff))  = 0x121200; // enable two IQ counters
	
	//EsyncFsyncConfig();
    printk("Enabling Radio %d in pulse mode...\n", RadId);

    // end critical section
    spin_unlock_irqrestore(&mr_lock, flags);

    /*
     * 和射频部分ｇｐｉｏ相关，具体功能未知
     */
    //REG32(TRANSCEDE_GPIO_OE_REG) |= GPIO_0;
    //REG32(TRANSCEDE_GPIO_OUTPUT_REG) |= GPIO_0; //toggle radio state (move to ACTIVE state),这个GPIO的使能对Slave无效,被FPGA屏蔽\E6\8E?    REG32(TRANSCEDE_GPIO_OUTPUT_REG) &= ~GPIO_0;
    //REG32(TRANSCEDE_GPIO_OUTPUT_REG) &= ~GPIO_0;
	pJesdRegs->RPDIFFrameSyncCtrl = 1;
    //udelay(100);
    pJesdRegs->InterfaceEnable = 1;

    /////////////pJdmaRegs->TxControl |= 0x3; 
}

static void RadDrvDisable(int RadId)
{
    PRADCTX pRadDrvCtx = &RadDrvCtx[RadId];
    PJESDREGS pJesdRegs;
    PJDMAREGS pJdmaRegs;

    if (!pRadDrvCtx->RadEnabled)
        return;

    pJesdRegs = pRadDrvCtx->pJesdRegs;
    pJdmaRegs = pRadDrvCtx->pJdmaRegs;

    pJdmaRegs->TxIrqEna = 0;
    pJdmaRegs->RxIrqEna = 0;
    pJdmaRegs->TxControl = 0; // disable Tx DMA
    pJdmaRegs->RxControl = 0; // disable Rx DMA
    pJesdRegs->InterfaceEnable = 0;


    if (pRadDrvCtx->RadioMode == RADMODE_PULSE) {
        REG32(TRANSCEDE_GPIO_OUTPUT_REG) |= GPIO_0; //toggle radio state (move to ALERT state)
        REG32(TRANSCEDE_GPIO_OUTPUT_REG) &= ~GPIO_0;
    }
}

static int RadDrvRelease(int RadId)
{
    PRADCTX pRadDrvCtx = &RadDrvCtx[RadId];

    if (pRadDrvCtx->RadOpen) {
            pRadDrvCtx->pRxDmaBuf = 0;
        }

    iounmap(pRadDrvCtx->pJesdRegs);
    iounmap(pRadDrvCtx->pJdmaRegs);
    pRadDrvCtx->RadOpen = 0;


    return 0;
}


/* Called when module gets installed (insmod or modprobe)"
  */
static int __init tradio_init(void)
{
    int i;
    char devname[16];

    printk("init start\n");

    tradio_class = class_create(THIS_MODULE, "tradiodrv");
    if (IS_ERR(tradio_class)) {
        return PTR_ERR(tradio_class);
    }


    for (i = RAD_ID0; i < NUM_RADIO_AC; i++) {
        sprintf(devname, "%s%d", DEVICE_NAME, i);

        tradio_major[i] = register_chrdev(0, devname, &tradio_fops[i]);

        if (tradio_major[i] < 0) {
            printk ("Registering %s device failed with %d\n", devname, tradio_major[i]);
            return tradio_major[i];
        }

        tradio_device[i] = device_create(tradio_class, NULL, MKDEV(tradio_major[i], 0), NULL, devname);
        if (IS_ERR(tradio_device[i])) {
           class_destroy(tradio_class);
           unregister_chrdev(tradio_major[i], devname);
           return PTR_ERR(tradio_device[i]);
        }

    }
    memset((void *)&rad_stats[i], 0, sizeof(rad_stats[i]));

    printk("%s device has been successfully initialized. Dynamic major is %d\n", devname, tradio_major[i]);

	RadTmInit();
	
	EsyncFsyncConfig();
	
	REG32(TRANSCEDE_GPIO_OE_REG) |= GPIO_0; // initialize radio control signal

    pMdma_ctrl_regs0 = (volatile struct MDMA_CTRL_REGS *)ioremap_nocache(MDMA_CTRL_BASE_ADDR0,0x10000);
    pMdma_ctrl_regs1 = (volatile struct MDMA_CTRL_REGS *)ioremap_nocache(MDMA_CTRL_BASE_ADDR1,0x10000);
    pMdma_Desc_virt = (struct MDMA_FDESC *)ddr_heap_alloc(1024); 
    pMdma_Desc_phys = (struct MDMA_FDESC *)ddr_heap_virt_to_phys((unsigned int)pMdma_Desc_virt);

    return 0;
}

/* Called when module gets removed (rmmod)"
  */
static void __exit tradio_exit(void)
{
    int i;
    char devname[16];

    for (i = RAD_ID0; i < NUM_RADIO_AC; i++) {
        sprintf(devname, "%s%d", DEVICE_NAME, i);

        printk(KERN_INFO "%s-%d stats:\n", DEVICE_NAME, i);
        printk(KERN_INFO "  - irq number:              %d\n", (int)rad_stats[i].irq);
        printk(KERN_INFO "  - lost irqs:               %d\n", (int)rad_stats[i].lost);

        device_destroy(tradio_class, MKDEV(tradio_major[i], 0));
        /* Unregister the device */
        unregister_chrdev(tradio_major[i], devname);
    }
    class_destroy(tradio_class);
}


/* Called when a process tries to open the device file, like open() or "cat /dev/tradio0/1"
  */
static int tradio0_open(struct inode *inode, struct file *file)
{
//   printk("tradio0 device open\n");

   return RadDrvOpen(RAD_ID0);
}

static int tradio1_open(struct inode *inode, struct file *file)
{

//RADTIMERS map
//vir_addr = (unsigned int*)ioremap_nocache(RADTIMER_BASE_ADDR, RADTIMER_MAP_SIZE);
//printk("vir_addr:0x%x\n", vir_addr);
//RADGROUP map
vir_addr = (unsigned int*)ioremap_nocache(RADGROUP_BASE_ADDR, RADGROUP_MAP_SIZE);
printk("vir_addr:0x%08x\n", (unsigned int)vir_addr);
   //return RadDrvOpen(RAD_ID1);
   return 0;
}


/* Called when a process closes the device file */
static int tradio0_release(struct inode *inode, struct file *file)
{

   return RadDrvRelease(RAD_ID0);
}

static int tradio1_release(struct inode *inode, struct file *file)
{
//RADTIMER
   iounmap(vir_addr);
   //return RadDrvRelease(RAD_ID1);
   return 0;
}

int tradio_ioctl(int DevId, unsigned int cmd, unsigned long arg)
{
    PRADCTX pRadDrvCtx = &RadDrvCtx[1];
    unsigned int jdma_handle;
    volatile PSYNCNET_CONFIG pSyncnetConf;
    //unsigned int write_syncnet_regs[2];
    //unsigned int ceva_curent_irq;
    //unsigned int start_copy_csbp;
    unsigned int symbol_number_int_subframe;
    PJESDREGS pJesdRegs;
    PESYNCTMREGS pESyncTmRegs;
    PRADTMREGS pRadTmRegs;
    PJDMAREGS pJdmaRegs;
    JDMA_MAP1 jdma_map1_regs;
    PDMASYMCTRL pRxSymDesc;
    PIQCONFIG pIqCntCfg_ceva_1 = NULL;
    PIQCONFIG pIqCntCfg_arm_0 = NULL;
    PFFIQCONFIG pFFIqCntCfg = NULL;
    unsigned int mdma_recv_addr;
//    unsigned int *iq_addr;
    int i;
    int rc = 0;
	unsigned int idx = 0;
    RADCONF RadConf;
    volatile unsigned int tick1,tick2 = 0;
	unsigned int LongCpLen, ShortCpLen, SymLen, SubframeLen, MapSymLen, MapSubFrmLen;
	UINT32 type_cluster;
	type_cluster = pRadDrvCtx->MasterOrSlave;
	SubframeLen = pRadDrvCtx->SamplingRate;				//30720/15360/7680
	SymLen     = SubframeLen / 15;								//2048/1024/512
	LongCpLen  = SubframeLen / 192;								//160 /80  /40
    ShortCpLen = (SubframeLen / 640) * 3;					//144 /72  /36
	MapSymLen = (((LongCpLen / 40 * 300) / 64 +1) * 64) * 4;
	MapSubFrmLen = MapSymLen * 14;

    pJesdRegs = pRadDrvCtx->pJesdRegs;
    pJdmaRegs = pRadDrvCtx->pJdmaRegs;
    pSyncnetConf = (PSYNCNET_CONFIG)SYNCNET_BASEADDR;
	pRadTmRegs = (PRADTMREGS)(0xf5e320c0);
	pESyncTmRegs = (PESYNCTMREGS)(0xf5e380c0);

    pIqCntCfg_ceva_1 = (PIQCONFIG)IQCOUNTER_ADDR(1);
    pIqCntCfg_arm_0 = (PIQCONFIG)IQCOUNTER_ADDR(4);
    pFFIqCntCfg = (PFFIQCONFIG)IQCOUNTER_ADDR(0xff);

    switch (cmd) {
    case WHEREAMI:
	    if (copy_to_user((char * __user)arg, &type_cluster, sizeof(char)))
			return -EFAULT;
    break;
    case TRAD_IOCTL_ENABLE:
        RadDrvEnable(DevId);
    break;
	case TRAD_IOCTL_JDMA_REGS:
	    if (copy_to_user((PJDMAREGS __user)arg, pJdmaRegs, sizeof(JDMAREGS)))
			return -EFAULT;
    break;
	case TRAD_IOCTL_JDMA_UL_REGS:
	    memcpy((PJDMATEMPLATE)&(jdma_map1_regs.TxTemplate0),(PJDMATEMPLATE)((UINT32)&pJdmaRegs->TxTemplate0 + 0x1000),sizeof(JDMATEMPLATE));
	    memcpy((PJDMATEMPLATE)&(jdma_map1_regs.TxTemplate1),(PJDMATEMPLATE)((UINT32)&pJdmaRegs->TxTemplate1 + 0x1000),sizeof(JDMATEMPLATE));
	    memcpy((PJDMATEMPLATE)&(jdma_map1_regs.RxTemplate0),(PJDMATEMPLATE)((UINT32)&pJdmaRegs->RxTemplate0 + 0x1000),sizeof(JDMATEMPLATE));
	    memcpy((PJDMATEMPLATE)&(jdma_map1_regs.RxTemplate1),(PJDMATEMPLATE)((UINT32)&pJdmaRegs->RxTemplate1 + 0x1000),sizeof(JDMATEMPLATE));
	    memcpy((PDMASYMCTRL)&(jdma_map1_regs.TxSymCtrl0),(PDMASYMCTRL)((UINT32)&pJdmaRegs->TxSymCtrl0 + 0x1000),sizeof(DMASYMCTRL));
	    memcpy((PDMASYMCTRL)&(jdma_map1_regs.RxSymCtrl0),(PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0 + 0x1000),sizeof(DMASYMCTRL));
	    if (copy_to_user((PJDMA_MAP1 __user)arg,(PJDMA_MAP1)&jdma_map1_regs, sizeof(JDMA_MAP1)))
			return -EFAULT;
    break;
	case TRAD_IOCTL_JESD_REGS:
	    if (copy_to_user((PJESDREGS __user)arg, pJesdRegs, sizeof(JESDREGS)))
			return -EFAULT;
	break;
	case TRAD_IOCTL_TM_REGS:
	    if (copy_to_user((PRADTMREGS __user)arg, pRadTmRegs, sizeof(RADTMREGS)))
			return -EFAULT;
    break;
	case TRAD_IOCTL_ESYNC_REGS:
	    if (copy_to_user((PESYNCTMREGS __user)arg, pESyncTmRegs, sizeof(ESYNCTMREGS)))
			return -EFAULT;
    break;
	case TRAD_IOCTL_ESYNC_REGS2:
		EsyncFsyncConfig();
    break;
	case TRAD_IOCTL_ESYNC_REGS3:
		EsyncTest();
    break;
	case TRAD_IOCTL_ESYNC_REGS4:
			//EsyncHold();
			while(0x01100010 != pIqCntCfg_arm_0->TrCnt);
			tick1 = SysTimerGetTick();
			tick2 = tick1;
			while(((tick2-tick1) < 100)) {
				while((pIqCntCfg_arm_0->TrCnt&0xffff0010) != (0x01100010&0xffff0010));
				tick2 = SysTimerGetTick();
				//printk("stuck in ");
				}
			printk("arm cycle time is %d * 4(38 23 board) or 5(52 board)ns\n", tick2-tick1);
    break;
    case TRAD_IOCTL_INIT_SYS:
	    RadDrvOpen(RAD_ID1);
    break;
    case TRAD_IOCTL_CLOSE_SYS:
	    RadDrvRelease(RAD_ID1);
    break;
    case TRAD_IOCTL_DISABLE:
        RadDrvDisable(DevId);
    break;
    case  TRAD_IOCTL_SET_RAD_CONFIG:
        if (copy_from_user(&RadConf, (PRADCONF)arg, sizeof(RADCONF)))
			return -EFAULT;
        RadDrvConfig(DevId, &RadConf);
        if (copy_to_user((PRADCONF)arg, &RadConf, sizeof(RADCONF)))
            return -EFAULT;
	break;
	case TRAD_IOCTL_IQ_FF_CTR_READ_REGS:
	    if (copy_to_user((PFFIQCONFIG __user *)arg,pFFIqCntCfg,sizeof(FFIQCONFIG)))
			return -EFAULT;
	break;
	case TRAD_IOCTL_IQ_1_CTR_READ_REGS:
	    if (copy_to_user((PIQCONFIG __user *)arg,pIqCntCfg_ceva_1, sizeof(IQCONFIG)))
			return -EFAULT;
    break;
	case TRAD_IOCTL_IQ_4_CTR_READ_REGS:
	    if (copy_to_user((PIQCONFIG __user *)arg,pIqCntCfg_arm_0, sizeof(IQCONFIG)))
			return -EFAULT;
    break;
	case TRAD_IOCTL_GET_SYNCNET_REGS:
	    if (copy_to_user((PSYNCNET_CONFIG __user *)arg,pSyncnetConf,sizeof(SYNCNET_CONFIG)))
			return -EFAULT;
    break;
    case  TRAD_IOCTL_JDMA_CONTROL:
        if (copy_from_user(&jdma_handle, (unsigned int *)arg, sizeof(unsigned int)))
		   return -EFAULT;
	    
	    printk("jdma handle is %d\n",jdma_handle);

	    switch (jdma_handle) {
	    case 0:
		    pJdmaRegs->TxControl |= 0x3; 
		    printk("txcontrol enable\n");
	    break;
	    case 1:
		    pJdmaRegs->TxControl &= 0xfffffffc; 
		    printk("txcontrol disable\n");
	    break;
	    case 2:
		    pJdmaRegs->RxControl |= 0x3;
		    printk("rxcontrol enable\n");
	    break;
	    case 3:
		    pJdmaRegs->RxControl &= 0xfffffffc; 
		    printk("rxcontrol disable\n");
	    break;
	    }
    break;
    case TRAD_IOCTL_COPY_CELL_SEARCH_JDMA0_DATA_12:
	    if (copy_to_user((void __user *)arg,(PUINT8)(cell_jdma0_data_baseaddr_virt),CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE))
		   return -EFAULT;
    break;
    case TRAD_IOCTL_COPY_CELL_SEARCH_JDMA1_DATA_12:
	    if (copy_to_user((void __user *)arg,(PUINT8)(cell_jdma1_data_baseaddr_virt),CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE))
		   return -EFAULT;
    break;
    case TRAD_IOCTL_COPY_CELL_SEARCH_JDMA0_DATA_40:
	    if (copy_to_user((void __user *)arg,(PUINT8)(cell_jdma0_data_baseaddr_virt),CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE))
		   return -EFAULT;
    break;
    case TRAD_IOCTL_COPY_CELL_SEARCH_JDMA1_DATA_40:
	    if (copy_to_user((void __user *)arg,(PUINT8)(cell_jdma1_data_baseaddr_virt),CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE))
		   return -EFAULT;
    break;
    case  TRAD_IOCTL_MASTER_JDMA0_RX_SAMPLING_10SUBFRAME_DATA:
	pRxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0);
	mdma_recv_addr = JDMA0_SAMPLING_10SUBFRAME_DATA_BASEADDR;

	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);
	for(i = 0; i < 10; i++) {
		while (pRxSymDesc->BasePtr != ((9 == i)? MASTER_JDMA0_PING:(MASTER_JDMA0_PING + (i + 1) * TRANSMIT_PER_SUBFRAME_20MHZ))) {
			udelay(1);
		}
		tick1 = SysTimerGetTick();
		printk("%d:%d * 4(5) ms\n",i,tick1-tick2);		
		tick2 = SysTimerGetTick();
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_JDMA0_PING + i * TRANSMIT_PER_SUBFRAME_20MHZ);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;

		pMdma_Desc_virt->IoBdesc[0].BCtrl = SubframeLen * 4;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = SubframeLen * 4;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		if(0 == i%2)
			pMdma_ctrl_regs0->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;//\D3\C3ͬһ\B8\F6mdma\D3п\C9\C4ܻ\E1\D3г\E5ͻ\A1\A3ץ\B2\BB\B5\BD\CA\FD\BE\DD
		else
			pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;

		mdma_recv_addr += SubframeLen * 4;
	//printk("This is in JDMA0 Master.ADDR:%x\n",pMdma_Desc_virt->IoBdesc[0].BPtr);
	}
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)jdma0_sampling_10ms_data_virt,JDMA_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
	break;
    case  TRAD_IOCTL_MASTER_JDMA1_RX_SAMPLING_10SUBFRAME_DATA:

	pRxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0 + 1 * 0x1000);
	mdma_recv_addr = JDMA1_SAMPLING_10SUBFRAME_DATA_BASEADDR;

	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);
	for(i = 0; i < 10; i++) {
		while (pRxSymDesc->BasePtr != ((9 == i)? MASTER_JDMA1_PING:(MASTER_JDMA1_PING + (i + 1) * TRANSMIT_PER_SUBFRAME_20MHZ))) {
			udelay(1);
		}
		tick1 = SysTimerGetTick();
		printk("SubFrame %d:%d * 4(5) ms\n",i,tick1-tick2);		
		tick2 = SysTimerGetTick();
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_JDMA1_PING + i * TRANSMIT_PER_SUBFRAME_20MHZ);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;

		pMdma_Desc_virt->IoBdesc[0].BCtrl = SubframeLen * 4;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = SubframeLen * 4;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		if(0 == i%2)
			pMdma_ctrl_regs0->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;//\D3\C3ͬһ\B8\F6mdma\D3п\C9\C4ܻ\E1\D3г\E5ͻ\A1\A3ץ\B2\BB\B5\BD\CA\FD\BE\DD
		else
			pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;

		mdma_recv_addr += SubframeLen * 4;
	//printk("This is in JDMA1 Master.ADDR:%x\n",pMdma_Desc_virt->IoBdesc[0].BPtr);
	}
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)jdma1_sampling_10ms_data_virt,JDMA_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
	break;
    case  TRAD_IOCTL_SLAVE_JDMA0_RX_SAMPLING_5SUBFRAME_DATA:	
	pRxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0);
	mdma_recv_addr = JDMA0_SAMPLING_10SUBFRAME_DATA_BASEADDR;

	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);
	idx = (*(unsigned int *)GCDEBUG2);
	while (0 == ((*(unsigned int *)GCDEBUG0) - idx)%2) udelay(1);
	while (1 == ((*(unsigned int *)GCDEBUG0) - idx)%2) udelay(1);

	symbol_number_int_subframe = 0;

	for (i = 0;i < 70;i++) {									//10ms 14 * 10 / 2 = 70

		while (pRxSymDesc->CurSymBufPtr != SLAVE_JDMA0_PONG) {
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_JDMA0_PING);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		if (0 == symbol_number_int_subframe) {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + LongCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + LongCpLen) * 4;
		} else {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + ShortCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + ShortCpLen) * 4;
		}

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		if (0 == symbol_number_int_subframe) {
			mdma_recv_addr += (SymLen + LongCpLen) * 4;
		} else  {
			mdma_recv_addr += (SymLen + ShortCpLen) * 4;
		}
		
		symbol_number_int_subframe++;

		while (pRxSymDesc->CurSymBufPtr != SLAVE_JDMA0_PING) {
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_JDMA0_PONG);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		if (7 == symbol_number_int_subframe) {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + LongCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + LongCpLen) * 4;
		} else {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + ShortCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + ShortCpLen) * 4;
		}

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;

		if (7 == symbol_number_int_subframe) {
			mdma_recv_addr += (SymLen + LongCpLen) * 4;
		} else
			mdma_recv_addr += (SymLen + ShortCpLen) * 4;
			
		symbol_number_int_subframe++;
		if (14 == symbol_number_int_subframe)
			symbol_number_int_subframe = 0;
	}	printk("This is in JDMA0 Slave.ADDR:%x\n",pMdma_Desc_virt->IoBdesc[0].BPtr);
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)jdma0_sampling_10ms_data_virt,JDMA_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
	break;
    case  TRAD_IOCTL_SLAVE_JDMA1_RX_SAMPLING_5SUBFRAME_DATA:	
    pRxSymDesc = (PDMASYMCTRL)((UINT32)&pJdmaRegs->RxSymCtrl0 + 1 * 0x1000);
	mdma_recv_addr = JDMA1_SAMPLING_10SUBFRAME_DATA_BASEADDR;

	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);
	idx = (*(unsigned int *)GCDEBUG2);
	while (0 == ((*(unsigned int *)GCDEBUG0) - idx)%2) udelay(1);
	while (1 == ((*(unsigned int *)GCDEBUG0) - idx)%2) udelay(1);
	
	symbol_number_int_subframe = 0;
	for (i = 0;i < 70;i++) {

		while (pRxSymDesc->CurSymBufPtr != SLAVE_JDMA1_PONG) {
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_JDMA1_PING);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		if (0 == symbol_number_int_subframe) {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + LongCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + LongCpLen) * 4;
		} else {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + ShortCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + ShortCpLen) * 4;
		}

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		if (0 == symbol_number_int_subframe) {
			mdma_recv_addr += (SymLen + LongCpLen) * 4;
		} else  {
			mdma_recv_addr += (SymLen + ShortCpLen) * 4;
		}
		
		symbol_number_int_subframe++;

		while (pRxSymDesc->CurSymBufPtr != SLAVE_JDMA1_PING) {
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_JDMA1_PONG);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		if (7 == symbol_number_int_subframe) {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + LongCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + LongCpLen) * 4;
		} else {
			pMdma_Desc_virt->IoBdesc[0].BCtrl = (SymLen + ShortCpLen) * 4;
			pMdma_Desc_virt->IoBdesc[1].BCtrl = (SymLen + ShortCpLen) * 4;
		}

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;

		if (7 == symbol_number_int_subframe) {
			mdma_recv_addr += (SymLen + LongCpLen) * 4;
		} else
			mdma_recv_addr += (SymLen + ShortCpLen) * 4;
			
		symbol_number_int_subframe++;
		if (14 == symbol_number_int_subframe)
			symbol_number_int_subframe = 0;
	}	printk("This is in JDMA1 Slave.ADDR:%x\n",pMdma_Desc_virt->IoBdesc[0].BPtr);
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)jdma1_sampling_10ms_data_virt,JDMA_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
    break;
    case  TRAD_IOCTL_MASTER_MAP0_OUTBOUNT_DATA:	
	mdma_recv_addr = MAP0_OUT_BOUND_10SUBFRAME_DATA_BASEADDR;
	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);

	while (!((0 == (*(unsigned int *)PINGPONGINDEX)) && (0 == (*(unsigned int *)GXXU8FN))))
		udelay(1);
		
	for (i = 0;i < 4;i++) {
		while (0 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_MAP0_PING);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
		
		if(3 == i)
			break;
			
		while (1 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}
	
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_MAP0_PONG);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;	
		
		while (2 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}
	
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_MAP0_PING2);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
	}
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)map0_out_boundsampling_10ms_data_virt,MAP_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
    break;
    case  TRAD_IOCTL_MASTER_MAP1_OUTBOUNT_DATA:	
	mdma_recv_addr = MAP1_OUT_BOUND_10SUBFRAME_DATA_BASEADDR;
	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);

	while (!((0 == (*(unsigned int *)PINGPONGINDEX)) && (0 == (*(unsigned int *)GXXU8FN))))
		udelay(1);
		
	for (i = 0;i < 4;i++) {
		while (0 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_MAP1_PING);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
		
		if(3 == i)
			break;
		
		while (1 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}
	
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_MAP1_PONG);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;	
		
		while (2 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}
	
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(MASTER_MAP1_PING2);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
	}
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)map1_out_boundsampling_10ms_data_virt,MAP_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
    break;
    case  TRAD_IOCTL_SLAVE_MAP0_OUTBOUNT_DATA:	
	mdma_recv_addr = MAP0_OUT_BOUND_10SUBFRAME_DATA_BASEADDR;

	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);

	while (!((0 == (*(unsigned int *)PINGPONGINDEX)) && (0 == (*(unsigned int *)GXXU8FN))))
		udelay(1);
	for (i = 0;i < 5;i++) {

		while (0 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_MAP0_PING);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
		
		while (1 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}
	
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_MAP0_PONG);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
	}
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)map0_out_boundsampling_10ms_data_virt,MAP_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
    break;
    case  TRAD_IOCTL_SLAVE_MAP1_OUTBOUNT_DATA:	
	mdma_recv_addr = MAP1_OUT_BOUND_10SUBFRAME_DATA_BASEADDR;

	while ((*(unsigned int *)GCDEBUG3) != 1)
		udelay(1);

	while (!((0 == (*(unsigned int *)PINGPONGINDEX)) && (0 == (*(unsigned int *)GXXU8FN))))
		udelay(1);
	for (i = 0;i < 5;i++) {

		while (0 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}
	
		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_MAP1_PING);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
		
		while (1 != (*(unsigned int *)PINGPONGINDEX))
		{
		    udelay(1);
		}

		pMdma_Desc_virt->IoBdesc[0].BPtr = (unsigned int)(SLAVE_MAP1_PONG);
		pMdma_Desc_virt->IoBdesc[1].BPtr = (unsigned int)mdma_recv_addr;
		pMdma_Desc_virt->Next = 0;
		pMdma_Desc_virt->IoBdesc[0].BCtrl = MapSubFrmLen;
		pMdma_Desc_virt->IoBdesc[1].BCtrl = MapSubFrmLen;

		pMdma_Desc_virt->HwContext.M2DBdesc = 1;
		pMdma_Desc_virt->HwContext.D2MBdesc = 1;
		pMdma_ctrl_regs1->DMAFcomCtrl.HeadFDesc = (unsigned int)pMdma_Desc_phys;
		mdma_recv_addr += MapSubFrmLen;
	}
	if (copy_to_user((PIQCONFIG __user *)arg,(unsigned int *)map1_out_boundsampling_10ms_data_virt,MAP_10MS_SUBFRAME_DATA_SIZE))
			return -EFAULT;
    break;
    case  TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF:
            if (copy_from_user((PUINT8)jdma_map0_tx_buf_virt,(PUINT8)arg,JDMA_MAP0_TX_BUF_SIZE))
            //if (copy_from_user((PUINT8)0xf3060000,(PUINT8)arg,JDMA_MAP0_TX_BUF_SIZE))
               return -EFAULT;
    break;
    case  TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF:
            if (copy_from_user((PUINT8)jdma_map1_tx_buf_virt,(PUINT8)arg,JDMA_MAP1_TX_BUF_SIZE))
            //if (copy_from_user((PUINT8)0xf3060000,(PUINT8)arg,JDMA_MAP1_TX_BUF_SIZE))
               return -EFAULT;
    break;
	case  SWITCH_GPIO:
		REG32(TRANSCEDE_GPIO_OE_REG) |= GPIO_0;
		REG32(TRANSCEDE_GPIO_OUTPUT_REG) |= GPIO_0; //toggle radio state (move to ACTIVE state),这个GPIO的使能对Slave无效,被FPGA屏蔽\E6\8E?    REG32(TRANSCEDE_GPIO_OUTPUT_REG) &= ~GPIO_0;
		REG32(TRANSCEDE_GPIO_OUTPUT_REG) &= ~GPIO_0;
    break;
    
    }
    return rc;
} 
static long tradio0_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return tradio_ioctl(RAD_ID0, cmd, arg);
}

static long tradio1_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    return tradio_ioctl(RAD_ID1, cmd, arg);
}

module_init(tradio_init);
module_exit(tradio_exit);

MODULE_AUTHOR("Mindspeed Technologies, Inc.");
MODULE_DESCRIPTION("Mindspeed T2200 DSP Driver");
MODULE_LICENSE("GPL");
//MODULE_VERSION(PKG_VERSION);
