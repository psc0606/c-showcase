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

#include "../ad9361radio.h"
#include "../spi.h"
#include "../trioctl.h"
#include "../radtimers.h"

/*
 * 包括jdma，jesd的数据结�? */
#include "../tradio.h"



//extern unsigned int sin1mhziqnum;
//extern signed short sin1mhziqbuf[];

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

#define TRX_ARRAY_SIZE  SAMPLING_RATE_20MHZ*4             
#define TRANSMIT_PER_MS_20MZ (15360 * 4 * 2)

//#define PCIE
// default configuration for radio
RADCONF RadConfig = {
    .NumAntennas = 2,
    .SamplingRate = 15360,
    .TxLoopback = 0,
    .Band = 7,
    .RadioMode = RADMODE_LEVEL,
    .MapSyncMode = MAPSYNCMODE_OFF,

    .TxPlayBufUserVirtAddr = 0x00000000,//ltedl5mhziqbuf,to cram,最后给pTxDmaBuf
    .TxPlayBufVirtAddr = 0x00000000,
    .TxPlayBufPhysAddr = 0x00000000,
    .TxPlayBufSize = 0x00000000,

    .pRadProfile = 0,
    .MapBootReq = 1,
    .MapBufHeap = 0,
    .RadTimerIrqTrigger = 0,
    .RadFrameSyncEnable = 0,
	.TypeCluster = 0,
};

static const char *spi_device[2] =  {
    "/dev/spidev0.4",
    "/dev/spidev1.2"
};

static const char *rad_device[2] =  {
    "/dev/tradio0",
    "/dev/tradio1"
};

static int RadId = -1;
static int radio_init = 0;
static int radio_mode = -1;
static int radio_loopback = 0;
static int radio_test_time = 0;
static int radio_playback_sid = 0;
static int gpio_rf_sync = 0;
static int radio_band = -1;
static int radio_sampling_rate = -1;
static int radio_enable = 0;
static int radio_reset_request = 0;
static int type_cluster = -1;

#define SCRIPT_CMDBUFFER_DEFAULT_SIZE (5000*sizeof(ADRADPROFILE))
#define SCRIPT_DEFAULT_WCDVALUE 0


#define SAMPLE_IQBUFFER_DEFAULT_SIZE (200000*sizeof(ADRADIQ))


static unsigned int pattern_buffer[2*15360];

static unsigned int rad_timer_records[1000];

static void radio_reset(void);

static void pabort(const char *s)
{
    if (errno) {
        perror(s);
        exit(errno);
    } else {
        printf(s);
        exit(-1);
    }
}

static void print_usage(const char *prog)
{
    printf("Usage: %s [-B:a:b:c:d:e:g:p:l:m:s:t:f:q:i]\n", prog);
    puts(
         "  -a <Number of Antennas > - default is 1\n"
         "  -b <Radio Band>          - default is 7 for LVDS, 2 for CMOS. No other bands supported yet.\n"
		 "  -c <type_cluster>        - (0 - Master, 1 - Slave)\n"
		 "  -d <Radio ID>            - (0 - CMOS, 1 - LVDS)\n"
         "  -i                       - AD9361 initialization\n"
         "  -l <Loopback Type>       - 0-no loopback, 1-Tx loopback\n"
         "  -p <Playback Signal>     - 0-pattern, 1-1Mhz sinwave, 2-10Mhz QAM16, 3-10Mhz QAM64, 4-5Mhz LTE, 5-20Mhz LTE, 6-WCDMA\n"
         "  -r <Radio Mode>          - Pulse=0/Level=1\n"
         "  -s <Sampling Rate>       - default is 15360 for LVDS, 7680 for CMOS\n"
    );
    exit(1);
}

static void parse_opts(int argc, char *argv[])
{
    while (1) {
        static const struct option lopts[] = {
            { "radio_id",            1, 0, 'd' },
            { "radio_antenas",       1, 0, 'a' },
            { "radio_band",          1, 0, 'b' },
            { "type_cluster",        1, 0, 'c' },
            { "radio_mode",          1, 0, 'r' },
            { "radio_init",          0, 0, 'i' },
            { "radio_loopback",      0, 0, 'l' },
            { "radio_test_time",     0, 0, 't' },
            { "radio_playback_sid",  0, 0, 'p' },
            { "radio_sampling_rate", 0, 0, 's' },
            { NULL,                  0, 0, '\0' },
        };
        int c;
        int option_index = 0;

        c = getopt_long(argc, argv, "a:b:c:d:l:p:r:s:t:i", lopts, &option_index);

        if (c == -1)
            break;

        switch (c) {
    case 0:
        /* If this option set a flag, do nothing else now. */
        if (lopts[option_index].flag != 0)
            break;

        printf ("option %s", lopts[option_index].name);

        if (optarg)
        printf (" with arg %s", optarg);
        printf ("\n");
        break;
        case 'a':
            RadConfig.NumAntennas = atoi(optarg);
            break;
        case 'b':
            radio_band = atoi(optarg);
            break;
        case 'c':
            type_cluster = atoi(optarg);
            break;
        case 'd':
            RadId = atoi(optarg);
            break;
        case 'i':
            radio_init = 1;
            break;
        case 'l':
            radio_loopback = atoi(optarg);
            break;
        case 'p':
            radio_playback_sid = atoi(optarg);
            break;
        case 'r':
            radio_mode = atoi(optarg);
            break;
        case 's':
            radio_sampling_rate= atoi(optarg);
            break;
        case 't':
            radio_test_time = atoi(optarg);
            break;
        default:
            print_usage(argv[0]);
            break;
        }
    }
}
int spi_config(int fd)
{
    int ret;
    static uint8_t spi_mode = SPI_CPOL;
    static uint8_t spi_bits = 8;
    static uint32_t spi_speed = 1000000;

    // spi mode
    ret = ioctl(fd, SPI_IOC_WR_MODE, &spi_mode);
    if (ret == -1)
        pabort("can't set spi mode");

    ret = ioctl(fd, SPI_IOC_RD_MODE, &spi_mode);
    if (ret == -1)
        pabort("can't get spi mode");

    // bits per word
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &spi_bits);
    if (ret == -1)
        pabort("can't set bits per word");

    ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &spi_bits);
    if (ret == -1)
        pabort("can't get bits per word");

    // max speed hz
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed);
    if (ret == -1)
        pabort("can't set max speed hz");

    ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &spi_speed);
    if (ret == -1)
        pabort("can't get max speed hz");

//    printf("spi mode: %d\n", spi_mode);
//    printf("bits per word: %d\n", spi_bits);
//    printf("max speed: %d Hz (%d KHz)\n", spi_speed, spi_speed/1000);

}

void print_jdma_regs(PJDMAREGS );
void print_jesd_regs(PJESDREGS );
void print_iq_counter_regs(PIQCONFIG);
void print_tm_regs(PRADTMREGS);
unsigned int print_syncnet_regs(PSYNCNET_CONFIG psyncnet_regs);
char char_to_hex(char *c);
unsigned short str_to_hex(char *);
unsigned int str_to_hex32(char *str);


int main(int argc, char *argv[])
{
    FILE *fp_load;
	int fd_pcie;
    int ret = 0;
    int fd_spi;
    unsigned int jdma_handle;
    char jdma_tx_status;
    char jdma_rx_status;
    int fd_rad;
    int i,j;
    char do_what;
    double central_freq_point;
    int exit_flag = 0;
    unsigned int rx_data_size = 0;
    ADRADPROFILE load_data[5000];
    unsigned int write_tm_reg[2];
    char input_str1[10];
    char input_str2[10];
    unsigned int write_syncnet_reg[2];
    char s[80];
	unsigned long pcie_data,data;
	char pcie_data_back[4];

    JDMAREGS jdmaregs;
    JESDREGS jesdregs;
    RADTMREGS radtmregs;
    IQCONFIG IqCntCfg;
    SYNCNET_CONFIG syncnet_regs;

    parse_opts(argc, argv);

    if (RadId != 0 && RadId != 1)
        pabort("Invalid RadId ID. Use 0 for CMOS, 1 for LVDS radio");
    RadConfig.RadId = RadId;
    if (type_cluster != 0 && type_cluster != 1)
        pabort("Invalid type_cluster ID. Use 0 for Master, 1 for Slave Cluster");
    RadConfig.TypeCluster = type_cluster;


    if (RadConfig.NumAntennas != 1 && RadConfig.NumAntennas != 2)
        pabort("Invalid number of antennas. Use 1 or 2");

    if (radio_mode == -1) {
        radio_mode = (RadId == 0) ? RADMODE_LEVEL : RADMODE_PULSE; // set Level mode as default for CMOS and Pulse as default for LVDS
        printf("RadId=%d radio_mode=%d\n", RadId, radio_mode);
    }
    RadConfig.RadioMode = radio_mode;

    if (radio_band == -1)
        radio_band = (RadId == 0) ? 2 : 7; // set Band 2 as default for CMOS and Band 7 as default for LVDS
    RadConfig.Band = radio_band;

    if (radio_sampling_rate == -1)
        radio_sampling_rate = (RadId == 0) ? SAMPLING_RATE_5MHZ : SAMPLING_RATE_10MHZ; // set 7680 as default for CMOS and 15360 as default for LVDS
    else {
        if (radio_sampling_rate != SAMPLING_RATE_5MHZ && radio_sampling_rate != SAMPLING_RATE_10MHZ && radio_sampling_rate != SAMPLING_RATE_20MHZ)
            pabort("Invalid smpling rate. Supports only 7680, 15360 and 30720");
    }

    RadConfig.SamplingRate = radio_sampling_rate;

    if (radio_loopback == 0) {
        RadConfig.TxLoopback = 0;
    }
    else if (radio_loopback == 1)
        RadConfig.TxLoopback = 1;
    else
        pabort("Invalid radio loopback mode. Supports only 0-no loopback, 1-Tx loopback ");

    fd_spi = open(spi_device[RadId], O_RDWR);
    if (fd_spi < 0) {
        printf("Can't open %s RadId. ", spi_device[RadId]);
        abort();
    }

    if (radio_init) {
	    printf("Setting up radio 1 using Band7 %d Mhz LTE LVDS profile", (SAMPLING_RATE_20MHZ==radio_sampling_rate)? 20:((SAMPLING_RATE_10MHZ==radio_sampling_rate)? 10:5));

	    printf("\n%s Mode, NumAntennas=%d, TxLoopback=%d\n", (radio_mode == RADMODE_LEVEL) ? "Level" : "Pulse", RadConfig.NumAntennas, RadConfig.TxLoopback);

	    spi_config(fd_spi);

	    if (radio_reset_request)
		    radio_reset();
    }

    if (radio_test_time) {
        fd_rad = open(rad_device[RadId], O_RDWR);
        ret = ioctl(fd_rad,TRAD_IOCTL_INIT_SYS ,NULL);

        if (fd_rad < 0) {
            printf("Can't open %s RadId. ", rad_device[RadId]);
            abort();
        }
		if(0 == type_cluster) //only master init 9361
		{
			printf("Initializing AD9361 ...\n");
			if ((fp_load = fopen(".load","a+")) == NULL) {
				printf("open fail\n");
				exit(1);
			}
			i = 0;
			while (fscanf(fp_load,"%u %u %u",&load_data[i].OpCode,&load_data[i].Addr,&load_data[i].Data) > 0) {
				i++;
			};
			RadConfig.pRadProfile = (PADRADPROFILE)load_data;
			Ad9361RadioInit(fd_spi, &RadConfig);
			if (radio_mode == RADMODE_LEVEL)
				Ad9361WriteReg(fd_spi, 0x14, 0x23); // for level mode move radio to FDD active state
			else
				Ad9361WriteReg(fd_spi, 0x14, 0x13); // for pulse mode enable pin control
			fclose(fp_load);
		}
#ifdef PCIE
			fd_pcie = open("/dev/pciedrv", O_RDWR);
			if (fd_pcie < 0) {
				printf("Can't open /dev/pciedrv.\n");
				abort();
			}
			printf("fd_pcie is %d\n",fd_pcie);
			if(0 == type_cluster)
			{
				pcie_data = 0xd0;
				write(fd_pcie, &pcie_data, sizeof(pcie_data));
				printf("Init 9361 OK.\nMaster writing 0xd0 to PCIe ADDR. 0x0\n");
			}
			else
			{
				pcie_data_back[0] = 0;
				printf("Waiting Master init 9361, and reading PCIe to make sure\n");
				//while(pcie_data != 0xd0)
				{
					read(fd_pcie, pcie_data_back, sizeof(pcie_data));
					printf("%d ",pcie_data_back[0]);
					usleep(1000);
				}
				printf("Slave read 0xd0 from PCIe ADDR. 0x0\nInit 9361 OK.\n");
			}
			close(fd_pcie);
#endif
		printf("This %s Cluster, config & enable\n", (RadConfig.TypeCluster==0) ? "Master":"Slave");
        ret = ioctl(fd_rad, TRAD_IOCTL_SET_RAD_CONFIG, &RadConfig);
        if (ret)
			pabort("Radio Config IOCTL failed");
		else
			printf("%s Radio Config OK\n", (RadConfig.TypeCluster==0) ? "Master":"Slave");

        ret = ioctl(fd_rad, TRAD_IOCTL_ENABLE, 0);
        if (ret) 
            printf("Radio enable fails.\n");
		else
			printf("%s Radio Enable OK\n", (RadConfig.TypeCluster==0) ? "Master":"Slave");
        


	/*
	 * 9361自发数据
	Ad9361WriteReg(fd_spi, 0x14, 0x23); 
	Ad9361WriteReg(fd_spi, 0x3f4, 0x0b); 
	 */
        close(fd_rad);

    }

    close(fd_spi);



    return ret;
}

static void radio_reset(void)
{
    int fd;
    volatile unsigned int *gpio_regs;
    volatile unsigned int *radio_control_regs;

    fd = open("/dev/mem", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Can't open /dev/mem, errno: %d (%s)\n", errno, strerror(errno));
    }

    gpio_regs = mmap(NULL, 256, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0xFE070000);
    if (gpio_regs == MAP_FAILED) {
        fprintf(stderr, "Can't mmap /dev/mem at address %08X, errno: %d (%s)\n", 0xFE070000, errno, strerror(errno));
    }

    radio_control_regs = mmap(NULL, 32, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0xF5E01000);
    if (radio_control_regs == MAP_FAILED) {
        fprintf(stderr, "Can't mmap /dev/mem at address %08X, errno: %d (%s)\n", 0xF5E01000, errno, strerror(errno));
    }

    *(gpio_regs + 0x04) |= 1; // set GPI_0 to output enable
    *gpio_regs &= ~1; // set GPI_0 to 0

    // Take Radio 0 and 1 out of reset
    printf("\nRadio HW Reset via GPIO3\n\n");

    // MCLK reset
    *(radio_control_regs+4) = 3; // 0xF5E01010
    usleep(1000);
    *(radio_control_regs+4) = 0;

    *(gpio_regs + 1) |= 1<<3;   // GPIO3 controls Radio module reset (0xFE070004)
    *(gpio_regs + 0) &= ~(1<<3); // set it low to reset
    usleep(10000);
    *(gpio_regs + 0) |= 1<<3; // set it high to put it out of reset
    usleep(10000);

    if (munmap((void *)gpio_regs, 256) == -1) {
        fprintf(stderr, "Error un-mmapping the file");
    }

    if (munmap((void *)radio_control_regs, 32) == -1) {
        fprintf(stderr, "Error un-mmapping the file");
    }
    close(fd);
}

void print_jdma_regs(PJDMAREGS pjdmaregs)
{
	JDMAREGS jdmaregs = *pjdmaregs;
	printf("Tx_Control                  : 0x%08x\n",jdmaregs.TxControl);
	printf("Tx_Status                   : 0x%08x\n",jdmaregs.TxStatus);
	printf("Tx_Filter                   : 0x%08x\n",jdmaregs.TxFiller);
	printf("Tx_Res0                     : 0x%08x\n",jdmaregs.TxRes0);
	printf("TxTemplate0.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[0].TrSize);
	printf("TxTemplate0.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[0].TrCtrl);
	printf("TxTemplate0.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[0].SymOffset);
	printf("TxTemplate0.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[0].Res0);
	printf("TxTemplate0.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[1].TrSize);
	printf("TxTemplate0.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[1].TrCtrl);
	printf("TxTemplate0.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[1].SymOffset);
	printf("TxTemplate0.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[1].Res0);
	printf("TxTemplate0.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[2].TrSize);
	printf("TxTemplate0.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[2].TrCtrl);
	printf("TxTemplate0.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[2].SymOffset);
	printf("TxTemplate0.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[2].Res0);
	printf("TxTemplate0.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate0.Desc[3].TrSize);
	printf("TxTemplate0.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate0.Desc[3].TrCtrl);
	printf("TxTemplate0.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate0.Desc[3].SymOffset);
	printf("TxTemplate0.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate0.Desc[3].Res0);
	printf("TxTemplate1.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[0].TrSize);
	printf("TxTemplate1.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[0].TrCtrl);
	printf("TxTemplate1.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[0].SymOffset);
	printf("TxTemplate1.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[0].Res0);
	printf("TxTemplate1.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[1].TrSize);
	printf("TxTemplate1.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[1].TrCtrl);
	printf("TxTemplate1.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[1].SymOffset);
	printf("TxTemplate1.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[1].Res0);
	printf("TxTemplate1.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[2].TrSize);
	printf("TxTemplate1.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[2].TrCtrl);
	printf("TxTemplate1.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[2].SymOffset);
	printf("TxTemplate1.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[2].Res0);
	printf("TxTemplate1.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate1.Desc[3].TrSize);
	printf("TxTemplate1.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate1.Desc[3].TrCtrl);
	printf("TxTemplate1.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate1.Desc[3].SymOffset);
	printf("TxTemplate1.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate1.Desc[3].Res0);
	printf("TxTemplate2.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[0].TrSize);
	printf("TxTemplate2.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[0].TrCtrl);
	printf("TxTemplate2.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[0].SymOffset);
	printf("TxTemplate2.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[0].Res0);
	printf("TxTemplate2.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[1].TrSize);
	printf("TxTemplate2.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[1].TrCtrl);
	printf("TxTemplate2.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[1].SymOffset);
	printf("TxTemplate2.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[1].Res0);
	printf("TxTemplate2.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[2].TrSize);
	printf("TxTemplate2.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[2].TrCtrl);
	printf("TxTemplate2.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[2].SymOffset);
	printf("TxTemplate2.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[2].Res0);
	printf("TxTemplate2.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate2.Desc[3].TrSize);
	printf("TxTemplate2.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate2.Desc[3].TrCtrl);
	printf("TxTemplate2.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate2.Desc[3].SymOffset);
	printf("TxTemplate2.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate2.Desc[3].Res0);
	printf("TxTemplate3.Desc0.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[0].TrSize);
	printf("TxTemplate3.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[0].TrCtrl);
	printf("TxTemplate3.Desc0.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[0].SymOffset);
	printf("TxTemplate3.Desc0.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[0].Res0);
	printf("TxTemplate3.Desc1.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[1].TrSize);
	printf("TxTemplate3.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[1].TrCtrl);
	printf("TxTemplate3.Desc1.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[1].SymOffset);
	printf("TxTemplate3.Desc1.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[1].Res0);
	printf("TxTemplate3.Desc2.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[2].TrSize);
	printf("TxTemplate3.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[2].TrCtrl);
	printf("TxTemplate3.Desc2.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[2].SymOffset);
	printf("TxTemplate3.Desc2.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[2].Res0);
	printf("TxTemplate3.Desc3.Trsize    : 0x%04x\n",jdmaregs.TxTemplate3.Desc[3].TrSize);
	printf("TxTemplate3.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.TxTemplate3.Desc[3].TrCtrl);
	printf("TxTemplate3.Desc3.SymOffset : 0x%04x\n",jdmaregs.TxTemplate3.Desc[3].SymOffset);
	printf("TxTemplate3.Desc3.Res0      : 0x%01x\n",jdmaregs.TxTemplate3.Desc[3].Res0);
	printf("TxSymCtrl0.TslEntry0        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[0]);
	printf("TxSymCtrl0.TslEntry1        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[1]);
	printf("TxSymCtrl0.TslEntry2        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[2]);
	printf("TxSymCtrl0.TslEntry3        : 0x%08x\n",jdmaregs.TxSymCtrl0.TslEntry[3]);
	printf("TxSymCtrl0.BasePtr          : 0x%08x\n",jdmaregs.TxSymCtrl0.BasePtr);
	printf("TxSymCtrl0.CurSymBufPtr     : 0x%08x\n",jdmaregs.TxSymCtrl0.CurSymBufPtr);
	printf("TxSymCtrl0.ShortStride      : 0x%08x\n",jdmaregs.TxSymCtrl0.ShortStride);
	printf("TxSymCtrl0.LongStride       : 0x%08x\n",jdmaregs.TxSymCtrl0.LongStride);
	printf("TxSymCtrl0.Res              : 0x%08x\n",jdmaregs.TxSymCtrl0.Res);
	printf("TxSymCtrl0.IrqSync          : 0x%08x\n",jdmaregs.TxSymCtrl0.IrqSync);
	printf("TxSymCtrl0.Res1.0           : 0x%08x\n",jdmaregs.TxSymCtrl0.Res1[0]);
	printf("TxSymCtrl0.Res1.1           : 0x%08x\n",jdmaregs.TxSymCtrl0.Res1[1]);
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
	printf("jdmaregs.TxRes4.[0-21]               : \n");
	printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.TxRes4[0],jdmaregs.TxRes4[1],jdmaregs.TxRes4[2],jdmaregs.TxRes4[3],jdmaregs.TxRes4[4],jdmaregs.TxRes4[5],jdmaregs.TxRes4[6]);
	printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.TxRes4[7],jdmaregs.TxRes4[8],jdmaregs.TxRes4[9],jdmaregs.TxRes4[10],jdmaregs.TxRes4[11],jdmaregs.TxRes4[12],jdmaregs.TxRes4[13]);
	printf("0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x	0x%08x\n",jdmaregs.TxRes4[14],jdmaregs.TxRes4[15],jdmaregs.TxRes4[16],jdmaregs.TxRes4[17],jdmaregs.TxRes4[18],jdmaregs.TxRes4[19],jdmaregs.TxRes4[20]);
	printf("Rx_Control                  : 0x%08x\n",jdmaregs.RxControl);
	printf("Rx_Status                   : 0x%08x\n",jdmaregs.RxStatus);
	printf("Rx_Res0[1]                  : 0x%08x\n",jdmaregs.RxRes0[0]);
	printf("Rx_Res0[2]                  : 0x%08x\n",jdmaregs.RxRes0[1]);
	printf("RxTemplate0.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[0].TrSize);
	printf("RxTemplate0.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[0].TrCtrl);
	printf("RxTemplate0.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[0].SymOffset);
	printf("RxTemplate0.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[0].Res0);
	printf("RxTemplate0.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[1].TrSize);
	printf("RxTemplate0.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[1].TrCtrl);
	printf("RxTemplate0.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[1].SymOffset);
	printf("RxTemplate0.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[1].Res0);
	printf("RxTemplate0.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[2].TrSize);
	printf("RxTemplate0.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[2].TrCtrl);
	printf("RxTemplate0.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[2].SymOffset);
	printf("RxTemplate0.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[2].Res0);
	printf("RxTemplate0.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate0.Desc[3].TrSize);
	printf("RxTemplate0.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate0.Desc[3].TrCtrl);
	printf("RxTemplate0.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate0.Desc[3].SymOffset);
	printf("RxTemplate0.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate0.Desc[3].Res0);
	printf("RxTemplate1.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[0].TrSize);
	printf("RxTemplate1.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[0].TrCtrl);
	printf("RxTemplate1.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[0].SymOffset);
	printf("RxTemplate1.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[0].Res0);
	printf("RxTemplate1.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[1].TrSize);
	printf("RxTemplate1.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[1].TrCtrl);
	printf("RxTemplate1.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[1].SymOffset);
	printf("RxTemplate1.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[1].Res0);
	printf("RxTemplate1.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[2].TrSize);
	printf("RxTemplate1.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[2].TrCtrl);
	printf("RxTemplate1.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[2].SymOffset);
	printf("RxTemplate1.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[2].Res0);
	printf("RxTemplate1.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate1.Desc[3].TrSize);
	printf("RxTemplate1.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate1.Desc[3].TrCtrl);
	printf("RxTemplate1.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate1.Desc[3].SymOffset);
	printf("RxTemplate1.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate1.Desc[3].Res0);
	printf("RxTemplate2.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[0].TrSize);
	printf("RxTemplate2.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[0].TrCtrl);
	printf("RxTemplate2.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[0].SymOffset);
	printf("RxTemplate2.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[0].Res0);
	printf("RxTemplate2.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[1].TrSize);
	printf("RxTemplate2.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[1].TrCtrl);
	printf("RxTemplate2.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[1].SymOffset);
	printf("RxTemplate2.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[1].Res0);
	printf("RxTemplate2.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[2].TrSize);
	printf("RxTemplate2.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[2].TrCtrl);
	printf("RxTemplate2.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[2].SymOffset);
	printf("RxTemplate2.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[2].Res0);
	printf("RxTemplate2.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate2.Desc[3].TrSize);
	printf("RxTemplate2.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate2.Desc[3].TrCtrl);
	printf("RxTemplate2.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate2.Desc[3].SymOffset);
	printf("RxTemplate2.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate2.Desc[3].Res0);
	printf("RxTemplate3.Desc0.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[0].TrSize);
	printf("RxTemplate3.Desc0.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[0].TrCtrl);
	printf("RxTemplate3.Desc0.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[0].SymOffset);
	printf("RxTemplate3.Desc0.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[0].Res0);
	printf("RxTemplate3.Desc1.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[1].TrSize);
	printf("RxTemplate3.Desc1.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[1].TrCtrl);
	printf("RxTemplate3.Desc1.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[1].SymOffset);
	printf("RxTemplate3.Desc1.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[1].Res0);
	printf("RxTemplate3.Desc2.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[2].TrSize);
	printf("RxTemplate3.Desc2.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[2].TrCtrl);
	printf("RxTemplate3.Desc2.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[2].SymOffset);
	printf("RxTemplate3.Desc2.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[2].Res0);
	printf("RxTemplate3.Desc3.Trsize    : 0x%04x\n",jdmaregs.RxTemplate3.Desc[3].TrSize);
	printf("RxTemplate3.Desc3.TrCtrl    : 0x%01x\n",jdmaregs.RxTemplate3.Desc[3].TrCtrl);
	printf("RxTemplate3.Desc3.SymOffset : 0x%04x\n",jdmaregs.RxTemplate3.Desc[3].SymOffset);
	printf("RxTemplate3.Desc3.Res0      : 0x%01x\n",jdmaregs.RxTemplate3.Desc[3].Res0);
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
}
void print_jesd_regs(PJESDREGS pjesdregs)
{
	JESDREGS jesdregs = *pjesdregs;
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
}

void print_tm_regs(PRADTMREGS p_rad_tm_regs)
{
	int i,j;
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
		printf("-------------------------------------------New FSyncTimers-----------------------------------------\n");
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

void print_iq_counter_regs(PIQCONFIG pIqCntCfg)
{
	printf("iq counter register\n");
	printf("IntEnable:%u\n",pIqCntCfg->IntEnable);
	printf("OvfIntEnable:%u\n",pIqCntCfg->OvfIntEnable);
	printf("EnableCounter:%u\n",pIqCntCfg->EnableCounter);
	printf("EnableClearingCounters:%u\n",pIqCntCfg->EnableClearingCounters);
	printf("res0:%u\n",pIqCntCfg->res0);
	printf("OvfInd:%u\n",pIqCntCfg->OvfInd);
	printf("res1:%u\n",pIqCntCfg->res1);
	printf("TrCnt:%u\n",pIqCntCfg->TrCnt);
	printf("TrCntMaxVal:%u\n",pIqCntCfg->TrCntMaxVal);
	printf("IntTrigVal:%u\n",pIqCntCfg->IntTrigVal);
	printf("IntTrigMask:%u\n",pIqCntCfg->IntTrigMask);
	printf("ProgDelayCount:%u\n",pIqCntCfg->ProgDelayCount);
	printf("res2:%u\n",pIqCntCfg->res2);
	printf("CntInitVal:%u\n",pIqCntCfg->CntInitVal);
}

unsigned int print_syncnet_regs(PSYNCNET_CONFIG psyncnet_regs)
{
	printf("0.soft_reset:0x%08x\n",psyncnet_regs->soft_reset);
	printf("1.control:0x%08x\n",psyncnet_regs->control);
	printf("2.reserved[0]:0x%08x\n",psyncnet_regs->reserved[0]);
	printf("3.reserved[1]:0x%08x\n",psyncnet_regs->reserved[1]);
	printf("4.ul_acc_clear:0x%08x\n",psyncnet_regs->ul_acc_clear);
	printf("5.ul_acc_maxcount:0x%08x\n",psyncnet_regs->ul_acc_maxcount);
	printf("6.ul_cpdma_acc_status:0x%08x\n",psyncnet_regs->ul_cpdma_acc_status);
	printf("7.ul_rpdif_acc_status:0x%08x\n",psyncnet_regs->ul_rpdif_acc_status);
	printf("8.ul_bufcount_select:0x%08x\n",psyncnet_regs->ul_bufcount_select);
	printf("9.ul_bufcount_window:0x%08x\n",psyncnet_regs->ul_bufcount_window);
	printf("reserved1:\n");
	printf("0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x\n",psyncnet_regs->reserved1[0],psyncnet_regs->reserved1[1],psyncnet_regs->reserved1[2],psyncnet_regs->reserved1[3],psyncnet_regs->reserved1[4],psyncnet_regs->reserved1[5]);
	printf("16.dl_acc_clear:0x%08x\n",psyncnet_regs->dl_acc_clear);
	printf("17.dl_acc_maxcount:0x%08x\n",psyncnet_regs->dl_acc_maxcount);
	printf("18.dl_cpdma_acc_status:0x%08x\n",psyncnet_regs->dl_cpdma_acc_status);
	printf("19.dl_rpdif_acc_status:0x%08x\n",psyncnet_regs->dl_rpdif_acc_status);
	printf("20.dl_bufcount_select:0x%08x\n",psyncnet_regs->dl_bufcount_select);
	printf("21.dl_bufcount_window:0x%08x\n",psyncnet_regs->dl_bufcount_window);
	printf("22.dl_req_trigby_arm:0x%08x\n",psyncnet_regs->dl_req_trigby_arm);
	printf("23.reserved2:0x%08x\n",psyncnet_regs->reserved2);
	printf("24.total_acc_maxcount:0x%08x\n",psyncnet_regs->total_acc_maxcount);
	printf("25.total_acc_count_stat:0x%08x\n",psyncnet_regs->total_acc_count_stat);
	printf("26.ul_irq_enable:0x%08x\n",psyncnet_regs->ul_irq_enable);
	printf("27.dl_irq_enable:0x%08x\n",psyncnet_regs->dl_irq_enable);
	printf("28.ul_irq_status:0x%08x\n",psyncnet_regs->ul_irq_status);
	printf("29.dl_irq_status:0x%08x\n",psyncnet_regs->dl_irq_status);
	printf("30.reserved3[0]:0x%08x\n",psyncnet_regs->reserved3[0]);
	printf("31.reserved3[1]:0x%08x\n",psyncnet_regs->reserved3[1]);
	printf("32.ceva2arm_req_select:0x%08x\n",psyncnet_regs->ceva2arm_req_select);
	printf("33.map_req_select:0x%08x\n",psyncnet_regs->map_req_select);
	printf("34.update_iqcount_select:0x%08x\n",psyncnet_regs->update_iqcount_select);
	printf("35.irq_ceva0_select:0x%08x\n",psyncnet_regs->irq_ceva0_select);
	printf("36.irq_ceva1_select:0x%08x\n",psyncnet_regs->irq_ceva1_select);
	printf("37.tdone2arm_and_req_select0:0x%08x\n",psyncnet_regs->tdone2arm_and_req_select0);
	printf("38.tdone2arm_and_req_select1:0x%08x\n",psyncnet_regs->tdone2arm_and_req_select1);
	printf("39.tdone2arm_or_req_select0:0x%08x\n",psyncnet_regs->tdone2arm_or_req_select0);
	printf("40.tdone2arm_or_req_select1:0x%08x\n",psyncnet_regs->tdone2arm_or_req_select1);
	printf("41.req_proc2psc0_select:0x%08x\n",psyncnet_regs->req_proc2psc0_select);
	printf("42.req_proc2rpd_select:0x%08x\n",psyncnet_regs->req_proc2rpd_select);
	printf("43.req_proc2cort0_select:0x%08x\n",psyncnet_regs->req_proc2cort0_select);
	printf("44.req_proc2map_select:0x%08x\n",psyncnet_regs->req_proc2map_select);
}
