/********************************************************************************
*       uartdrv.h                                                              *
*                                                                              *
*       This file contains UART 16550 driver header                           *
*                                                                              *
*******************************************************************************/ 
#ifndef __UARTDRV_H__
#define __UARTDRV_H__

#ifndef _UINT8_
#define _UINT8_
typedef unsigned char UINT8;
#endif /*_UINT8*/

#ifndef _UINT32_
#define _UINT32_
typedef unsigned int UINT32;
#endif /*_UINT32_*/

#define UART_DEVICE_NAME  "uartdrv"

#define UART0_BASEADDR                  0xFE800000
#define UART1_BASEADDR                  0xFE090000
#define UART0_APB_SIZE                  (64*1024)
#define UART1_APB_SIZE                  (8*1024)

//#define UART_BAUD_RATE         115200   
#define UART_BAUD_RATE         9600
//#define UART_BAUD_RATE         4800
#define EXP_CLK                 140000000

#define LCR_CHAR_LEN_5      0x00
#define LCR_CHAR_LEN_6      0x01
#define LCR_CHAR_LEN_7      0x02
#define LCR_CHAR_LEN_8      0x03
#define LCR_ONE_STOP        0x00    /* One stop bit! - default */
#define LCR_TWO_STOP        0x04    /* Two stop bit! */
#define LCR_PEN             0x08    /* Parity Enable */
#define LCR_PARITY_NONE     0x00
#define LCR_EPS             0x10    /* Even Parity Select */
#define LCR_PS              0x20    /* Enable Parity  Stuff */
#define LCR_SBRK            0x40    /* Start Break */
#define LCR_PSB             0x80    /* Parity Stuff Bit */
#define LCR_DLAB            0x80    /* UART 16550 Divisor Latch Assess */

#define UART_DATA_SIZE  	LCR_CHAR_LEN_8    //0x03
#define UART_PARITY     	LCR_PARITY_NONE     //0x00
#define UART_STOP_BITS  	LCR_ONE_STOP      //0x00

/* UART 16550 FIFO Control Register */
#define FCR_FIFOEN          0x01
#define FCR_RCVRRES         0x02
#define FCR_XMITRES         0x04

/*return arguments*/
#define SUCCESS             0x00
#define UART_NOSYMB         0xA3

/*Get Bus Clock*/
#define AXI_CLK_CNTRL_0                 0xF4CF0040  /**< AXI Clock Control Register 0 */
#define AXI_CLK_CNTRL_1                 0xF4CF0044  /**< AXI Clock Control Register 1 */
#define AXI_CLK_CNTRL_2                 0xF4CF0048  /**<AXI Clock Control Register 2 */ 
#define AXI_CLK_DIV_CNTRL               0xF4CF004C  /**< AXI Clock Divider Control Register */

#define SYS_REF_CLK                      25

#define rPLL_M_LSB                   0xF4CF01C0
#define rPLL_M_MSB                   0xF4CF01C4
#define PLL_SIZE                        0x44

typedef struct 
{
	unsigned int Speed;    //the speed of RX/TX
	unsigned char ByteSize;  //uart byte size
	unsigned char StopBits;   //number of stop bits
	unsigned char Parity;   //parity
}UARTCFG, *PUARTCFG;

typedef struct
{
	unsigned int *BaseAddr;
//	UARTCFG Config;      //uart device config parameters
	unsigned char *Rx;
	unsigned char *Tx;
}UARTCTX, *PUARTCTX;

typedef struct tagUARTDLREGS
{
	volatile unsigned int dll;   //divisor latch (low)
	volatile unsigned int dlh;   //divisor latch (high)
}UARTDLREGS, *PUARTDLREGS;

typedef struct tagUARTREGS
{
	volatile unsigned int data;   //receive/transmit data register
	volatile unsigned int ier;   //interrupt enable register
	volatile unsigned int iir_fcr;  //interrupt identity register / FIFO control register
	volatile unsigned int lcr;   //line Control register
	volatile unsigned int mcr;  //modem control register
	volatile unsigned int lsr;  //line status register
	volatile unsigned int msr;  //modern staturs register
	volatile unsigned int sr;    //scratch register
}UARTREGS, *PUARTREGS;

/*************************************************
static int uartdrv_open(struct inode *inode, struct file *filp);
static int uartdrv_release(struct inode *inode, struct file *filp);
static ssize_t uartdrv_read(struct file *filp, char *buff, size_t len, loff_t *off);
static ssize_t uartdrv_write(struct file *filp, const char *buff, size_t len, loff_t *off);
//static long uartdrv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int __init uartdrv_init(void);
static void __exit uartdrv_exit(void);
**************************************************/

#endif  /*__UARTDRV_H__*/

