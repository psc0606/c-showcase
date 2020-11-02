#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
//#include "exp.h"
#define EXP_BASE 0xfe100000
int main()
{
    int fd_mem;
    fd_mem = open("/dev/mem",O_RDWR);
    if(fd_mem < 0)
	printf("ERROR\n");
    else
    {
    volatile unsigned int *exp_base = mmap(NULL, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, fd_mem, EXP_BASE);
    printf("-------------------- EXP_BUS_REGISTER --------------------\n");
    printf("\t\tEX_SWRST_R	:0x%08x\n",*exp_base);
    printf("\t\tEX_CSEN_R 	:0x%08x\n",  *(exp_base+0x04/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CS0_BASE_R   :0x%08x\n",*(exp_base+0x08/4));
    printf("\t\tEX_CS1_BASE_R   :0x%08x\n",*(exp_base+0x0c/4));
    printf("\t\tEX_CS2_BASE_R   :0x%08x\n",*(exp_base+0x10/4));
    printf("\t\tEX_CS3_BASE_R   :0x%08x\n",*(exp_base+0x14/4));
    printf("\t\tEX_CS4_BASE_R   :0x%08x\n",*(exp_base+0x18/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CS0_SEG_R    :0x%08x\n",*(exp_base+0x1c/4));
    printf("\t\tEX_CS1_SEG_R    :0x%08x\n",*(exp_base+0x20/4));
    printf("\t\tEX_CS2_SEG_R    :0x%08x\n",*(exp_base+0x24/4));
    printf("\t\tEX_CS3_SEG_R    :0x%08x\n",*(exp_base+0x28/4));
    printf("\t\tEX_CS4_SEG_R    :0x%08x\n",*(exp_base+0x2c/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CS0_CFG_R    :0x%08x\n",*(exp_base+0x30/4));
    printf("\t\tEX_CS1_CFG_R    :0x%08x\n",*(exp_base+0x34/4));
    printf("\t\tEX_CS2_CFG_R    :0x%08x\n",*(exp_base+0x38/4));
    printf("\t\tEX_CS3_CFG_R    :0x%08x\n",*(exp_base+0x3c/4));
    printf("\t\tEX_CS4_CFG_R    :0x%08x\n",*(exp_base+0x40/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CS0_TMG1_R   :0x%08x\n",*(exp_base+0x44/4));
    printf("\t\tEX_CS1_TMG1_R   :0x%08x\n",*(exp_base+0x48/4));
    printf("\t\tEX_CS2_TMG1_R   :0x%08x\n",*(exp_base+0x4c/4));
    printf("\t\tEX_CS3_TMG1_R   :0x%08x\n",*(exp_base+0x50/4));
    printf("\t\tEX_CS4_TMG1_R   :0x%08x\n",*(exp_base+0x54/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CS0_TMG2_R   :0x%08x\n",*(exp_base+0x58/4));
    printf("\t\tEX_CS1_TMG2_R   :0x%08x\n",*(exp_base+0x5c/4));
    printf("\t\tEX_CS2_TMG2_R   :0x%08x\n",*(exp_base+0x60/4));
    printf("\t\tEX_CS3_TMG2_R   :0x%08x\n",*(exp_base+0x64/4));
    printf("\t\tEX_CS4_TMG2_R   :0x%08x\n",*(exp_base+0x68/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CS0_TMG3_R   :0x%08x\n",*(exp_base+0x6c/4));
    printf("\t\tEX_CS1_TMG3_R   :0x%08x\n",*(exp_base+0x70/4));
    printf("\t\tEX_CS2_TMG3_R   :0x%08x\n",*(exp_base+0x74/4));
    printf("\t\tEX_CS3_TMG3_R   :0x%08x\n",*(exp_base+0x78/4));
    printf("\t\tEX_CS4_TMG3_R   :0x%08x\n",*(exp_base+0x7c/4));
    printf("----------------------------------------------------------\n");
    printf("\t\tEX_CLOCK_DIV_R  :0x%08x\n",*(exp_base+0x80/4));
    printf("\t\tEX_MFSM_R	:0x%08x\n",*(exp_base+0x100/4));
    printf("\t\tEX_CSFSM_R 	:0x%08x\n",*(exp_base+0x104/4));
    printf("\t\tEX_WRFSM_R 	:0x%08x\n",*(exp_base+0x108/4));
    printf("\t\tEX_RDFSM_R 	:0x%08x\n",*(exp_base+0x10c/4));
    }
    close(fd_mem);
    return 0;
}
