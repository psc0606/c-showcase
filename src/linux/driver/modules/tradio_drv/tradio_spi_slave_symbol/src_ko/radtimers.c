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

#include "../radtimers.h"


static volatile PRADTMREGS pRadTmRegs;
static unsigned int SysBusClockFreq;
static volatile unsigned int *pTimerReg;

#if 0
void *p2vconv(unsigned int paddr, unsigned int size)
{
    unsigned int offset, mem_fd;
    void *vaddr;
    
    offset = paddr & 0xFFFF;
    paddr &= 0xFFFF0000;
    size += offset;
    
    //printf("map size=%x paddr=%x\n", size, paddr);
    
    mem_fd = open("/dev/mem", O_RDWR);
    vaddr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, paddr);
    if (vaddr == MAP_FAILED) {
        //fprintf(stderr, "Can't mmap /dev/mem at address 0x%08x, errno: %d (%s)\n", paddr, errno, strerror(errno));
		//printf("Can't mmap /dev/mem at address 0x%08x, errno: %d (%s)\n", paddr, errno, strerror(errno));
    }
    return (char *)vaddr + offset;
}
#endif

void EsyncFsyncConfig()
{
	static volatile PFSYNCTMREGS pRadEsync1TmRegs;    
	volatile PFSYNCTMREGS pFSyncTm;
	unsigned int ChipTermVal;
	unsigned int SlotTermVal;
	unsigned int FrameTermVal;
	unsigned int SuperFrameTermVal;
	unsigned int RadMode;
	unsigned int ClockSource;
	//unsigned int *pRadCfgRegs;
 
	pRadEsync1TmRegs  = (volatile PFSYNCTMREGS)ioremap_nocache(0xf5e38100, sizeof(FSYNCTMREGS)); 
//	pRadEsync1TmRegs  = (volatile PFSYNCTMREGS)ioremap_nocache(addr, sizeof(FSYNCTMREGS)); 

	RadMode = RADMOD_4G;
	ClockSource = FSYNC_RPDIF0_MCLK;

    pFSyncTm = pRadEsync1TmRegs;
	   
	//reg 0x180 and 0x184 will be programmed by CEVA1
	RadTmSetReg((volatile unsigned int *)(0xF5E380c0), 0x40); //INTR ENABLE

	// Reset Timer Clock Domain
	RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, 3);
	// Bring Timer Domain out of Reset 
	RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, 1);
	// Program Timer Registers

	if (RadMode == RADMOD_4G) {
		ChipTermVal = 20-1; //CHIP_SRATE-1;
		SlotTermVal = 15-1;// 10-1;
		FrameTermVal = 0x1fff-1; //0x1fff;
		SuperFrameTermVal = 0;// 0x7;
	} 
		ChipTermVal = 1; //CHIP_SRATE-1;
		SlotTermVal = 1;// 10-1;
		FrameTermVal = 2; //0x1fff;
		SuperFrameTermVal = 0;// 0x7;

	RadTmSetReg(&pFSyncTm->BASIC_CNTR_TC_VAL, 4); 
	RadTmSetReg(&pFSyncTm->BASIC_CNTR_INIT_VAL, 0);
	RadTmSetReg(&pFSyncTm->PRESCALE_DIV_CNTRL, 2); 

	RadTmSetReg(&pFSyncTm->CHIP_CNTR_TC_VAL_A, ChipTermVal);
	RadTmSetReg(&pFSyncTm->CHIP_CNTR_COMP_VAL_A, ChipTermVal);
	RadTmSetReg(&pFSyncTm->SLOT_CNTR_TC_VAL_A, SlotTermVal);
	RadTmSetReg(&pFSyncTm->SLOT_CNTR_COMP_VAL_A, SlotTermVal);
	RadTmSetReg(&pFSyncTm->FRAME_CNTR_TC_VAL_A, FrameTermVal);
	RadTmSetReg(&pFSyncTm->FRAME_CNTR_COMP_VAL_A, FrameTermVal);
	RadTmSetReg(&pFSyncTm->SUPER_FRAME_CNTR_TC_VAL_A, SuperFrameTermVal);
	RadTmSetReg(&pFSyncTm->SUPER_FRAME_CNTR_COMP_VAL_A, SuperFrameTermVal);

	RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, SET_FSYNC_CLK_SRC(ClockSource));

	RadTmSetReg(&pFSyncTm->INTRPTS_CNTRL, 0xF00);  
	RadTmSetReg(&pFSyncTm->TIMER_CNTRL, (1<<13) | (0<<10) | 1);
	//RadTmSetReg((volatile unsigned int *)(0xF5E380c0), 0x40); //INTR ENABLE
	//RadTmSetReg((volatile unsigned int *)(addr+0x0c0), 0x40); //INTR ENABLE
	//printk("INTR ENABLE 0X%08x", (volatile unsigned int *)(addr+0x0c0));

	printk("<-> done with esync1 programming\n");

}

void RadTmInit(void)
{
    volatile PFSYNCTMREGS pFSyncTm;
    
    SysBusClockFreq = SYS_REF_CLK * ((REG32(PLL_M_MSB(1)) << 8) | REG32(PLL_M_LSB(1))) / 5 / REG32(AXI_CLK_DIV_CNTRL);
    
    pRadTmRegs = (volatile PRADTMREGS)ioremap_nocache(RAD_TM_BASEADDR, sizeof(RADTMREGS));
	// pTimerReg = p2vconv(0xFE050004, 4);
	
	pTimerReg = (volatile unsigned int *)ioremap_nocache(0xFE050004, sizeof(unsigned int));;
    pFSyncTm = &pRadTmRegs->FSyncTimers[0];
    
    printk("SysBusClockFreq=%d PRESCALE_CLK_CNTRL=%x\n", SysBusClockFreq, pFSyncTm->PRESCALE_CLK_CNTRL);
}
void RadTmSetReg(volatile unsigned int *pReg, unsigned int Value)
{
    volatile unsigned int tick;
    unsigned int timeout;
    
    *pReg = Value;
    
    timeout = 3*(SysBusClockFreq / 4);
    tick = SysTimerGetTick();
    
    while ((SysTimerGetTick() - tick) < timeout);
}

unsigned int SysTimerGetTick(void)
{
    return *pTimerReg;
}
void EsyncTest()
{
	static volatile PFSYNCTMREGS pRadEsync1TmRegs;    
	volatile PFSYNCTMREGS pFSyncTm;
	//unsigned int *pRadCfgRegs;
 
	pRadEsync1TmRegs  = (volatile PFSYNCTMREGS)ioremap_nocache(0xf5e38100, sizeof(FSYNCTMREGS)); 

    pFSyncTm = pRadEsync1TmRegs;
	   
	// Reset Timer Clock Domain
	//RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, 3);
	// Bring Timer Domain out of Reset 
	//RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, 1);
	// Program Timer Registers

	//RadTmSetReg(&pFSyncTm->BASIC_CNTR_TC_VAL, 10); 
	//RadTmSetReg(&pFSyncTm->BASIC_CNTR_INIT_VAL, 0);
	//RadTmSetReg(&pFSyncTm->PRESCALE_DIV_CNTRL, 2); 

	//RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, SET_FSYNC_CLK_SRC(ClockSource));

	RadTmSetReg(&pFSyncTm->TIMER_CNTRL, (1<<13) | (0<<10) | 3);
	//RadTmSetReg(&pFSyncTm->INTRPTS_CNTRL, 0xF00);  
//	RadTmSetReg((volatile unsigned int *)(0xF5E380c0), 0x40); //INTR ENABLE
	//RadTmSetReg((volatile unsigned int *)(addr+0x0c0), 0x40); //INTR ENABLE

	printk("<-> done with esync1 change\n");

}

void EsyncHold()
{
	static volatile PFSYNCTMREGS pRadEsync1TmRegs;    
	volatile PFSYNCTMREGS pFSyncTm;
 
	pRadEsync1TmRegs  = (volatile PFSYNCTMREGS)ioremap_nocache(0xf5e38100, sizeof(FSYNCTMREGS)); 

    pFSyncTm = pRadEsync1TmRegs;
	   
	// Reset Timer Clock Domain
	//RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, 3);
	// Bring Timer Domain out of Reset 
	//RadTmSetReg(&pFSyncTm->PRESCALE_CLK_CNTRL, 1);
	// Program Timer Registers

	//RadTmSetReg(&pFSyncTm->BASIC_CNTR_TC_VAL, 10); 
	//RadTmSetReg(&pFSyncTm->BASIC_CNTR_INIT_VAL, 0);
	//RadTmSetReg(&pFSyncTm->PRESCALE_DIV_CNTRL, 2); 

	RadTmSetReg(&pFSyncTm->TIMER_CNTRL, (1<<13) | (1<<9) | (1<<10) | 3);
	//RadTmSetReg(&pFSyncTm->INTRPTS_CNTRL, 0xF00);  
//	RadTmSetReg((volatile unsigned int *)(0xF5E380c0), 0x40); //INTR ENABLE
	//RadTmSetReg((volatile unsigned int *)(addr+0x0c0), 0x40); //INTR ENABLE

	printk("<-> done with esync1 change\n");

}