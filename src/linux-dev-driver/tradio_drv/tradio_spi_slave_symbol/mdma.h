#define MDMA_BDESC_NUM  (4)
#define MAPEXPDMA_REGS_BASEADDR		(MAP_BASEADDR + MAPEXPDMA_REGS_OFFSET)
#define MAP_BASEADDR                    0xF3D40000  /**< MAP (FFT) base address*/
#define MAPEXPDMA_REGS_OFFSET           0x0000E400
struct DMA_CHAN_CTRL {
	volatile unsigned int Control;
	volatile unsigned int HeadFDesc;
	volatile unsigned int BurstLength;
	volatile unsigned int Reserved;
	volatile unsigned int DmaIrqEnable;
	volatile unsigned int IrqStatusClear;
	volatile unsigned int Reserved1[2];
	volatile unsigned int SoftReset;
	volatile unsigned int DiagRegs[55];
};
struct MDMA_CTRL_REGS {
	struct DMA_CHAN_CTRL DMAInCtrl;
	struct DMA_CHAN_CTRL DMAOutCtrl;
	struct DMA_CHAN_CTRL DMAFcomCtrl;
};
struct DMABDESC {
	volatile unsigned int BPtr;
	volatile unsigned int BCtrl;
};
struct MDMAHWCTX {
	// W1
	volatile unsigned int IRQEN:1; // 0
	volatile unsigned int W1_Unused1:31; // 31:1
	// W2
	volatile unsigned int UserDefined_0; // 31:0
	// W3
	volatile unsigned int M2DBdesc:8; // 7:0
	volatile unsigned int D2MBdesc:8; // 15:8
	volatile unsigned int UserDefined_1:15; // 30:16
	volatile unsigned int StatusComp:1; // 31
};
struct MDMA_FDESC {
	struct MDMA_FDESC * Next;
	struct MDMAHWCTX HwContext;
	struct DMABDESC IoBdesc[MDMA_BDESC_NUM];

	void (*CbDone) (void * ctx);
	void * CbContext;

	// Next Desc. Virtual address
	struct MDMA_FDESC * NextVirt;

	// to keep an alignment
	// on 16 bytes

	unsigned int Reserved_0;
	unsigned int Reserved_1;
};

