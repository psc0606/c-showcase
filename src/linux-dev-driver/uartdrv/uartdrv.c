/********************************************************************************
 *       uartdrv.c                                                              *
 *                                                                              *
 *       This file contains UART driver "C" code                                *
 *                                                                              *
 *******************************************************************************/

#include <linux/string.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/types.h>

#include <asm/io.h>
#include <asm/uaccess.h>

#include "uartdrv.h"

static int uartdrv_major;
static struct class *uartdrv_class;
static struct device *uartdrv_device;

static int dev_id;

PUARTCTX UartCtx;
unsigned int uartdata;

static unsigned int GetBusClock(void);
static int uartdrv_open(struct inode *inode, struct file *filp);
static int uartdrv_release(struct inode *inode, struct file *filp);
static ssize_t uartdrv_read(struct file *filp, char *buff, size_t len, loff_t *off);
static ssize_t uartdrv_write(struct file *filp, const char *buff, size_t len, loff_t *off);
//static long uartdrv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
static int __init uartdrv_init(void);
static void __exit uartdrv_exit(void);

static unsigned int GetBusClock(void)
{
	static volatile unsigned int *pAXI_CLK_CNTRL_0;
	static volatile unsigned int *pAXI_CLK_DIV_CNTRL;
	static volatile unsigned int *pPLL_M_MSB;
	static volatile unsigned int *pPLL_M_LSB;
	unsigned int pll_n, axi_div;
	unsigned int ret;

	pAXI_CLK_CNTRL_0 = (volatile unsigned int *)ioremap_nocache(AXI_CLK_CNTRL_0, 4);
	pAXI_CLK_DIV_CNTRL = (volatile unsigned int *)ioremap_nocache(AXI_CLK_DIV_CNTRL, 4);
	pPLL_M_LSB = (volatile unsigned int *)ioremap_nocache(rPLL_M_LSB, PLL_SIZE);
	pPLL_M_MSB = (volatile unsigned int *)ioremap_nocache(rPLL_M_MSB, PLL_SIZE);

	pll_n = ((*pAXI_CLK_CNTRL_0) >> 1) & 0x7;
	printk("pll_n is: %d\n", pll_n);
	axi_div = *pAXI_CLK_DIV_CNTRL;
	ret = SYS_REF_CLK * (((*(pPLL_M_MSB+pll_n*0x8)) << 8) | (*(pPLL_M_LSB+pll_n*0x8))) / 5 / axi_div;
	
	iounmap(pAXI_CLK_CNTRL_0);
	iounmap(pAXI_CLK_DIV_CNTRL);
	iounmap(pPLL_M_MSB);
	iounmap(pPLL_M_LSB);
	return ret;
}

static int uartdrv_open(struct inode *inode, struct file *filp)
{
	unsigned int latch_value;
	PUARTDLREGS pUartLatch;
	PUARTREGS pUart;
	unsigned int bclock;
	UartCtx =  kmalloc(sizeof(UARTCTX), GFP_KERNEL);
	memset(UartCtx,0, sizeof(UARTCTX));
	UartCtx->BaseAddr = ioremap_nocache(UART1_BASEADDR, UART1_APB_SIZE);
	if(!UartCtx->BaseAddr)
		printk(KERN_WARNING "invaild ioremap");
	else
		printk("ioremap_nocache is ok!BaseAddr is %x\n", (unsigned int)UartCtx->BaseAddr);
	
	filp->private_data = UartCtx;

	//latch_value = (unsigned int)10*EXP_CLK / (unsigned int)(16 * UART_BAUD_RATE);
	bclock = GetBusClock();
	printk("get bus clock is: %d\n", bclock);
	latch_value = (unsigned int)10*(bclock * 1000000) / (unsigned int)(16 * UART_BAUD_RATE);
	if((latch_value % 10) > 5)
		latch_value = (latch_value / 10) +1;
	else
		latch_value = (latch_value / 10);
	printk("latch_value is:%x\n", latch_value);

	pUartLatch = (PUARTDLREGS)UartCtx->BaseAddr;
	pUart = (PUARTREGS)UartCtx->BaseAddr;
	
	printk("init 1 lsr is %x\n", pUart->lsr);

	//if UART was in use before (u-boot)
	pUart->ier = 0;                               //disable INT
	pUart->iir_fcr = 0x06;                         //reset and turn off FIFOs

	uartdata = pUart->data;    //read data byte	
	
	printk("init 2 lsr is %x\n", pUart->lsr);

	//set divisor latch
	pUart->lcr = LCR_DLAB;
	pUartLatch->dll = (latch_value & 0xFF);
	pUartLatch->dlh = (latch_value >> 8) & 0xFF;

	pUart->lcr = UART_DATA_SIZE | UART_STOP_BITS | UART_PARITY;  //data length is 8 bits  0x00000011
	pUart->iir_fcr = FCR_XMITRES | FCR_RCVRRES | FCR_FIFOEN;  //reset, FIFO enable, 0 thresholds

	printk("%s device open\n",UART_DEVICE_NAME);
	return 0;
}

static int uartdrv_release(struct inode *inode, struct file *filp)
{
	PUARTCTX pdev = (PUARTCTX)filp->private_data;
	iounmap(pdev->BaseAddr);
	printk("%s device release\n", UART_DEVICE_NAME);
	return 0;
}

//function read, parameter buff is empty, read date to buff, receive
static ssize_t uartdrv_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	PUARTCTX pCtx = (PUARTCTX)filp->private_data;
	PUARTREGS pUart = (PUARTREGS)pCtx->BaseAddr;
	unsigned int rxlength = 0;
	pCtx->Rx = (char *)kmalloc(sizeof(char)*len, GFP_KERNEL);
	memset(pCtx->Rx, 0x00, sizeof(char)*len);
	while(pUart->lsr & 0x01)
	{
	    (pCtx->Rx)[rxlength++] = pUart->data;
	    if(rxlength >= len)   break;
	    while((pUart->lsr & 0x01) == 0);
	}
	len = rxlength;
	if(len)
	{
	   if(copy_to_user(buff, (void *)pCtx->Rx, len)){
	       printk("read failed\n");
	       return -EFAULT;
	   }
	    printk("read length is: %d\n", len);
	    printk("read ok!\n");
	    return SUCCESS;
	}
	else
	    return UART_NOSYMB;

/*    	while(pUart->lsr & 0x01)
	{
	    if(copy_to_user(&buff[rxlength++], (char *)&(pUart->data),1)){
		printk("read failed\n");
		return -EFAULT;
	    }
	    else{
		printk("read ok!\n");
		printk("3. pUart->lsr is 0x%x\n", pUart->lsr);
	    }
	}*/
}

//equal to transmit
static ssize_t uartdrv_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	PUARTCTX pCtx =(PUARTCTX)filp->private_data;
	volatile PUARTREGS pUart = (PUARTREGS)pCtx->BaseAddr;
	unsigned int i = 0;
	pCtx->Tx = (char *)kmalloc(sizeof(char)*len, GFP_KERNEL);
	memset(pCtx->Tx, 0x00, sizeof(char)*len);

	if(copy_from_user((void *)pCtx->Tx,buff,len))
		return -EFAULT;

	while(len--)
		{
			pUart->data = (pCtx->Tx)[i++];
			while((pUart->lsr & 0x20) == 0);
		}
	printk("write ok!\n");
	return SUCCESS;
	
/*	if(copy_from_user((void *)&(pUart->data), buff, len))
	    return -EFAULT;
	else {
	    while((pUart->lsr & 0x20) == 0);
	    printk("pUart->lsr is 0x%x\n",pUart->lsr);
	    printk("write end!\n");
	    return 0;
	}*/
	/*	while(len--)
		{
			printk("write begin..............................\n");
			pUart->data = *buff++;
			printk("write the %d byte data\n", len);
			printk("1. pUart->lsr is 0x%x to pUart->lsr & 0x20 \n", (pUart->lsr));
			printk("pUart data is 0x%x\n",pUart->data);
			while((pUart->lsr & 0x20) == 0);
			printk("2. puart->lsr is 0x%x\n",pUart->lsr);
		}
	printk("write %d bytes data\n", length);*/
}

struct file_operations uartdrv_fops = {
	.owner = THIS_MODULE,
	.open =  uartdrv_open,
	.release =  uartdrv_release,
	.read =  uartdrv_read,
	.write = uartdrv_write,
//	.ioctl = uartdrv_ioctl;
};

static int __init uartdrv_init(void)
{
	uartdrv_major = register_chrdev(0, UART_DEVICE_NAME, &uartdrv_fops);
	if(uartdrv_major < 0)
		{
		//	printk("Registering %s device failed with %d\n", UART_DEVICE_NAME, uartdrv_major);
			return uartdrv_major;
		}
//	printk("%s device got %d major\n", UART_DEVICE_NAME,uartdrv_major);

	uartdrv_class = class_create(THIS_MODULE, UART_DEVICE_NAME);
	if(IS_ERR(uartdrv_class)){
		unregister_chrdev(uartdrv_major, UART_DEVICE_NAME);
		return PTR_ERR(uartdrv_class);
	}

	dev_id = MKDEV(uartdrv_major, 0);
	uartdrv_device = device_create(uartdrv_class, NULL, dev_id, NULL, UART_DEVICE_NAME);
	if(IS_ERR(uartdrv_device)){
		class_destroy(uartdrv_class);
		unregister_chrdev(uartdrv_major, UART_DEVICE_NAME);
		return PTR_ERR(uartdrv_device);
	}
	return 0;

}

static void __exit uartdrv_exit(void)
{
	device_destroy(uartdrv_class, MKDEV(uartdrv_major, 0));
	class_destroy(uartdrv_class);
	unregister_chrdev(uartdrv_major, UART_DEVICE_NAME);
}

module_init(uartdrv_init);
module_exit(uartdrv_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("zhangna");
//MODULE_VERSION(PKG_VERSION);
//MODULE_DESCRIPTION("UART DEVICE DRIVER");


