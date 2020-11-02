#include "../mdmadrv.h"

int mdma_in_use = 0;

void mdma_set_data(struct MDMA_FDESC * pFDesc, void * pInData, unsigned long InDataSize, void * pOutData, unsigned long OutDataSize)
{
	if (pFDesc == NULL) {
		printf("NULL FDesc pointer\n");
		return;
	}

	pFDesc->IoBdesc[0].BPtr = (unsigned long)pInData;
	pFDesc->IoBdesc[0].BCtrl = InDataSize;

	pFDesc->IoBdesc[1].BPtr = (unsigned long)pOutData;
	pFDesc->IoBdesc[1].BCtrl = OutDataSize;

	pFDesc->HwContext.M2DBdesc = 1;
	pFDesc->HwContext.D2MBdesc = 1;
}

int mdma_run_noirq(volatile struct MDMA_CTRL_REGS *MdmaCtrlRegs,void* pphy)
{
	int rc = 0;

	MdmaCtrlRegs->DMAFcomCtrl.HeadFDesc = (unsigned long)pphy;
	mdma_in_use = 1;

	return rc;
}

int mdma_wait_completion(struct MDMA_FDESC * this)
{
	volatile unsigned long mdma_status_complete = this->HwContext.StatusComp;
	int cnt = 0;

	if (!mdma_in_use) {
		printf("MDMA if off");
		return 0;
	}

	while (mdma_status_complete != 1) {
		mdma_status_complete = this->HwContext.StatusComp;

		asm volatile ("mov r0, r0");
		asm volatile ("mov r0, r0");

		cnt++;

		if (cnt > MDMA_RUN_TIMEOUT) {
			printf("MDMA stuck");
		}
	}

	mdma_in_use = 0;

	return cnt;
}
