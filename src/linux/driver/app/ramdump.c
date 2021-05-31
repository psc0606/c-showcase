/*
 * read and wirte data to cram,iram,ddr
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>

/*
//actually, 0x10000 is the baseaddr of external ddr
#define BASE_ADDR	0x10000      //ddr base address
#define RAM_SIZE	0x7FFFFFFFUL //ddr size=2G bytes UL=unsigned long
*/

#define CMD_BUFF_SIZE	512	
#define CMD_PARAM_CNT	3	
#define CMD_PARAM_WIDE	8

//四舍五入
#define ROUND(x, dx)  (((x) + ((dx) - 1) ) & ~((dx) - 1))
//#define u64 u64
#define DEV_NAME	"/dev/mem"

//print prompt提示符
#define CON_PROMOT() do { printf("RAM_DUMP>"); }while(0)
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef unsigned long  u64;

u64 BASE_ADDR;
u64 RAM_SIZE;

/*
#define IRAM_BASEADDR	0xF4000000
#define IRAM_SIZE	0x00040000
#define CRAM_BASEADDR   0xF3000000
#define CRAM_SIZE       0x000C0000
*/

//insure addr escape range
#define ADDR_IN_XRAM(p,base,len) (((p) >= (base)) && ((p) < ((base) + (len))))

#if 0
enum {
	/*
	 * default = 0, then increase one by one
	 */
	MEM_TYPE_UNKNOWN,
	MEM_TYPE_IRAM,
	MEM_TYPE_CRAM,
	MEM_TYPE_MDMA,
	MEM_TYPE_DDR,
};
#endif

u32 map_size; 	     //mmap size
u64 map_base_addr;   //mmap base size
int ram_type;
int disp_type;

int operate_ram(u64, u8*, u32, u32);
int parsercmd(u8*);
int get_ram_type(u64);
void procccmd(u32,u64*,int);

int main(int argc, char* argv[])
{
	int ibrk = 0;
	char szcmdbuf[CMD_BUFF_SIZE];
	do {
		printf("Please enter ram type:\n\t<1> iram\n\t<2> cram\n\t<3> ddr\nEnter:");
		scanf("%d", &ram_type);

		switch(ram_type)
		{
		case 1: //iram
			BASE_ADDR = 0xF4000000;
			RAM_SIZE  = 0x00040000;
			break;
		case 2: //cram
			BASE_ADDR = 0xF3000000;
			RAM_SIZE  = 0x000C0000;
			break;
		case 3: //ddr
			BASE_ADDR = 0x00010000;
			RAM_SIZE  = 0x7FFFFFFF;
			break;
		default:
			printf("unkown ram type\n");
			return -1;
		}

		CON_PROMOT();
		getchar();
		gets(szcmdbuf);
		putchar('a');
		puts(szcmdbuf);
		ibrk = parsercmd(szcmdbuf);
	}while(!ibrk);
	return 0;
}

int parsercmd(u8* szcmd)
{
	int icnt = 0;
	int page_size = 0;
	u64 ulparam[CMD_PARAM_CNT] = {0}; //u64 parameters
	u32 rwdflag = 0; // 0 means read,1 means write

	/*
	 * strok places a NULL terminator in front of the token, if found
	 */
	char* psztoken = strtok(szcmd," ");
	if (!psztoken) {
		printf("Invalid command!\n");
		return 0;
	}

	switch(psztoken[0])
	{
	case 'r'://read
		rwdflag = 0;
		break;
	case 'w'://write
		rwdflag = 1;
		break;
	case 'q'://quit
		return 1;
	default:
		printf("Invalid command!\n");
		return 0;
	}

	while(psztoken) {
		/*
		 * A second call to strtok using a NULL as the first parameter
		 * returns a pointer to the character following the token
		 */
		psztoken = strtok(NULL," ");
		if(NULL == psztoken)
		    break;

		/*
		 * stroul change char to u64
		 * ulparam[0]=baseaddr, ulparam[1]=size, ulparam[2]=type_flag
		 */
		ulparam[icnt++] = strtoul(psztoken,NULL,16);
		//printf("%ld,%ld\n", ulparam[0], ulparam[1]);
		if (icnt >= CMD_PARAM_CNT)
		    break;
	}
	
	/*
	 * many systerms are 4KB=0x1000B(page size)
	 */
	page_size = getpagesize();
	if(0 == (ulparam[0]%page_size)) {
		map_base_addr = ulparam[0];
		//printf("page size:%ld\n", page_size);
		//printf("map_base_addr1:0x%x\n", map_base_addr);
	}
	else {
		map_base_addr = ROUND(ulparam[0], page_size) - page_size;
		//printf("map_base_addr2:0x%x\n", map_base_addr);
	}
	if(0 == ulparam[1]) {
		printf("please input the number of size~\n");
		exit(1);
	}
	
	/*
	 * mamp():map_size must be the times of page_size
	 * when addr cross-pages跨页, should be careful!
	 */
	map_size = ((ulparam[0]%page_size + ulparam[1]) / page_size + 1)*page_size;
	
	switch(ulparam[2])
	{
		case 1://1 byte 
			disp_type = 1;
			break;
		case 2://2 bytes 
			disp_type = 2;
			break;
		case 4://4 bytes 
			disp_type = 4;
			break;
		default:
			disp_type = 4;
			break;
	}
	printf("map_base_addr:	0x%x\n", map_base_addr);
	printf("map_size:	0x%x\n", map_size);
	printf("disp_type:	%d\n", disp_type);

	procccmd(rwdflag,ulparam,icnt);
	return 0;
}

void procccmd(u32 flag, u64* ulparam, int nlen)
{
	char* pbuf = NULL;
	char* log_name = "./DDRinfo.log";
	u32 nbuflen = 0;
	FILE *f_log_file;

	f_log_file = fopen(log_name, "w+");

	do {
		if (nlen < 1 || nlen > CMD_BUFF_SIZE)
			break;
		//nbuflen = ROUND(ulparam[1],4*1024);
		//pbuf = (char*)malloc(sizeof(char)*nbuflen);
		
		//buf for read and write
		pbuf = (u8*)malloc(sizeof(u8)*ulparam[1]);
		
		//if (!operate_ram(ulparam[0],pbuf,nbuflen,flag)) {//operate success
		if (!operate_ram(ulparam[0], pbuf, ulparam[1], flag)) {//operate success
			if (0 == flag) {// read operation
				int i = 0;
			for (i = 0; i < ulparam[1]/4; i += 4) {
			//for (i = (ulparam[0]-map_base_addr) ; i < (ulparam[0]-map_base_addr+ulparam[1]/4) ;i += 4) {
			    switch(disp_type)
			    {
				case 1:
				    printf("0x%08x\t %02x  %02x  %02x  %02x \n", ulparam[0]+i, pbuf[i], pbuf[i+1], pbuf[i+2], pbuf[i+3]);
				    fprintf(f_log_file, "0x%02hx\n0x%02hx\n0x%02hx\n0x%02hx\n",((char)(*(char*)(&pbuf[i]))),((char)(*((char*)&pbuf[i+1]))), ((char)(*((char*)&pbuf[i+2]))), ((char)(*((char*)&pbuf[i+3]))));
				    break;
				case 2:
				    printf("0x%08x\t %04x  %04x\n", ulparam[0]+i,(*(u16*)&pbuf[i]),(*(u16*)&pbuf[i + 2]));
				    fprintf(f_log_file, "0x%04hx\n0x%04hx \n",((u16)(*(u16*)(&pbuf[i]))),((u16)(*((u16*)&pbuf[i+2]))));
				    break;
				case 4:// read from offsize, not map_base_addr
				    printf("0x%08x\t %08x\n", ulparam[0]+i,(*(u64*)&pbuf[i]));
				    fprintf(f_log_file, "0x%08x\n",(*(u64*)(&pbuf[i])));
				    break;
			    }
			}
		    }
		}
	}while(0);

	fprintf(f_log_file, "**************************************************************\n\n\n");

	if (pbuf)
	    free(pbuf);
	if(f_log_file)
	    fclose(f_log_file);
}

int operate_ram(u64 addr,u8* pbuf,u32 nbufsize,u32 rwflag)
{
	int fd = -1, i = 0;
	u8* pszMap = NULL;
	u32 offsize = addr - map_base_addr;
	
	do {
	//printf("phy_addr:0x%x,nbufsize:0x%x, offsize:0x%x\n", addr, nbufsize, offsize);
	
		//if(((map_base_addr+map_size) >= BASE_ADDR+RAM_SIZE) ||
			//(addr < BASE_ADDR)) {
		//printf("addr:0x%x, base_addr:0x%x, ram_size:0x%x\n", addr, BASE_ADDR, RAM_SIZE);
		if(!ADDR_IN_XRAM(addr,BASE_ADDR,RAM_SIZE)) {
			
			printf("Invalid address range!\n");
			return -1;
		}
		if((fd = open(DEV_NAME, O_RDWR) ) < 0) {
			printf("Open Device[%s] failed!\n",DEV_NAME);
			break;
		}
		
		// MAP_FAILED kerneal
		if(MAP_FAILED == (pszMap = mmap(NULL, map_size,
					PROT_READ | PROT_WRITE,MAP_SHARED,fd,map_base_addr) )) {
			printf("Map %s at address 0x%08x failed!\n",DEV_NAME,map_base_addr);
			break;
		}
		printf("mmap ok, virtual addr:0x%x,phy addr:0x%x\n", pszMap, map_base_addr);
		if (rwflag) {// write
			for (i = 0; i < nbufsize; ++i)
				pszMap[offsize + i] = pbuf[i];
		}else {//read size of pages
			for (i = 0; i < nbufsize; ++i)
				pbuf[i] = pszMap[offsize+i];
		}
	}while(0);

	if (pszMap) //unmap
		munmap(pszMap, map_size);
	if (fd >= 0)
		close(fd);
	return 0;
}
