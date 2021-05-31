/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>

#include "../trioctl.h"
#include "../radtimers.h"
#include "../ad9361radio.h"

#include "../tradio.h"

#define TRANSMIT_PER_SUBFRAME_20MHZ						(15360 * 4 * 2)
#define MAP_OUT_PER_SUBFRAME_20MHZ						(1216 * 14 * 4)
#define JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE			(MAP_OUT_PER_SUBFRAME_20MHZ * 10)	 
#define CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE	(TRANSMIT_PER_SUBFRAME_20MHZ * 12)
#define CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE	(TRANSMIT_PER_SUBFRAME_20MHZ * 40)
#define JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE			(TRANSMIT_PER_SUBFRAME_20MHZ * 10)

// default configuration for radio
static const char *spi_device[2] =  {
    "/dev/spidev0.4",
    "/dev/spidev1.2"
};

static const char *rad_device[2] =  {
    "/dev/tradio0",
    "/dev/tradio1"
};

void print_jdma_rx_regs(PJDMAREGS);
void print_jdma_tx_regs(PJDMAREGS);
void print_jdma_map1_regs(PJDMA_MAP1);
void print_jesd_regs(PJESDREGS );
void print_iq_counter_regs(PIQCONFIG);
void print_iq_total_regs(PFFIQCONFIG);
void print_tm_regs(PRADTMREGS);
void print_esync_regs(PESYNCTMREGS);
int rw_regs(int);
int catch_data_master(int);
int catch_data_slave(int);
unsigned int print_syncnet_regs(PSYNCNET_CONFIG psyncnet_regs);
char char_to_hex(char *c);
int cfg9361_base_rx_freq(int fd_spi,double freq);
unsigned short str_to_hex(char *);
unsigned int str_to_hex32(char *str);

signed short vector_test_aligning[];
unsigned int vector_test_aligning_size = 30720 * 1;

extern unsigned int sin1mhziqnum;	//to tell you, 30720
extern signed short sin1mhziqbuf[];

extern unsigned int ltedlqam16iqnum; //614400; 
extern signed short ltedlqam16iqbuf[];

extern unsigned int ltedlqam64iqnum; //153600;
extern signed short ltedlqam64iqbuf[];

extern unsigned int ltedl20mhziqnum; //307200;
extern signed short ltedl20mhziqbuf[];

extern unsigned int trianglewaveiqnum; //30720
extern signed short trianglewaveiqbuf[];

extern signed short lteqpsk10mhziqbuf[]; //307200

int main(int argc, char *argv[])
{
	FILE *fprx;
	int ret = 0;
	int fd_spi;
	unsigned int jdma_handle;
	char jdma_tx_status;
	char jdma_rx_status;
	char is_loopback;
	int fd_rad;
	int i,j;
	char do_what;
	char whereami;
	int exit_flag = 0;
	short *recv_addr;
	
	JDMAREGS jdmaregs;
	JESDREGS jesdregs;
	JDMA_MAP1 jdma_map1_regs;
	RADTMREGS radtmregs;
	ESYNCTMREGS esynctmregs;
	IQCONFIG IqCntCfg;
	FFIQCONFIG FFIqCntCfg;
	SYNCNET_CONFIG syncnet_regs;

	fd_rad = open(rad_device[1], O_RDWR);
	fd_spi = open(spi_device[1], O_RDWR);
	
	jdma_tx_status = 0;
	jdma_rx_status = 0;
	is_loopback = Ad9361ReadReg(fd_spi, 0x3f5);
	
	ret = ioctl(fd_rad, WHEREAMI, &whereami);

	while (1) {
		printf("-------------------------------------U R in %s Cluster-----------------------------------\n",(whereami == 0) ? "Master":"Slave");
		printf("1:IQ Counter 1 REGS (CEVA)	    2:IQ Counter 4 REGS (ARM)	    3:IQ_Counter Top Cfg REGS\n");
		printf("4:SYNC NET REGS			    5:JDMA port 0 RX REGS   	    6:JDMA port 1 RX REGS\n");
		printf("7:JDMA port 0 TX REGS		    8:JESD REGS			    9:ESYNC REGS\n");
		printf("a:PRINT AD9361 REGS(Master Only)    b/c/d:ESYNC CONTROL(unused)     e:RW REGS**\033[40;31mcaution\033[0m**\n");
		printf("f:Catch Time/Freq Data via ARM	    g:SEND GPIO PULSE		    h:Load Tx Test Vector\n");
		if(is_loopback)
		    printf("i:LOOPBACK OFF			    ");
		else
		    printf("i:LOOPBACK ON			    ");
		if (jdma_tx_status)
			printf("j:JDMA Tx STOP		    ");
		else 
			printf("j:JDMA Tx START		    ");			
		if (jdma_rx_status)
			printf("k:JDMA Rx STOP\n");
		else 
			printf("k:JDMA Rx START\n");
		printf("0:QUIT\n");

		do_what = getchar();
		getchar();
		switch (do_what){
		case '1'://iqcounter 1 ceva
			ret = ioctl(fd_rad,TRAD_IOCTL_IQ_1_CTR_READ_REGS,&IqCntCfg);
			print_iq_counter_regs(&IqCntCfg);
		break;
		case '2'://iqcounter 4 arm
			ret = ioctl(fd_rad,TRAD_IOCTL_IQ_4_CTR_READ_REGS,&IqCntCfg);
			print_iq_counter_regs(&IqCntCfg);
		break;
		case '3'://iqcounter top
			ret = ioctl(fd_rad,TRAD_IOCTL_IQ_FF_CTR_READ_REGS,&FFIqCntCfg);
			print_iq_total_regs(&FFIqCntCfg);
		break;
		case '4'://sync net
			ret = ioctl(fd_rad,TRAD_IOCTL_GET_SYNCNET_REGS,&syncnet_regs);
			print_syncnet_regs(&syncnet_regs);
		break;
		case '5'://jdma port 0 rx
			ret = ioctl(fd_rad, TRAD_IOCTL_JDMA_REGS, &jdmaregs);
			print_jdma_rx_regs(&jdmaregs);
		break;
		case '6'://jdma port 1 rx
			ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_UL_REGS , &jdma_map1_regs);
			print_jdma_map1_regs(&jdma_map1_regs);
		break;
		case '7'://jdma port 0 tx
			ret = ioctl(fd_rad, TRAD_IOCTL_JDMA_REGS, &jdmaregs);
			print_jdma_tx_regs(&jdmaregs);
		break;
		case '8'://jesd
			ret = ioctl(fd_rad, TRAD_IOCTL_JESD_REGS, &jesdregs);
			print_jesd_regs(&jesdregs);
		break;
		case '9'://esync
			ret = ioctl(fd_rad, TRAD_IOCTL_ESYNC_REGS, &esynctmregs);
			print_esync_regs(&esynctmregs);
		break;
		case 'a'://print 9361 regs
			if(whereami == 0)
				Ad9361DumpRegs(fd_spi);
			else
				printf("U Must check it in Master Cluster!\n");
		break;
		case 'b'://esync
			ret = ioctl(fd_rad, TRAD_IOCTL_ESYNC_REGS2, &esynctmregs);
			printf("config\n");
			//print_esync_regs(&esynctmregs);
		break;
		case 'c'://esync 1
			ret = ioctl(fd_rad, TRAD_IOCTL_ESYNC_REGS3, &esynctmregs);
			printf("force to start\n");
			//print_esync_regs(&esynctmregs);
		break;
		case 'd'://esync 2
			ret = ioctl(fd_rad, TRAD_IOCTL_ESYNC_REGS4, &esynctmregs);
			printf("please quit and check iqcounter cycle with command [dmesg]\n");
		break;
		case 'g'://SWITCH_GPIO
			ret = ioctl(fd_rad, SWITCH_GPIO, &esynctmregs);
			printf("GPIO PULSE SEND\n");
		break;
		case 'e'://r/w regs
			ret = rw_regs(fd_rad);
		break;
		case 'f'://catch data
			if(whereami == 0)
				ret = catch_data_master(fd_rad);
			else
				ret = catch_data_slave(fd_rad);
		break;
		case 'h'://load tx_jdma
			radio_playback_signal(fd_rad);
			//ret = ioctl(fd_rad,TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF,vector_test_aligning);
			//ret = ioctl(fd_rad,TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF,vector_test_aligning);
		break;
		case 'i'://loopback
			if(is_loopback){
				//ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_CONTROL ,&jdma_handle);
				Ad9361WriteReg(fd_spi, 0x3f5, 0x00);
				Ad9361WriteReg(fd_spi, 0x02, 0x0);
				is_loopback = 0;
			}else{
				//ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_CONTROL ,&jdma_handle);
				Ad9361WriteReg(fd_spi, 0x3f5, 0x01);
				Ad9361WriteReg(fd_spi, 0x02, 0xde);
				is_loopback = 1;
			}
		break;
		case 'j'://tx_jdma_status
			if (jdma_tx_status){
				jdma_handle = 1;
				ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_CONTROL ,&jdma_handle);
				jdma_tx_status = 0;
			} else {
				jdma_handle = 0;
				ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_CONTROL ,&jdma_handle);
				jdma_tx_status = 1;
			}
		break;
		case 'k'://rx_jdma_status
			if (jdma_rx_status){
				jdma_handle = 3;
				ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_CONTROL ,&jdma_handle);
				jdma_rx_status = 0;
			} else {
				jdma_handle = 2;
				ret = ioctl(fd_rad,TRAD_IOCTL_JDMA_CONTROL ,&jdma_handle);
				jdma_rx_status = 1;
			}
		break;
		default:
			exit_flag = 1;
		//	return ret;
		break;
		}

		if (exit_flag == 1)
			break;
	}
	return ret;
}

void print_iq_total_regs(PFFIQCONFIG pIqCntCfg)
{
	printf("************************TOTAL iq counter register************************\n");
	printf("EnableCounter		    :0x%02x\n",pIqCntCfg->en_counter);
	printf("SEL_RADIOCLK		    :0x%02x\n",pIqCntCfg->sel_radioclk);
	printf("FS 2 EN COUNTER		    :0x%02x\n",pIqCntCfg->fs2en_counter);
	printf("FS 2 INIT COUNTER	    :0x%02x\n",pIqCntCfg->fs2init_counter);
	printf("OVER FLOW INDC TOP	    :0x%02x\n",pIqCntCfg->ovrflw_indc_top);
	printf("************************************************************************\n");
}
void print_iq_counter_regs(PIQCONFIG pIqCntCfg)
{
	printf("************************IQ Counter REGS************************\n");
	printf("iq counter register\n");
	printf("IntEnable		    :%u\n",pIqCntCfg->IntEnable);
	printf("OvfIntEnable		    :%u\n",pIqCntCfg->OvfIntEnable);
	printf("EnableCounter		    :%u\n",pIqCntCfg->EnableCounter);
	printf("EnableClearingCounters	    :%u\n",pIqCntCfg->EnableClearingCounters);
	//printf("res0:%u\n",pIqCntCfg->res0);
	printf("OvfInd			    :%u\n",pIqCntCfg->OvfInd);
	//printf("res1:%u\n",pIqCntCfg->res1);
	printf("TrCnt			    :0x%08x\n",pIqCntCfg->TrCnt);
	printf("TrCntMaxVal		    :0x%08x\n",pIqCntCfg->TrCntMaxVal);
	printf("IntTrigVal		    :0x%08x\n",pIqCntCfg->IntTrigVal);
	printf("IntTrigMask		    :%u\n",pIqCntCfg->IntTrigMask);
	printf("ProgDelayCount		    :%u\n",pIqCntCfg->ProgDelayCount);
	//printf("res2:%u\n",pIqCntCfg->res2);
	printf("CntInitVal		    :%u\n",pIqCntCfg->CntInitVal);
	printf("************************************************************************\n");
}

unsigned int print_syncnet_regs(PSYNCNET_CONFIG psyncnet_regs)
{
	printf("************************Sync Net REGS************************\n");
	printf("0.soft_reset			:0x%08x\n",psyncnet_regs->soft_reset);
	printf("1.control	    		:0x%08x\n",psyncnet_regs->control);
	printf("2.reserved[0]			:0x%08x\n",psyncnet_regs->reserved[0]);
	printf("3.reserved[1]			:0x%08x\n",psyncnet_regs->reserved[1]);
	printf("4.ul_acc_clear			:0x%08x\n",psyncnet_regs->ul_acc_clear);
	printf("5.ul_acc_maxcount		:0x%08x\n",psyncnet_regs->ul_acc_maxcount);
	printf("6.ul_cpdma_acc_status	    	:0x%08x\n",psyncnet_regs->ul_cpdma_acc_status);
	printf("7.ul_rpdif_acc_status		:0x%08x\n",psyncnet_regs->ul_rpdif_acc_status);
	printf("8.ul_bufcount_select		:0x%08x\n",psyncnet_regs->ul_bufcount_select);
	printf("9.ul_bufcount_window		:0x%08x\n",psyncnet_regs->ul_bufcount_window);
	//printf("reserved1:\n");
	//printf("0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n",psyncnet_regs->reserved1[0],psyncnet_regs->reserved1[1],psyncnet_regs->reserved1[2],psyncnet_regs->reserved1[3],psyncnet_regs->reserved1[4],psyncnet_regs->reserved1[5]);
	printf("16.dl_acc_clear			:0x%08x\n",psyncnet_regs->dl_acc_clear);
	printf("17.dl_acc_maxcount		:0x%08x\n",psyncnet_regs->dl_acc_maxcount);
	printf("18.dl_cpdma_acc_status		:0x%08x\n",psyncnet_regs->dl_cpdma_acc_status);
	printf("19.dl_rpdif_acc_status		:0x%08x\n",psyncnet_regs->dl_rpdif_acc_status);
	printf("20.dl_bufcount_select		:0x%08x\n",psyncnet_regs->dl_bufcount_select);
	printf("21.dl_bufcount_window		:0x%08x\n",psyncnet_regs->dl_bufcount_window);
	printf("22.dl_req_trigby_arm		:0x%08x\n",psyncnet_regs->dl_req_trigby_arm);
	//printf("23.reserved2:0x%08x\n",psyncnet_regs->reserved2);
	printf("24.total_acc_maxcount		:0x%08x\n",psyncnet_regs->total_acc_maxcount);
	printf("25.total_acc_count_stat		:0x%08x\n",psyncnet_regs->total_acc_count_stat);
	printf("26.ul_irq_enable		:0x%08x\n",psyncnet_regs->ul_irq_enable);
	printf("27.dl_irq_enable		:0x%08x\n",psyncnet_regs->dl_irq_enable);
	printf("28.ul_irq_status		:0x%08x\n",psyncnet_regs->ul_irq_status);
	printf("29.dl_irq_status		:0x%08x\n",psyncnet_regs->dl_irq_status);
	//printf("30.reserved3[0]:0x%08x\n",psyncnet_regs->reserved3[0]);
	//printf("31.reserved3[1]:0x%08x\n",psyncnet_regs->reserved3[1]);
	printf("32.ceva2arm_req_select		:0x%08x\n",psyncnet_regs->ceva2arm_req_select);
	printf("33.map_req_select		:0x%08x\n",psyncnet_regs->map_req_select);
	printf("34.update_iqcount_select	:0x%08x\n",psyncnet_regs->update_iqcount_select);
	printf("35.irq_ceva0_select		:0x%08x\n",psyncnet_regs->irq_ceva0_select);
	printf("36.irq_ceva1_select		:0x%08x\n",psyncnet_regs->irq_ceva1_select);
	printf("37.tdone2arm_and_req_select0	:0x%08x\n",psyncnet_regs->tdone2arm_and_req_select0);
	printf("38.tdone2arm_and_req_select1	:0x%08x\n",psyncnet_regs->tdone2arm_and_req_select1);
	printf("39.tdone2arm_or_req_select0	:0x%08x\n",psyncnet_regs->tdone2arm_or_req_select0);
	printf("40.tdone2arm_or_req_select1	:0x%08x\n",psyncnet_regs->tdone2arm_or_req_select1);
	printf("41.req_proc2psc0_select		:0x%08x\n",psyncnet_regs->req_proc2psc0_select);
	printf("42.req_proc2rpd_select		:0x%08x\n",psyncnet_regs->req_proc2rpd_select);
	printf("43.req_proc2cort0_select	:0x%08x\n",psyncnet_regs->req_proc2cort0_select);
	printf("44.req_proc2map_select		:0x%08x\n",psyncnet_regs->req_proc2map_select);
	printf("************************************************************************\n");
}

void print_jdma_rx_regs(PJDMAREGS pjdmaregs)
{
	JDMAREGS jdmaregs = *pjdmaregs;
	printf("************************JDMA PORT0 RX REGS*************************\n");
	printf("Rx_Control                  : 0x%08x\n",jdmaregs.RxControl);
	printf("Rx_Status                   : 0x%08x\n",jdmaregs.RxStatus);
	printf("Rx_Res0[1]                  : 0x%08x\n",jdmaregs.RxRes0[0]);
	printf("Rx_Res0[2]                  : 0x%08x\n",jdmaregs.RxRes0[1]);
	printf("RxTemplate0.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[0].TrSize);
	printf("RxTemplate0.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[0].TrCtrl);
	printf("RxTemplate0.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[0].SymOffset);
	//printf("RxTemplate0.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[0].Res0);
	printf("RxTemplate0.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[1].TrSize);
	printf("RxTemplate0.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[1].TrCtrl);
	printf("RxTemplate0.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[1].SymOffset);
	//printf("RxTemplate0.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[1].Res0);
	printf("RxTemplate0.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[2].TrSize);
	printf("RxTemplate0.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[2].TrCtrl);
	printf("RxTemplate0.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[2].SymOffset);
	//printf("RxTemplate0.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[2].Res0);
	printf("RxTemplate0.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[3].TrSize);
	printf("RxTemplate0.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[3].TrCtrl);
	printf("RxTemplate0.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[3].SymOffset);
	//printf("RxTemplate0.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[3].Res0);
	printf("RxTemplate1.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[0].TrSize);
	printf("RxTemplate1.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[0].TrCtrl);
	printf("RxTemplate1.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[0].SymOffset);
	//printf("RxTemplate1.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[0].Res0);
	printf("RxTemplate1.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[1].TrSize);
	printf("RxTemplate1.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[1].TrCtrl);
	printf("RxTemplate1.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[1].SymOffset);
	//printf("RxTemplate1.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[1].Res0);
	printf("RxTemplate1.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[2].TrSize);
	printf("RxTemplate1.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[2].TrCtrl);
	printf("RxTemplate1.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[2].SymOffset);
	//printf("RxTemplate1.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[2].Res0);
	printf("RxTemplate1.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[3].TrSize);
	printf("RxTemplate1.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[3].TrCtrl);
	printf("RxTemplate1.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[3].SymOffset);
	//printf("RxTemplate1.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[3].Res0);
	printf("RxTemplate2.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[0].TrSize);
	printf("RxTemplate2.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[0].TrCtrl);
	printf("RxTemplate2.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[0].SymOffset);
	//printf("RxTemplate2.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[0].Res0);
	printf("RxTemplate2.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[1].TrSize);
	printf("RxTemplate2.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[1].TrCtrl);
	printf("RxTemplate2.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[1].SymOffset);
	//printf("RxTemplate2.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[1].Res0);
	printf("RxTemplate2.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[2].TrSize);
	printf("RxTemplate2.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[2].TrCtrl);
	printf("RxTemplate2.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[2].SymOffset);
	//printf("RxTemplate2.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[2].Res0);
	printf("RxTemplate2.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[3].TrSize);
	printf("RxTemplate2.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[3].TrCtrl);
	printf("RxTemplate2.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[3].SymOffset);
	//printf("RxTemplate2.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[3].Res0);
	printf("RxTemplate3.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[0].TrSize);
	printf("RxTemplate3.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[0].TrCtrl);
	printf("RxTemplate3.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[0].SymOffset);
	//printf("RxTemplate3.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[0].Res0);
	printf("RxTemplate3.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[1].TrSize);
	printf("RxTemplate3.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[1].TrCtrl);
	printf("RxTemplate3.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[1].SymOffset);
	//printf("RxTemplate3.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[1].Res0);
	printf("RxTemplate3.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[2].TrSize);
	printf("RxTemplate3.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[2].TrCtrl);
	printf("RxTemplate3.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[2].SymOffset);
	//printf("RxTemplate3.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[2].Res0);
	printf("RxTemplate3.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[3].TrSize);
	printf("RxTemplate3.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[3].TrCtrl);
	printf("RxTemplate3.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[3].SymOffset);
	//printf("RxTemplate3.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[3].Res0);
	printf("RxSymCtrl0.TslEntry0        : 0x%08x\n",jdmaregs.RxSymCtrl0.TslEntry[0]);
	printf("RxSymCtrl0.TslEntry1        : 0x%08x\n",jdmaregs.RxSymCtrl0.TslEntry[1]);
	printf("RxSymCtrl0.TslEntry2        : 0x%08x\n",jdmaregs.RxSymCtrl0.TslEntry[2]);
	printf("RxSymCtrl0.TslEntry3        : 0x%08x\n",jdmaregs.RxSymCtrl0.TslEntry[3]);
	printf("RxSymCtrl0.BasePtr          : 0x%08x\n",jdmaregs.RxSymCtrl0.BasePtr);
	printf("RxSymCtrl0.CurSymBufPtr     : 0x%08x\n",jdmaregs.RxSymCtrl0.CurSymBufPtr);
	printf("RxSymCtrl0.ShortStride      : 0x%08x\n",jdmaregs.RxSymCtrl0.ShortStride);
	printf("RxSymCtrl0.LongStride       : 0x%08x\n",jdmaregs.RxSymCtrl0.LongStride);
	printf("RxSymCtrl0.Res              : 0x%08x\n",jdmaregs.RxSymCtrl0.Res);
	printf("RxSymCtrl0.IrqSync          : 0x%08x\n",jdmaregs.RxSymCtrl0.IrqSync);
	printf("RxSymCtrl0.Res1.0           : 0x%08x\n",jdmaregs.RxSymCtrl0.Res1[0]);
	printf("RxSymCtrl0.Res1.1           : 0x%08x\n",jdmaregs.RxSymCtrl0.Res1[1]);
	printf("RxIrqEna                    : 0x%08x\n",jdmaregs.RxIrqEna);
	printf("RxIrqStat                   : 0x%08x\n",jdmaregs.RxIrqStat);
	printf("RxRes3                      : 0x%08x\n",jdmaregs.RxRes3);
	printf("RxArbiterRoundRobin         : 0x%08x\n",jdmaregs.RxArbiterRoundRobin);
	printf("RxArbiterPriority           : 0x%08x\n",jdmaregs.RxArbiterPriority);
	printf("RxAxiBurstLength            : 0x%08x\n",jdmaregs.RxAxiBurstLength);
	printf("RxFifoDataAccessWidth       : 0x%08x\n",jdmaregs.RxFifoDataAccessWidth);
	printf("RxMCacheCtrl                : 0x%08x\n",jdmaregs.RxMCacheCtrl);
	printf("RxFifoUnderrun              : 0x%08x\n",jdmaregs.RxFifoOverrun);
	printf("RxFifoClearUnderrun         : 0x%08x\n",jdmaregs.RxFifoClearOverrun);
	printf("jdmaregs.RxRes4.[0-21]               : \n");
	printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.RxRes4[0],jdmaregs.RxRes4[1],jdmaregs.RxRes4[2],jdmaregs.RxRes4[3],jdmaregs.RxRes4[4],jdmaregs.RxRes4[5],jdmaregs.RxRes4[6]);
	printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.RxRes4[7],jdmaregs.RxRes4[8],jdmaregs.RxRes4[9],jdmaregs.RxRes4[10],jdmaregs.RxRes4[11],jdmaregs.RxRes4[12],jdmaregs.RxRes4[13]);
	printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.RxRes4[14],jdmaregs.RxRes4[15],jdmaregs.RxRes4[16],jdmaregs.RxRes4[17],jdmaregs.RxRes4[18],jdmaregs.RxRes4[19],jdmaregs.RxRes4[20]);
	printf("************************************************************************\n");
}
void print_jdma_tx_regs(PJDMAREGS pjdmaregs)
{
	JDMAREGS jdmaregs = *pjdmaregs;
	printf("************************JDMA PORT0 TX REGS************************\n");
	printf("Tx_Control                  : 0x%08x\n",jdmaregs.TxControl);
	printf("Tx_Status                   : 0x%08x\n",jdmaregs.TxStatus);
	printf("Tx_Filter                   : 0x%08x\n",jdmaregs.TxFiller);
	// printf("Tx_Res0                     : 0x%08x\n",jdmaregs.TxRes0);
	printf("TxTemplate0.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[0].TrSize);
	printf("TxTemplate0.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[0].TrCtrl);
	printf("TxTemplate0.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[0].SymOffset);
	// printf("TxTemplate0.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[0].Res0);
	printf("TxTemplate0.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[1].TrSize);
	printf("TxTemplate0.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[1].TrCtrl);
	printf("TxTemplate0.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[1].SymOffset);
	// printf("TxTemplate0.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[1].Res0);
	printf("TxTemplate0.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[2].TrSize);
	printf("TxTemplate0.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[2].TrCtrl);
	printf("TxTemplate0.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[2].SymOffset);
	// printf("TxTemplate0.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[2].Res0);
	printf("TxTemplate0.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[3].TrSize);
	printf("TxTemplate0.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[3].TrCtrl);
	printf("TxTemplate0.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[3].SymOffset);
	// printf("TxTemplate0.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[3].Res0);
	printf("TxTemplate1.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[0].TrSize);
	printf("TxTemplate1.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[0].TrCtrl);
	printf("TxTemplate1.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[0].SymOffset);
	// printf("TxTemplate1.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[0].Res0);
	printf("TxTemplate1.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[1].TrSize);
	printf("TxTemplate1.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[1].TrCtrl);
	printf("TxTemplate1.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[1].SymOffset);
	// printf("TxTemplate1.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[1].Res0);
	printf("TxTemplate1.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[2].TrSize);
	printf("TxTemplate1.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[2].TrCtrl);
	printf("TxTemplate1.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[2].SymOffset);
	// printf("TxTemplate1.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[2].Res0);
	printf("TxTemplate1.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[3].TrSize);
	printf("TxTemplate1.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[3].TrCtrl);
	printf("TxTemplate1.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[3].SymOffset);
	// printf("TxTemplate1.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[3].Res0);
	printf("TxTemplate2.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[0].TrSize);
	printf("TxTemplate2.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[0].TrCtrl);
	printf("TxTemplate2.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[0].SymOffset);
	// printf("TxTemplate2.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[0].Res0);
	printf("TxTemplate2.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[1].TrSize);
	printf("TxTemplate2.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[1].TrCtrl);
	printf("TxTemplate2.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[1].SymOffset);
	// printf("TxTemplate2.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[1].Res0);
	printf("TxTemplate2.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[2].TrSize);
	printf("TxTemplate2.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[2].TrCtrl);
	printf("TxTemplate2.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[2].SymOffset);
	// printf("TxTemplate2.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[2].Res0);
	printf("TxTemplate2.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[3].TrSize);
	printf("TxTemplate2.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[3].TrCtrl);
	printf("TxTemplate2.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[3].SymOffset);
	// printf("TxTemplate2.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[3].Res0);
	printf("TxTemplate3.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[0].TrSize);
	printf("TxTemplate3.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[0].TrCtrl);
	printf("TxTemplate3.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[0].SymOffset);
	// printf("TxTemplate3.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[0].Res0);
	printf("TxTemplate3.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[1].TrSize);
	printf("TxTemplate3.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[1].TrCtrl);
	printf("TxTemplate3.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[1].SymOffset);
	// printf("TxTemplate3.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[1].Res0);
	printf("TxTemplate3.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[2].TrSize);
	printf("TxTemplate3.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[2].TrCtrl);
	printf("TxTemplate3.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[2].SymOffset);
	// printf("TxTemplate3.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[2].Res0);
	printf("TxTemplate3.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[3].TrSize);
	printf("TxTemplate3.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[3].TrCtrl);
	printf("TxTemplate3.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[3].SymOffset);
	// printf("TxTemplate3.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[3].Res0);
	printf("TxSymCtrl0.TslEntry0        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[0]);
	printf("TxSymCtrl0.TslEntry1        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[1]);
	printf("TxSymCtrl0.TslEntry2        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[2]);
	printf("TxSymCtrl0.TslEntry3        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[3]);
	printf("TxSymCtrl0.BasePtr          : 0x%08x\n",jdmaregs.TxSymCtrl0.BasePtr);
	printf("TxSymCtrl0.CurSymBufPtr     : 0x%08x\n",jdmaregs.TxSymCtrl0.CurSymBufPtr);
	printf("TxSymCtrl0.ShortStride      : 0x%08x\n",jdmaregs.TxSymCtrl0.ShortStride);
	printf("TxSymCtrl0.LongStride       : 0x%08x\n",jdmaregs.TxSymCtrl0.LongStride);
	// printf("TxSymCtrl0.Res              : 0x%08x\n",jdmaregs.TxSymCtrl0.Res);
	printf("TxSymCtrl0.IrqSync          : 0x%08x\n",jdmaregs.TxSymCtrl0.IrqSync);
	// printf("TxSymCtrl0.Res1.0           : 0x%08x\n",jdmaregs.TxSymCtrl0.Res1[0]);
	// printf("TxSymCtrl0.Res1.1           : 0x%08x\n",jdmaregs.TxSymCtrl0.Res1[1]);
	printf("TxIrqEna                    : 0x%08x\n",jdmaregs.TxIrqEna);
	printf("TxIrqStat                   : 0x%08x\n",jdmaregs.TxIrqStat);
	printf("TxRes3                      : 0x%08x\n",jdmaregs.TxRes3);
	printf("TxArbiterRoundRobin         : 0x%08x\n",jdmaregs.TxArbiterRoundRobin);
	printf("TxArbiterPriority           : 0x%08x\n",jdmaregs.TxArbiterPriority);
	printf("TxAxiBurstLength            : 0x%08x\n",jdmaregs.TxAxiBurstLength);
	printf("TxFifoDataAccessWidth       : 0x%08x\n",jdmaregs.TxFifoDataAccessWidth);
	printf("TxMCacheCtrl                : 0x%08x\n",jdmaregs.TxMCacheCtrl);
	printf("TxFifoUnderrun              : 0x%08x\n",jdmaregs.TxFifoUnderrun);
	printf("TxFifoClearUnderrun         : 0x%08x\n",jdmaregs.TxFifoClearUnderrun);
	//printf("jdmaregs.TxRes4.[0-21]               : \n");
	//printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.TxRes4[0],jdmaregs.TxRes4[1],jdmaregs.TxRes4[2],jdmaregs.TxRes4[3],jdmaregs.TxRes4[4],jdmaregs.TxRes4[5],jdmaregs.TxRes4[6]);
	//printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.TxRes4[7],jdmaregs.TxRes4[8],jdmaregs.TxRes4[9],jdmaregs.TxRes4[10],jdmaregs.TxRes4[11],jdmaregs.TxRes4[12],jdmaregs.TxRes4[13]);
	//printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.TxRes4[14],jdmaregs.TxRes4[15],jdmaregs.TxRes4[16],jdmaregs.TxRes4[17],jdmaregs.TxRes4[18],jdmaregs.TxRes4[19],jdmaregs.TxRes4[20]);
	printf("************************************************************************\n");
}

void print_jdma_map1_regs(PJDMA_MAP1 p_jdma_map1_regs)
{
	JDMA_MAP1 jdma_map1_regs = *p_jdma_map1_regs;
	printf("************************JDMA PORT1 RX REGS************************\n");
	printf("RxTemplate0.Desc0.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[0].TrSize);
	printf("RxTemplate0.Desc0.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[0].TrCtrl);
	printf("RxTemplate0.Desc0.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[0].SymOffset);
	//printf("RxTemplate0.Desc0.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[0].Res0);
	printf("RxTemplate0.Desc1.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[1].TrSize);
	printf("RxTemplate0.Desc1.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[1].TrCtrl);
	printf("RxTemplate0.Desc1.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[1].SymOffset);
	//printf("RxTemplate0.Desc1.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[1].Res0);
	printf("RxTemplate0.Desc2.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[2].TrSize);
	printf("RxTemplate0.Desc2.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[2].TrCtrl);
	printf("RxTemplate0.Desc2.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[2].SymOffset);
	//printf("RxTemplate0.Desc2.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[2].Res0);
	printf("RxTemplate0.Desc3.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[3].TrSize);
	printf("RxTemplate0.Desc3.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[3].TrCtrl);
	printf("RxTemplate0.Desc3.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate0.Desc[3].SymOffset);
	//printf("RxTemplate0.Desc3.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate0.Desc[3].Res0);
	printf("RxTemplate1.Desc0.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[0].TrSize);
	printf("RxTemplate1.Desc0.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[0].TrCtrl);
	printf("RxTemplate1.Desc0.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[0].SymOffset);
	//printf("RxTemplate1.Desc0.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[0].Res0);
	printf("RxTemplate1.Desc1.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[1].TrSize);
	printf("RxTemplate1.Desc1.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[1].TrCtrl);
	printf("RxTemplate1.Desc1.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[1].SymOffset);
	//printf("RxTemplate1.Desc1.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[1].Res0);
	printf("RxTemplate1.Desc2.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[2].TrSize);
	printf("RxTemplate1.Desc2.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[2].TrCtrl);
	printf("RxTemplate1.Desc2.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[2].SymOffset);
	//printf("RxTemplate1.Desc2.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[2].Res0);
	printf("RxTemplate1.Desc3.Trsize    : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[3].TrSize);
	printf("RxTemplate1.Desc3.TrCtrl    : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[3].TrCtrl);
	printf("RxTemplate1.Desc3.SymOffset : 0x%04x\n",jdma_map1_regs.RxTemplate1.Desc[3].SymOffset);
	//printf("RxTemplate1.Desc3.Res0      : 0x%01x\n",jdma_map1_regs.RxTemplate1.Desc[3].Res0);
	printf("RxSymCtrl0.TslEntry0        : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.TslEntry[0]);
	printf("RxSymCtrl0.TslEntry1        : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.TslEntry[1]);
	printf("RxSymCtrl0.TslEntry2        : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.TslEntry[2]);
	printf("RxSymCtrl0.TslEntry3        : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.TslEntry[3]);
	printf("RxSymCtrl0.BasePtr          : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.BasePtr);
	printf("RxSymCtrl0.CurSymBufPtr     : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.CurSymBufPtr);
	printf("RxSymCtrl0.ShortStride      : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.ShortStride);
	printf("RxSymCtrl0.LongStride       : 0x%08x\n",jdma_map1_regs.RxSymCtrl0.LongStride);
	printf("************************************************************************\n");
}


void print_jesd_regs(PJESDREGS pjesdregs)
{
	JESDREGS jesdregs = *pjesdregs;
	printf("************************JESD REGS************************\n");
	printf("Ad356Comp               : 0x%08x\n",jesdregs.Ad356Comp);
	printf("DataDelayCtrl           : 0x%08x\n",jesdregs.DataDelayCtrl);
	printf("JesdCust                : 0x%08x\n",jesdregs.JesdCust);
	printf("JesdClockDiv            : 0x%08x\n",jesdregs.JesdClockDiv);
	printf("ClockSync               : 0x%08x\n",jesdregs.ClockSync);
	printf("CtrlCurState            : 0x%08x\n",jesdregs.CtrlCurState);
	printf("MultByteBufCtrl         : 0x%08x\n",jesdregs.MultByteBufCtrl);
	printf("MultByteBufStat         : 0x%08x\n",jesdregs.MultByteBufStat);
	printf("MultByteBurstLen        : 0x%08x\n",jesdregs.MultByteBurstLen);
	printf("CtrlPlanMode            : 0x%08x\n",jesdregs.CtrlPlanMode);
	printf("MultByteBuf             : 0x%08x\n",jesdregs.MultByteBuf);
	printf("Res0                    : \n");
	printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",jesdregs.Res0[0],jesdregs.Res0[1],jesdregs.Res0[2],jesdregs.Res0[3],jesdregs.Res0[4]);
	printf("DrvOffsetPos            : 0x%08x\n",jesdregs.DrvOffsetPos);
	printf("DrvOffsetNeg            : 0x%08x\n",jesdregs.DrvOffsetNeg);
	printf("SampleOffsetPos         : 0x%08x\n",jesdregs.SampleOffsetPos);
	printf("SampleOffsetNeg         : 0x%08x\n",jesdregs.SampleOffsetNeg);
	printf("Res1                    : \n");
	printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",jesdregs.Res1[0],jesdregs.Res1[1],jesdregs.Res1[2],jesdregs.Res1[3],jesdregs.Res1[4],jesdregs.Res1[5]);
	printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",jesdregs.Res1[6],jesdregs.Res1[7],jesdregs.Res1[8],jesdregs.Res1[9],jesdregs.Res1[10],jesdregs.Res1[11]);
	printf("InterfaceEnable         : 0x%08x\n",jesdregs.InterfaceEnable);
	printf("Mode                    : 0x%08x\n",jesdregs.Mode);
	printf("NumAntennas             : 0x%08x\n",jesdregs.NumAntennas);
	printf("FrameSignalingCtrl      : 0x%08x\n",jesdregs.FrameSignalingCtrl);
	printf("TxShiftWidth            : 0x%08x\n",jesdregs.TxShiftWidth);
	printf("RxShiftWidth            : 0x%08x\n",jesdregs.RxShiftWidth);
	printf("TxBurstStartLatency     : 0x%08x\n",jesdregs.TxBurstStartLatency);
	printf("TxBurstFinishLatency    : 0x%08x\n",jesdregs.TxBurstFinishLatency);
	printf("RxBurstStartLatency     : 0x%08x\n",jesdregs.RxBurstStartLatency);
	printf("RxBurstFinishLatency    : 0x%08x\n",jesdregs.RxBurstFinishLatency);
	printf("TxBurstLength           : 0x%08x\n",jesdregs.TxBurstLength);
	printf("RxBurstLength           : 0x%08x\n",jesdregs.RxBurstLength);
	printf("BurstToBurstLatency     : 0x%08x\n",jesdregs.BurstToBurstLatency);
	printf("RPDIFState              : 0x%08x\n",jesdregs.RPDIFState);
	printf("RPDIFCtrl               : 0x%08x\n",jesdregs.RPDIFCtrl);
	printf("RPDIFDllCalibrationCtrl : 0x%08x\n",jesdregs.RPDIFDllCalibrationCtrl);
	printf("RPDIFDllCalDoneStat     : 0x%08x\n",jesdregs.RPDIFDllCalDoneStat);
	printf("RPDIFDllManualCtrl      : 0x%08x\n",jesdregs.RPDIFDllManualCtrl);
	printf("RPDIFDllExtDelayCtrl    : 0x%08x\n",jesdregs.RPDIFDllExtDelayCtrl);
	printf("RPDIFDllCalDelay        : 0x%08x\n",jesdregs.RPDIFDllCalDelay);
	printf("RPDIFFrameSyncCtrl      : 0x%08x\n",jesdregs.RPDIFFrameSyncCtrl);
	printf("RPDIFSubFrameLength     : 0x%08x\n",jesdregs.RPDIFSubFrameLength);
	printf("RPDIFTxBurstCountStat   : 0x%08x\n",jesdregs.RPDIFTxBurstCountStat);
	printf("RPDIFRxBurstCountStat   : 0x%08x\n",jesdregs.RPDIFRxBurstCountStat);
	printf("RPDIFSubFrameCountStat  : 0x%08x\n",jesdregs.RPDIFSubFrameCountStat);
	printf("RPDIFIntEnable          : 0x%08x\n",jesdregs.RPDIFIntEnable);
	printf("RPDIFIntStatus          : 0x%08x\n",jesdregs.RPDIFIntStatus);
	printf("Res2                    : \n");
	printf("0x%08x 0x%08x 0x%08x 0x%08x 0x%08x\n",jesdregs.Res2[0],jesdregs.Res2[1],jesdregs.Res2[2],jesdregs.Res2[3],jesdregs.Res2[4]);
	printf("Status                  : 0x%08x\n",jesdregs.Status);
	printf("pBufferAddr             : 0x%08x\n",(unsigned int)jesdregs.pBufferAddr);
	printf("pCtrlBufferAddr         : 0x%08x\n",(unsigned int)jesdregs.pCtrlBufferAddr);
	printf("IntStatus               : 0x%08x\n",jesdregs.IntStatus);
	printf("IntEnable               : 0x%08x\n",jesdregs.IntEnable);
	printf("************************************************************************\n");
}

void print_tm_regs(PRADTMREGS p_rad_tm_regs)
{
	int i,j;
	printf("************************Radio Timers Top Level************************\n");
	printf("INTRPTS_CNTRL              : 0x%08x\n",p_rad_tm_regs->INTRPTS_CNTRL);
	printf("CPU_INTRPTS_CNTRL          : \n");
	printf("0x%08x 0x%08x 0x%08x 0x%08x\n",p_rad_tm_regs->CPU_INTRPTS_CNTRL[0],p_rad_tm_regs->CPU_INTRPTS_CNTRL[1],p_rad_tm_regs->CPU_INTRPTS_CNTRL[2],p_rad_tm_regs->CPU_INTRPTS_CNTRL[3]);
	printf("TIMER_INTRPTS_STATUS       : 0x%08x\n",p_rad_tm_regs->TIMER_INTRPTS_STATUS);
	printf("CHIP_INTRPTS_STATUS        : 0x%08x\n",p_rad_tm_regs->CHIP_INTRPTS_STATUS);
	printf("SLOT_INTRPTS_STATUS        : 0x%08x\n",p_rad_tm_regs->SLOT_INTRPTS_STATUS);
	printf("FRAME_INTRPTS_STATUS       : 0x%08x\n",p_rad_tm_regs->FRAME_INTRPTS_STATUS);
	printf("CHIP_ERROR_STATUS          : 0x%08x\n",p_rad_tm_regs->CHIP_ERROR_STATUS);
	printf("SLOT_ERROR_STATUS          : 0x%08x\n",p_rad_tm_regs->SLOT_ERROR_STATUS);
	printf("FRAME_ERROR_STATUS         : 0x%08x\n",p_rad_tm_regs->FRAME_ERROR_STATUS);
	printf("SUPER_FRAME_INTRPTS_STATUS : 0x%08x\n",p_rad_tm_regs->SUPER_FRAME_INTRPTS_STATUS);
	printf("SUPER_FRAME_ERROR_STATUS   : 0x%08x\n",p_rad_tm_regs->SUPER_FRAME_ERROR_STATUS);
	printf("RESERVED[0]                : 0x%08x\n",p_rad_tm_regs->RESERVED[0]);
	printf("RESERVED[1]                : 0x%08x\n",p_rad_tm_regs->RESERVED[1]);

	for (j = 0;j < 6;j++) {
		printf("-------------------------------------------FSyncTimers-----------------------------------------\n");
		printf("FSyncTimers[%d]                             : \n",j);
		printf("FSyncTimers[%d].PRESCALE_CLK_CNTRL          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].PRESCALE_CLK_CNTRL);
		printf("FSyncTimers[%d].PRESCALE_DIV_CNTRL          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].PRESCALE_DIV_CNTRL);
		printf("FSyncTimers[%d].TIMER_CNTRL                 : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].TIMER_CNTRL);
		printf("FSyncTimers[%d].BASIC_CNTR_TC_VAL           : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].BASIC_CNTR_TC_VAL);
		printf("FSyncTimers[%d].CHIP_CNTR_TC_VAL_A          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_TC_VAL_A);
		printf("FSyncTimers[%d].SLOT_CNTR_TC_VAL_A          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_TC_VAL_A);
		printf("FSyncTimers[%d].FRAME_CNTR_TC_VAL_A         : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_TC_VAL_A);
		printf("FSyncTimers[%d].CHIP_CNTR_COMP_VAL_A        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_COMP_VAL_A);
		printf("FSyncTimers[%d].SLOT_CNTR_COMP_VAL_A        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_COMP_VAL_A);
		printf("FSyncTimers[%d].FRAME_CNTR_COMP_VAL_A       : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_COMP_VAL_A);
		printf("FSyncTimers[%d].CHIP_CNTR_MASK_VAL_A        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_MASK_VAL_A);
		printf("FSyncTimers[%d].SLOT_CNTR_MASK_VAL_A        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_MASK_VAL_A);
		printf("FSyncTimers[%d].FRAME_CNTR_MASK_VAL_A       : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_MASK_VAL_A);
		printf("FSyncTimers[%d].CHIP_CNTR_TC_VAL_B          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_TC_VAL_B);
		printf("FSyncTimers[%d].SLOT_CNTR_TC_VAL_B          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_TC_VAL_B);
		printf("FSyncTimers[%d].FRAME_CNTR_TC_VAL_B         : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_TC_VAL_B);
		printf("FSyncTimers[%d].CHIP_CNTR_COMP_VAL_B        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_COMP_VAL_B);
		printf("FSyncTimers[%d].SLOT_CNTR_COMP_VAL_B        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_COMP_VAL_B);
		printf("FSyncTimers[%d].FRAME_CNTR_COMP_VAL_B       : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_COMP_VAL_B);
		printf("FSyncTimers[%d].CHIP_CNTR_MASK_VAL_B        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_MASK_VAL_B);
		printf("FSyncTimers[%d].SLOT_CNTR_MASK_VAL_B        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_MASK_VAL_B);
		printf("FSyncTimers[%d].FRAME_CNTR_MASK_VAL_B       : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_MASK_VAL_B);
		printf("FSyncTimers[%d].CHIP_CNTR_OFFSET_VAL        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_OFFSET_VAL);
		printf("FSyncTimers[%d].SLOT_CNTR_OFFSET_VAL        : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_OFFSET_VAL);
		printf("FSyncTimers[%d].FRAME_CNTR_OFFSET_VAL       : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_OFFSET_VAL);
		printf("FSyncTimers[%d].BASIC_CNTR_INIT_VAL         : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].BASIC_CNTR_INIT_VAL);
		printf("FSyncTimers[%d].CHIP_CNTR_INIT_VAL          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].CHIP_CNTR_INIT_VAL);
		printf("FSyncTimers[%d].SLOT_CNTR_INIT_VAL          : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SLOT_CNTR_INIT_VAL);
		printf("FSyncTimers[%d].FRAME_CNTR_INIT_VAL         : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].FRAME_CNTR_INIT_VAL);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_INIT_VAL   : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_INIT_VAL);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_TC_VAL_A   : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_TC_VAL_A);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_COMP_VAL_A : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_COMP_VAL_A);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_MSK_VAL_A  : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_MSK_VAL_A);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_TC_VAL_B   : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_TC_VAL_B);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_COMP_VAL_B : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_COMP_VAL_B);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_MSK_VAL_B  : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_MSK_VAL_B);
		printf("FSyncTimers[%d].SUPER_FRAME_CNTR_OFFSET_VAL : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].SUPER_FRAME_CNTR_OFFSET_VAL);
		printf("FSyncTimers[%d].INTRPTS_CNTRL               : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].INTRPTS_CNTRL);
		printf("FSyncTimers[%d].INTRPTS_STATUS              : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].INTRPTS_STATUS);
		printf("FSyncTimers[%d].TIMER_STATUS                : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].TIMER_STATUS);
		printf("FSyncTimers[%d].COUNTER_CURR_VAL            : 0x%08x\n",j,p_rad_tm_regs->FSyncTimers[j].COUNTER_CURR_VAL);

		printf("FSyncTimers[%d].reservered:",j);
		for (i = 0;i < (0x100-0xA4)/4 ;i++) {
			if (i % 10 == 0)
				printf("\n");
			printf("0x%08x ",j,p_rad_tm_regs->FSyncTimers[j].RESERVED[i]);
		}
		printf("\n");
	}
	printf("************************************************************************\n");

}

void print_esync_regs(PESYNCTMREGS p_rad_tm_regs)
{	
	int i,j;
	PFSYNCTMREGS p_ESync = &(p_rad_tm_regs->FSyncTimers);
	printf("************************ESync Control Top Level************************\n");
	printf("INTRPTS_CNTRL              : 0x%08x\n",p_rad_tm_regs->INTRPTS_CNTRL);
	printf("CPU_INTRPTS_CNTRL          : \n");
	printf("0x%08x 0x%08x 0x%08x 0x%08x\n",p_rad_tm_regs->CPU_INTRPTS_CNTRL[0],p_rad_tm_regs->CPU_INTRPTS_CNTRL[1],p_rad_tm_regs->CPU_INTRPTS_CNTRL[2],p_rad_tm_regs->CPU_INTRPTS_CNTRL[3]);
	printf("TIMER_INTRPTS_STATUS       : 0x%08x\n",p_rad_tm_regs->TIMER_INTRPTS_STATUS);
	printf("CHIP_INTRPTS_STATUS        : 0x%08x\n",p_rad_tm_regs->CHIP_INTRPTS_STATUS);
	printf("SLOT_INTRPTS_STATUS        : 0x%08x\n",p_rad_tm_regs->SLOT_INTRPTS_STATUS);
	printf("FRAME_INTRPTS_STATUS       : 0x%08x\n",p_rad_tm_regs->FRAME_INTRPTS_STATUS);
	printf("CHIP_ERROR_STATUS          : 0x%08x\n",p_rad_tm_regs->CHIP_ERROR_STATUS);
	printf("SLOT_ERROR_STATUS          : 0x%08x\n",p_rad_tm_regs->SLOT_ERROR_STATUS);
	printf("FRAME_ERROR_STATUS         : 0x%08x\n",p_rad_tm_regs->FRAME_ERROR_STATUS);
	printf("SUPER_FRAME_INTRPTS_STATUS : 0x%08x\n",p_rad_tm_regs->SUPER_FRAME_INTRPTS_STATUS);
	printf("SUPER_FRAME_ERROR_STATUS   : 0x%08x\n",p_rad_tm_regs->SUPER_FRAME_ERROR_STATUS);
	printf("RESERVED[0]                : 0x%08x\n",p_rad_tm_regs->RESERVED[0]);
	printf("RESERVED[1]                : 0x%08x\n",p_rad_tm_regs->RESERVED[1]);

	
	for (j = 0;j < 1;j++) {
		printf("-------------------------------------------ESyncTimers-----------------------------------------\n");
		printf("ESyncTimers[%d]                             : \n",j);
		printf("ESyncTimers[%d].PRESCALE_CLK_CNTRL          : 0x%08x\n",j,p_ESync->PRESCALE_CLK_CNTRL);
		printf("ESyncTimers[%d].PRESCALE_DIV_CNTRL          : 0x%08x\n",j,p_ESync->PRESCALE_DIV_CNTRL);
		printf("ESyncTimers[%d].TIMER_CNTRL                 : 0x%08x\n",j,p_ESync->TIMER_CNTRL);
		printf("ESyncTimers[%d].BASIC_CNTR_TC_VAL           : 0x%08x\n",j,p_ESync->BASIC_CNTR_TC_VAL);
		printf("ESyncTimers[%d].CHIP_CNTR_TC_VAL_A          : 0x%08x\n",j,p_ESync->CHIP_CNTR_TC_VAL_A);
		printf("ESyncTimers[%d].SLOT_CNTR_TC_VAL_A          : 0x%08x\n",j,p_ESync->SLOT_CNTR_TC_VAL_A);
		printf("ESyncTimers[%d].FRAME_CNTR_TC_VAL_A         : 0x%08x\n",j,p_ESync->FRAME_CNTR_TC_VAL_A);
		printf("ESyncTimers[%d].CHIP_CNTR_COMP_VAL_A        : 0x%08x\n",j,p_ESync->CHIP_CNTR_COMP_VAL_A);
		printf("ESyncTimers[%d].SLOT_CNTR_COMP_VAL_A        : 0x%08x\n",j,p_ESync->SLOT_CNTR_COMP_VAL_A);
		printf("ESyncTimers[%d].FRAME_CNTR_COMP_VAL_A       : 0x%08x\n",j,p_ESync->FRAME_CNTR_COMP_VAL_A);
		printf("ESyncTimers[%d].CHIP_CNTR_MASK_VAL_A        : 0x%08x\n",j,p_ESync->CHIP_CNTR_MASK_VAL_A);
		printf("ESyncTimers[%d].SLOT_CNTR_MASK_VAL_A        : 0x%08x\n",j,p_ESync->SLOT_CNTR_MASK_VAL_A);
		printf("ESyncTimers[%d].FRAME_CNTR_MASK_VAL_A       : 0x%08x\n",j,p_ESync->FRAME_CNTR_MASK_VAL_A);
		printf("ESyncTimers[%d].CHIP_CNTR_TC_VAL_B          : 0x%08x\n",j,p_ESync->CHIP_CNTR_TC_VAL_B);
		printf("ESyncTimers[%d].SLOT_CNTR_TC_VAL_B          : 0x%08x\n",j,p_ESync->SLOT_CNTR_TC_VAL_B);
		printf("ESyncTimers[%d].FRAME_CNTR_TC_VAL_B         : 0x%08x\n",j,p_ESync->FRAME_CNTR_TC_VAL_B);
		printf("ESyncTimers[%d].CHIP_CNTR_COMP_VAL_B        : 0x%08x\n",j,p_ESync->CHIP_CNTR_COMP_VAL_B);
		printf("ESyncTimers[%d].SLOT_CNTR_COMP_VAL_B        : 0x%08x\n",j,p_ESync->SLOT_CNTR_COMP_VAL_B);
		printf("ESyncTimers[%d].FRAME_CNTR_COMP_VAL_B       : 0x%08x\n",j,p_ESync->FRAME_CNTR_COMP_VAL_B);
		printf("ESyncTimers[%d].CHIP_CNTR_MASK_VAL_B        : 0x%08x\n",j,p_ESync->CHIP_CNTR_MASK_VAL_B);
		printf("ESyncTimers[%d].SLOT_CNTR_MASK_VAL_B        : 0x%08x\n",j,p_ESync->SLOT_CNTR_MASK_VAL_B);
		printf("ESyncTimers[%d].FRAME_CNTR_MASK_VAL_B       : 0x%08x\n",j,p_ESync->FRAME_CNTR_MASK_VAL_B);
		printf("ESyncTimers[%d].CHIP_CNTR_OFFSET_VAL        : 0x%08x\n",j,p_ESync->CHIP_CNTR_OFFSET_VAL);
		printf("ESyncTimers[%d].SLOT_CNTR_OFFSET_VAL        : 0x%08x\n",j,p_ESync->SLOT_CNTR_OFFSET_VAL);
		printf("ESyncTimers[%d].FRAME_CNTR_OFFSET_VAL       : 0x%08x\n",j,p_ESync->FRAME_CNTR_OFFSET_VAL);
		printf("ESyncTimers[%d].BASIC_CNTR_INIT_VAL         : 0x%08x\n",j,p_ESync->BASIC_CNTR_INIT_VAL);
		printf("ESyncTimers[%d].CHIP_CNTR_INIT_VAL          : 0x%08x\n",j,p_ESync->CHIP_CNTR_INIT_VAL);
		printf("ESyncTimers[%d].SLOT_CNTR_INIT_VAL          : 0x%08x\n",j,p_ESync->SLOT_CNTR_INIT_VAL);
		printf("ESyncTimers[%d].FRAME_CNTR_INIT_VAL         : 0x%08x\n",j,p_ESync->FRAME_CNTR_INIT_VAL);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_INIT_VAL   : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_INIT_VAL);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_TC_VAL_A   : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_TC_VAL_A);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_COMP_VAL_A : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_COMP_VAL_A);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_MSK_VAL_A  : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_MSK_VAL_A);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_TC_VAL_B   : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_TC_VAL_B);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_COMP_VAL_B : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_COMP_VAL_B);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_MSK_VAL_B  : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_MSK_VAL_B);
		printf("ESyncTimers[%d].SUPER_FRAME_CNTR_OFFSET_VAL : 0x%08x\n",j,p_ESync->SUPER_FRAME_CNTR_OFFSET_VAL);
		printf("ESyncTimers[%d].INTRPTS_CNTRL               : 0x%08x\n",j,p_ESync->INTRPTS_CNTRL);
		printf("ESyncTimers[%d].INTRPTS_STATUS              : 0x%08x\n",j,p_ESync->INTRPTS_STATUS);
		printf("ESyncTimers[%d].TIMER_STATUS                : 0x%08x\n",j,p_ESync->TIMER_STATUS);
		printf("ESyncTimers[%d].COUNTER_CURR_VAL            : 0x%08x\n",j,p_ESync->COUNTER_CURR_VAL);

		printf("ESyncTimers[%d].reservered:",j);
		for (i = 0;i < (0x100-0xA4)/4 ;i++) {
			if (i % 10 == 0)
				printf("\n");
			printf("0x%08x ",j,p_ESync->RESERVED[i]);
		}
		printf("\n");
	}
	printf("************************************************************************\n");

}

int rw_regs(int fd_mem)
{
    int size = 4;
    unsigned int data;
    unsigned int addr;
	char rw;
	while(1) {
	printf("************************Dangerous************************\nPlease input quit(q) write(w) or read(r)?\n");
	scanf("%c", &rw);
	getchar();
	if(rw == 'q')
		return 0;
	else if(rw == 'r')
	{
		printf("Please input addr( 0x???? )\n");
		scanf("0x%x", &addr);
		getchar();
		addr -= RADGROUP_BASE_ADDR;
		lseek(fd_mem, addr, SEEK_SET);
		read(fd_mem, &data, size);
		addr += RADGROUP_BASE_ADDR;
		printf("[Read] the val in Addr. 0x%08x is 0x%08x\n", addr, data);
	}
	else if(rw == 'w')
	{
		printf("Please input addr( 0x???? )\n");
		scanf("0x%x", &addr);
		getchar();
		printf("Please input val( 0x???? )\n");
		scanf("0x%x", &data);
		getchar();
		addr -= RADGROUP_BASE_ADDR;
		lseek(fd_mem, addr, SEEK_SET);
		write(fd_mem, &data, size);
		addr += RADGROUP_BASE_ADDR;
		printf("[Write] the val 0x%08x to Addr. 0x%08x\n", data, addr);
	}	
	else
		printf("try again\n");
	}
	return 0;
}

int radio_playback_signal(int fd_rad)
{
	int do_what;
	int errno;

	while(1)
	{
		printf("1-1Mhz sinwave, 2-10Mhz QAM16, 3-10Mhz QAM64, 4-20Mhz LTE, 5-triangle wave, 6-10Mhz qpsk, q-quit\n");
		do_what = getchar();
		getchar();
		switch(do_what)
		{
			case '1':
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF, sin1mhziqbuf);
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF, sin1mhziqbuf);
				return errno;
			case '2':
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF, ltedlqam16iqbuf);
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF, ltedlqam16iqbuf);
				return errno;
			case '3':
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF, ltedlqam64iqbuf);
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF, ltedlqam64iqbuf);
				return errno;
			case '4':
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF, ltedl20mhziqbuf);
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF, ltedl20mhziqbuf);
				return errno;
			case '5':
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF, trianglewaveiqbuf);
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF, trianglewaveiqbuf);
				return errno;
			case '6':
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP0_TX_BUF_DATA_TO_BUF, lteqpsk10mhziqbuf);
				errno = ioctl(fd_rad, TRAD_IOCTL_COPY_JDMA_MAP1_TX_BUF_DATA_TO_BUF, lteqpsk10mhziqbuf);
				return errno;
			case 'q':
				return 0;
		}
	}
}

int catch_data_master(int fd_rad)
{
    char do_what;
    FILE *fprx;
    int exit_flag = 0;
    int ret = 0, i;
    short *recv_addr;
    while (1) {
        printf("1:SAVE JDMA Ant0 12ms CELL SEARCH DATA	2:SAVE JDMA Ant1 12ms CELL SEARCH DATA\n");
        printf("3:SAVE JDMA Ant0 40ms CELL SEARCH DATA	4:SAVE JDMA Ant1 40ms CELL SEARCH DATA\n");
        printf("5:SAVE JDMA Ant0 10ms subframe DATA	6:SAVE JDMA Ant1 10ms subframe DATA\n");
        printf("7:SAVE MAP  Ant0 10ms subframe DATA	8:SAVE MAP  Ant1 10ms subframe DATA\n"); 
		printf("0:QUIT\n");
	do_what = getchar();
	getchar();
	switch (do_what){
	    case '1'://ant0 cs 12ms
		recv_addr = (short *)malloc(CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE);
                ret = ioctl(fd_rad,TRAD_IOCTL_COPY_CELL_SEARCH_JDMA0_DATA_12,recv_addr);
                fprx = fopen("cell_search_12subframe_Ant0_Iq_data","w+");
                for (i = 0;i < CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE / 2;i++) {
                    fprintf(fprx,"%d\n",*(recv_addr + i));
                }
                free(recv_addr);
                fclose(fprx);
	    break;
	    case '2'://ant1 cs 12ms
		recv_addr = (short *)malloc(CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE);
                ret = ioctl(fd_rad,TRAD_IOCTL_COPY_CELL_SEARCH_JDMA1_DATA_12,recv_addr);
                fprx = fopen("cell_search_12subframe_Ant1_Iq_data","w+");
                for (i = 0;i < CELL_SEARCH_12SUBFRAME_DATA_BASEADDR_PHY_SIZE / 2;i++) {
                    fprintf(fprx,"%d\n",*(recv_addr + i));
                }
                free(recv_addr);
                fclose(fprx);
	    break;
	    break;
	    case '3'://ant0 cs 40ms
		recv_addr = (short *)malloc(CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE);
                ret = ioctl(fd_rad,TRAD_IOCTL_COPY_CELL_SEARCH_JDMA0_DATA_40,recv_addr);
                fprx = fopen("cell_search_40subframe_Ant0_Iq_data","w+");
                for (i = 0;i < CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE / 2;i++) {
                    fprintf(fprx,"%d\n",*(recv_addr + i));
                }
                free(recv_addr);
                fclose(fprx);
	    break;
	    case '4'://ant1 cs 40ms
		recv_addr = (short *)malloc(CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE);
                ret = ioctl(fd_rad,TRAD_IOCTL_COPY_CELL_SEARCH_JDMA1_DATA_40,recv_addr);
                fprx = fopen("cell_search_40subframe_Ant1_Iq_data","w+");
                for (i = 0;i < CELL_SEARCH_40SUBFRAME_DATA_BASEADDR_PHY_SIZE / 2;i++) {
                    fprintf(fprx,"%d\n",*(recv_addr + i));
                }
                free(recv_addr);
                fclose(fprx);
	    break;
	    case '5'://ant0 rx sampling 10subframe
		recv_addr = (short *)malloc(JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_MASTER_JDMA0_RX_SAMPLING_10SUBFRAME_DATA,recv_addr);
		fprx = fopen("master_ant0_time_domain_10ms","w+");
		for (i = 0;i < JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    case '6'://ant1 rx sampling 10subframe
		recv_addr = (short *)malloc(JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_MASTER_JDMA1_RX_SAMPLING_10SUBFRAME_DATA,recv_addr);
		fprx = fopen("master_ant1_time_domain_10ms","w+");
		for (i = 0;i < JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    case '7'://ANT0 MAP OUTBOUND DATA
		recv_addr = (short *)malloc(JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_MASTER_MAP0_OUTBOUNT_DATA,recv_addr);
		fprx = fopen("master_ant0_freq_domain_10ms","w+");
		for (i = 0;i < JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    case '8'://ANT1 MAP OUTBOUND DATA
		recv_addr = (short *)malloc(JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_MASTER_MAP1_OUTBOUNT_DATA,recv_addr);
		fprx = fopen("master_ant1_freq_domain_10ms","w+");
		for (i = 0;i < JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    default:
		exit_flag = 1;
	    break;
	    }
	    if (exit_flag == 1)
		break;
	}
    return ret;
}
int catch_data_slave(int fd_rad)
{
    char do_what;
    FILE *fprx;
    int exit_flag = 0;
    int ret = 0, i;
    short *recv_addr;
    while (1) {
        printf("1:SAVE JDMA Ant0 10ms subframe DATA	2:SAVE JDMA Ant1 10ms subframe DATA\n");
        printf("3:SAVE MAP  Ant0 10ms subframe DATA	4:SAVE MAP  Ant1 10ms subframe DATA\n"); 
		printf("0:QUIT\n");
	do_what = getchar();
	getchar();
	switch (do_what){
	    case '1'://ant0 rx sampling
		recv_addr = (short *)malloc(JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_SLAVE_JDMA0_RX_SAMPLING_5SUBFRAME_DATA,recv_addr);		//for slave catch data	symbol pingpong
		// ret = ioctl(fd_rad,TRAD_IOCTL_MASTER_JDMA0_RX_SAMPLING_10SUBFRAME_DATA,recv_addr);	//for master catch data	subframe pingpong
		fprx = fopen("slave_ant0_time_domain_10ms","w+");
		for (i = 0;i < JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    case '2'://ant1 rx sampling
		recv_addr = (short *)malloc(JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_SLAVE_JDMA1_RX_SAMPLING_5SUBFRAME_DATA,recv_addr);		//for slave catch data	symbol pingpong
		// ret = ioctl(fd_rad,TRAD_IOCTL_MASTER_JDMA1_RX_SAMPLING_10SUBFRAME_DATA,recv_addr);	//for master catch data	subframe pingpong
		fprx = fopen("slave_ant1_time_domain_10ms","w+");
		for (i = 0;i < JDMA_RX_SAMPLING_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    case '3'://ANT0 MAP OUTBOUND DATA
		recv_addr = (short *)malloc(JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_SLAVE_MAP0_OUTBOUNT_DATA,recv_addr);
		fprx = fopen("slave_ant0_freq_domain_10ms","w+");
		for (i = 0;i < JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    case '4'://ANT1 MAP OUTBOUND DATA
		recv_addr = (short *)malloc(JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE);
		ret = ioctl(fd_rad,TRAD_IOCTL_SLAVE_MAP1_OUTBOUNT_DATA,recv_addr);
		fprx = fopen("slave_ant1_freq_domain_10ms","w+");
		for (i = 0;i < JDMA_MAP_OUT_BOUND_10SUBFRAME_DATA_SIZE / 2;i++) {
		    fprintf(fprx,"%d\n",*(recv_addr + i));
		}
		free(recv_addr);
		fclose(fprx);
	    break;
	    default:
		exit_flag = 1;
	    break;
	    }
	    if (exit_flag == 1)
		break;
	}
    return ret;
}
char char_to_hex(char *c) 
{
	char result = 0;	
	
	if ((*c > '0' - 1) && (*c < '9' + 1))
		result = (*c - '0') << 4;
	else if ((*c > 'a' - 1) && (*c < 'z' + 1))
		result = (*c - 'a' + 10) << 4;
	else 
		result = (*c - 'A' + 10) << 4;

	if ((*(c + 1) > '0' - 1) && (*(c + 1) < '9' + 1))
		result += *(c + 1) - '0';
	else if ((*(c + 1) > 'a' - 1) && (*(c + 1) < 'z' + 1))
		result += *(c + 1) - 'a' + 10;
	else 
		result += *(c + 1) - 'A' + 10;

	return result;
}
/*
int cfg9361_base_rx_freq(int fd_spi,double freq)
{
	double rx_vco_divider = 0;
	unsigned int N1 = 0;
	unsigned int N2 = 0;
	unsigned int reg5 = 0;
	double f_vco = 0;
	double f_ref = 40;

	if (freq > 1500 && freq < 2700 ) {
		rx_vco_divider = 4;
		reg5 = 0x11;
	} else if (freq > 750 && freq < 1500) {
		rx_vco_divider = 8;
		reg5 = 0x22;
	} else if (freq > 650 && freq < 750) {
		rx_vco_divider = 16;
		reg5 = 0x33;
	} else {
		printf("invalid value\n");
		return 1;
	}

	f_vco = freq * rx_vco_divider;
	N1 = (unsigned int)floor(f_vco / f_ref);
	printf("0x231 : %u\n",N1 & 0xff);
	printf("0x232 : %u\n",(N1 >> 8) & 0x7);
	Ad9361WriteReg(fd_spi, 0x231, (N1 & 0xff)); // for level mode move radio to FDD active state
	Ad9361WriteReg(fd_spi, 0x232,((N1 >> 8) & 0x7)); // for level mode move radio to FDD active state


	N2 = (unsigned int)round(8388593 * (f_vco / f_ref - N1));
	printf("0x233 : %u\n",N2 & 0xff);
	printf("0x234 : %u\n",((N2 >> 8) & 0xff));
	printf("0x235 : %u\n",((N2 >> 16) & 0x7f));
	Ad9361WriteReg(fd_spi, 0x233,N2 & 0xff ); // for level mode move radio to FDD active state
	Ad9361WriteReg(fd_spi, 0x234,((N2 >> 8) & 0xff )); // for level mode move radio to FDD active state
	Ad9361WriteReg(fd_spi, 0x235,((N2 >> 16) & 0x7f)); // for level mode move radio to FDD active state

	return 0;
}
*/
unsigned int str_to_hex32(char *str)
{
	short i;
	unsigned int tmp,num_hex,hex_mod;
	unsigned short len = strlen(str);

	if (len == 0 || len > 8)
		return 0xffffffff;

	num_hex = 0;
	hex_mod = 1;
	for (i = len - 1;i > -1 ;i--) {
		if ((str[i] >= '0' && str[i] <= '9'))
			tmp = str[i] - '0';
		else if ((str[i] >= 'a' && str[i] <= 'f'))
			tmp = str[i] - 'a' + 10;
		else if ((str[i] >= 'A' && str[i] <= 'F'))
			tmp = str[i] - 'A';
		else 
			return 0xffffffff;

		num_hex += tmp * hex_mod;
		hex_mod *= 16;
	}

	return num_hex;
}

unsigned short str_to_hex(char *str)
{
	short i;
	unsigned short tmp,num_hex,hex_mod;
	unsigned short len = strlen(str);

	if (len == 0 || len > 8)
		return 0xffff;

	num_hex = 0;
	hex_mod = 1;
	for (i = len - 1;i > -1 ;i--) {
		if ((str[i] >= '0' && str[i] <= '9'))
			tmp = str[i] - '0';
		else if ((str[i] >= 'a' && str[i] <= 'f'))
			tmp = str[i] - 'a' + 10;
		else if ((str[i] >= 'A' && str[i] <= 'F'))
			tmp = str[i] - 'A';
		else 
			return 0xffff;

		num_hex += tmp * hex_mod;
		hex_mod *= 16;
	}

	return num_hex;
}

signed short vector_test_aligning[] = {
};
