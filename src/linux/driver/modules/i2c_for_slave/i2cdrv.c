#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h> //udelay
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/types.h>

#include <asm/uaccess.h>

#define DEVNAME "i2c-0"

//#define AXI_RESET_1 (0xF4CF0000+0x54/4)  /* AXI Reset Control Register 1 */
#define CLOCK_RESET_BASE 0xF4CF0000
#define AXI_RESET_1 (0x54/4)
#define I2CSPI_AXI_RESET_1  (1<<5)

#define GPIO_BASEADDR 0xFE070000
#define GPIO_31_16_PIN_SELECT_REG (0x5C/4) //i2c use gpio 16 and 17

#define I2C_CLKDIV 0x4d

/*
 * I2C controller regsiters
 */
#define I2C_BASEADDR	0xFE09C000 //= apb_base_addr
#define I2C_ADDR_REG	0x00
#define I2C_DATA_REG	0x01
#define I2C_CNTR_REG	0x02
#define I2C_STAT_REG	0x03
#define I2C_CCRFS_REG	0x03
#define I2C_XADDR_REG	0x04
#define I2C_CCRH_REG	0x05
#define I2C_SRST_REG	0x07
#define I2C_CTRL	(0x1000/4) // VLSI to confirm

/* I2C Control register bits */
#define I2C_IEN      (1 << 7)
#define I2C_ENAB     (1 << 6)
#define I2C_STA      (1 << 5)
#define I2C_STP      (1 << 4)
#define I2C_IFLG     (1 << 3)
#define I2C_AAK      (1 << 2)

#define I2C_WRITE	0
#define I2C_READ	1
#define START_MASTER	(1<<5)
#define I2C_MAX_TIMEOUT	1000

/* STAT - Status codes */
#define I2C_BUS_ERROR		0x00	/* Bus error in master mode only */
#define I2C_START_TRANSMIT	0x08	/* Start condition transmitted */
#define I2C_REPEAT_START_TRANSMIT	0x10	/* Repeated Start condition transmited */
#define I2C_ADDRESS_W_ACK	0x18	/* Address + Write bit transmitted, ACK received */
#define I2C_ADDRESS_W_NACK	0x20	/* Address + Write bit transmitted, NACK received */
#define I2C_DATA_TRANSMIT_ACK	0x28	/* Data byte transmitted in master mode , ACK received */
#define I2C_DATA_TRANSMIT_NACK	0x30	/* Data byte transmitted in master mode , NACK received */
#define I2C_ARBIT_LOST		0x38	/* Arbitration lost in address or data byte */
#define I2C_ADDRESS_R_ACK	0x40	/* Address + Read bit transmitted, ACK received  */
#define I2C_ADDRESS_R_NACK	0x48	/* Address + Read bit transmitted, NACK received  */
#define I2C_DATA_RECEIVE_ACK	0x50	/* Data byte received in master mode, ACK transmitted  */
#define I2C_DATA_RECEIVE_NACK	0x58	/* Data byte received in master mode, NACK transmitted*/
#define I2C_ARBIT_LOST_ADDRESS	0x68	/* Arbitration lost in address  */
#define I2C_GENERAL_CALL	0x70	/* General Call, ACK transmitted */
#define I2C_NO_RELEVANT_INFO	0xF8	/* No relevant status information, IFLF=0 */

#define I2C_RC_TIMEOUT		-1UL
#define I2C_RC_NACK		-2UL
#define I2C_RC_HAVE_NOT_DEVICE	-3UL
#define I2C_RC_SET_MASTER_ERROR	-4UL

#define REG32(regaddr) (*(regaddr))

void I2CDrvResetDevice(void);

static struct class *class;
static int major = 0;

struct i2c_dev {
	struct cdev cdev;
	volatile u32 *i2c_vir_addr; //i2c unit:8bit
	volatile u32 *axi_reset_ctrl;
	volatile u32 *gpio_select_reg;
} *dev;

static u8 I2C_IS_IFLG_SET_RET()
{
	u32 timeout = 0;
	while (!(REG32(dev->i2c_vir_addr + I2C_CNTR_REG) & I2C_IFLG)) {
		timeout++;
		if (timeout >= I2C_MAX_TIMEOUT)
			return 0;

		udelay (1);
	}
	return 1;
}

static u8 I2C_IS_IFLG_SET() 
{
	u32 timeout = 0;

	while (!(REG32(dev->i2c_vir_addr + I2C_CNTR_REG) & I2C_IFLG)) {
		timeout++;
		if (timeout >= I2C_MAX_TIMEOUT)
			return 0;

		udelay (1);
	}
	return 1;
}

/* Calc Foscl = Fclk / ((2 << N) * (M + 1) * 10) */
static int i2c_calc_baudrate( unsigned int speed )
{
	unsigned int n, m, freq, delta, power;
	unsigned int actualFreq, actualN=0, actualM=0;
	unsigned int control, status;
	unsigned int deltamin = -1;

	for(n = 0 ; n < 8 ; n++) {
		for(m = 0 ; m < 16 ; m++) {
			power = 2 << n;
			freq = TRANSCEDE_AHBCLK_HZ / (10 * (m + 1) * power);
		//	freq = 300000000 / (10 * (m + 1) * power);
			delta = abs(speed - freq);
			if(delta < deltamin)
			{
				deltamin    = delta;
				actualFreq  = freq;
				actualN	    = n;
				actualM	    = m;
			}
		}
	}

	return (actualM << 4) | actualN;
}

static void sInitHW(void)
{
    REG32(dev->axi_reset_ctrl + AXI_RESET_1) &=  ~(I2CSPI_AXI_RESET_1); // Release I2C and SPI from reset
    REG32(dev->gpio_select_reg + GPIO_31_16_PIN_SELECT_REG) &= ~0x0F;

    REG32(dev->i2c_vir_addr + I2C_DATA_REG) = 0; 
    REG32(dev->i2c_vir_addr + I2C_ADDR_REG) = 0;
    REG32(dev->i2c_vir_addr + I2C_XADDR_REG) = 0;
    REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0; // Clear control register
//    REG32(dev->i2c_vir_addr + I2C_CCRFS_REG) = I2C_CLKDIV;//Set up clock divider for standard mode
//    REG32(dev->i2c_vir_addr + I2C_CCRFS_REG) = i2c_calc_baudrate(80000);
REG32(dev->i2c_vir_addr + I2C_CCRFS_REG) = i2c_calc_baudrate(100000);
    I2CDrvResetDevice(); //reset i2c

    REG32(dev->i2c_vir_addr + I2C_DATA_REG) = 0;
    REG32(dev->i2c_vir_addr + I2C_ADDR_REG) = 0;
    REG32(dev->i2c_vir_addr + I2C_XADDR_REG) = 0;
    REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
//    REG32(dev->i2c_vir_addr + I2C_CCRFS_REG) = I2C_CLKDIV;  
REG32(dev->i2c_vir_addr + I2C_CCRFS_REG) = i2c_calc_baudrate(100000);
}

int i2c_read(u8 chip, u32 addr, int alen, u8 *buffer, int len)
{
	u8  result;
	u16 i = 0;

	printk("i2c_read: chip=0x%02X, offs=0x%04X, len=%d\n", chip, addr, len);

	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_STA;
	if ( !I2C_IS_IFLG_SET_RET() ) {
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = START_MASTER;
		if (!I2C_IS_IFLG_SET()){
			printk("timeout1\n");
			return I2C_RC_TIMEOUT;
		}
	}

	result = REG32(dev->i2c_vir_addr + I2C_STAT_REG);

	if (result == I2C_START_TRANSMIT ||  result== I2C_REPEAT_START_TRANSMIT)
	{
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = ((chip << 1) & 0xFE)|I2C_WRITE;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
	}

	if (!I2C_IS_IFLG_SET()){
		printk("timeout2\n");
		return I2C_RC_TIMEOUT;
	}

	result = REG32(dev->i2c_vir_addr + I2C_STAT_REG);
	if (result != I2C_ADDRESS_W_ACK ){
		printk("timeout3\n");
		return I2C_RC_NACK;
	}

	if (alen > 1) {
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = (addr >> 8) & 0xFF;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
		if (!I2C_IS_IFLG_SET()){
			printk("timeout4\n");
			return I2C_RC_TIMEOUT;
		}
	}

	if (alen > 0) {
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = addr & 0xFF;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
		if (!I2C_IS_IFLG_SET())
			return I2C_RC_TIMEOUT;
	}

	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = START_MASTER;
	if ( !I2C_IS_IFLG_SET_RET() ) {
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = START_MASTER;
		if (!I2C_IS_IFLG_SET())
			return I2C_RC_TIMEOUT;
	}


	REG32(dev->i2c_vir_addr + I2C_DATA_REG) = ((chip << 1) & 0xFE) | I2C_READ;
	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
	if (!I2C_IS_IFLG_SET())
		return I2C_RC_TIMEOUT;

	result = REG32(dev->i2c_vir_addr + I2C_STAT_REG);
	if (result != I2C_ADDRESS_R_ACK )
		return I2C_RC_NACK;

	if (len > 1)
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_AAK;
	else if (len == 1)
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
	else  {
		if (!I2C_IS_IFLG_SET())
			return I2C_RC_TIMEOUT;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
	}

	for (i = 0; i < len; i++) {
		if (!I2C_IS_IFLG_SET())
			return I2C_RC_TIMEOUT;

		*(u8 *)(buffer + i) = (u8) (REG32(dev->i2c_vir_addr + I2C_DATA_REG));
		if (i == len-1)	{
			REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_STP;
			REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
		} else 	{
			if (i < len-2)
				REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_AAK;
			else
				REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0;
		}

	}
	return 0;
}

int i2c_write(u8 chip, u32 addr, int alen, u8 *buffer, int len)
{
	volatile u32 status;
	u32 i = 0; 

	printk("i2c_write: chip=0x%02X, offs=0x%04X, len=%d\n", chip, addr, len);

	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_STA;
	if ( !I2C_IS_IFLG_SET_RET() ) {
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = START_MASTER;
		if (!I2C_IS_IFLG_SET()){
			printk("timeout1\n");
			return I2C_RC_TIMEOUT;
		}
	}

	status = REG32(dev->i2c_vir_addr + I2C_STAT_REG);
	if (status == I2C_START_TRANSMIT ||
		status == I2C_REPEAT_START_TRANSMIT)
	{
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = ((chip << 1) & 0xFE) | I2C_WRITE;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) &= ~I2C_IFLG;
	}

	if (!I2C_IS_IFLG_SET()){
		printk("timeout2\n");
		return I2C_RC_TIMEOUT;
	}

	status = REG32(dev->i2c_vir_addr + I2C_STAT_REG);
	if (status != I2C_ADDRESS_W_ACK)
		return I2C_RC_SET_MASTER_ERROR;


	if (alen > 1) {
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = (addr >> 8) & 0xFF;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) &= ~I2C_IFLG;
		if (!I2C_IS_IFLG_SET()){
			printk("timeout3\n");
			return I2C_RC_TIMEOUT;
		}
		status = REG32(dev->i2c_vir_addr + I2C_STAT_REG);
	}

	if (alen > 0) {
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = addr & 0xFF;
		REG32(dev->i2c_vir_addr + I2C_CNTR_REG) &= ~I2C_IFLG;
		if (!I2C_IS_IFLG_SET()){
			printk("timeout4\n");
			return I2C_RC_TIMEOUT;
		}
		status = REG32(dev->i2c_vir_addr + I2C_STAT_REG);
	}

	for (i=0; i < len; i++)
	{
		REG32(dev->i2c_vir_addr + I2C_DATA_REG) = (u8) buffer [i];
	     	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) &= ~I2C_IFLG;
		if (!I2C_IS_IFLG_SET())
			return I2C_RC_TIMEOUT;
	}

	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_STP;

	return 0;
}

void I2CDrvResetDevice(void)
{
	u32 i;

	REG32(dev->i2c_vir_addr + I2C_CTRL) |= 1;

	for (i = 0; i < 9; i++) {
		//put dev in master mode and tx a start condition
		REG32(dev->i2c_vir_addr+ I2C_CNTR_REG) = I2C_STA;
		I2C_IS_IFLG_SET(); //wait, but if no transition ignore
	}
	//Send  9 data bits :  111111111, 8+1 from the ack, i2c ip will clock 9 pulses for us. note bus is held hi
	REG32(dev->i2c_vir_addr + I2C_DATA_REG) = 0xFF;
	// To send it, we must clear the iflg.
	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0x0;
	I2C_IS_IFLG_SET(); 

	// Send one START conditional
	REG32(dev->i2c_vir_addr+ I2C_CNTR_REG) = I2C_STA;
	I2C_IS_IFLG_SET(); //wait, but if no transition ignore

	// Send one STOP conditional
	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = I2C_STP;;
	REG32(dev->i2c_vir_addr + I2C_CNTR_REG) = 0x0; //done, clear the flag

	//undo the mux
	REG32(dev->i2c_vir_addr + I2C_CTRL) &= ~1;
}

static int i2c_open(struct inode *inode, struct file *filp)
{
	filp->private_data = dev;

	dev->i2c_vir_addr = (u8*)ioremap_nocache(I2C_BASEADDR, 0x2000);
	dev->axi_reset_ctrl = (u8*)ioremap_nocache(CLOCK_RESET_BASE, 0x1000);
	dev->gpio_select_reg = (u8*)ioremap_nocache(GPIO_BASEADDR, 0x1000);

//	if(!access_ok(VERIFY_WRITE, dev->vir_addr, APB_MAP_SIZE))
	if(!dev->i2c_vir_addr)
		printk(KERN_WARNING "invaild ioremap");

//	printk("addr:0x%x\n", dev->i2c_vir_addr);
	sInitHW();
//	printk(KERN_EMERG "reset:0x%x\n", *(dev->axi_reset_ctrl));
	
	return 0;
}

static ssize_t i2c_read_last(struct file *filp, char __user *buf, size_t count, loff_t *f_ops) {
	struct i2c_dev *dev = filp->private_data;
	unsigned long p = *f_ops;

	int err = 0;
	char kbuf[3];

	if(copy_from_user(kbuf, buf, count))
		return -EFAULT;

	err = i2c_read(kbuf[0], kbuf[1], 1, kbuf+2, 1);
	if(err){
		printk("i2c_read error\n");
		return -1;
	}

	if(copy_to_user(buf, kbuf, count))
		return -EFAULT;

//	printk("buf:0x%x\n", kbuf[2]);
	return count;
}

static ssize_t i2c_write_last(struct file *filp, const char __user *buf, size_t count, loff_t *f_ops)
{
	struct i2c_dev *dev = filp->private_data;
	unsigned long p = *f_ops;

	int err = 0;
	char kbuf[3];

	if(copy_from_user(kbuf, buf, count))
		return -EFAULT;

	err = i2c_write(kbuf[0], kbuf[1], 1, kbuf+2, 1);
	if(err){
		printk("i2c_write error\n");
		return -1;
	}

//	printk("buf[2]:0x%x\n", buf[2]);
	return count;
}

static int i2c_release(struct inode *inode, struct file *filp)
{
	return 0;
}

struct file_operations i2c_ops = {
	.owner   = THIS_MODULE,
	.open    = i2c_open,
	.release = i2c_release,
	.read 	 = i2c_read_last,
	.write   = i2c_write_last,
};

static int __init i2c_init(void)
{
	int rc;
	dev_t devid;
       	
	/* allocate device number*/
	if(major) {
		devid = MKDEV(major, 0);
		rc = register_chrdev_region(devid, 1, DEVNAME);
	} else {
		rc = alloc_chrdev_region(&devid, 0, 1, DEVNAME);
		major = MAJOR(devid);
	}

	if(rc < 0) {
		printk(KERN_WARNING "%s chrdev region error:%d", DEVNAME, rc);
		return rc;
	}	


	/* char device register*/
//	dev->cdev = cdev_alloc();
	dev = kmalloc(sizeof(struct i2c_dev), GFP_KERNEL);
	if(!dev)  {
		return ENOMEM;
	}
	memset(dev, 0, sizeof(struct i2c_dev));  
	cdev_init(&dev->cdev, &i2c_ops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &i2c_ops;

	rc = cdev_add(&dev->cdev, devid, 1);
	if(rc)
		printk(KERN_WARNING "Error %d adding %s", rc, DEVNAME);


	/* dynamic create device node */
	class = class_create(THIS_MODULE, DEVNAME);
	if(IS_ERR(class)) {
		goto unregister_chrdev_region;
	}

	device_create(class, NULL, devid, NULL, DEVNAME);
	
	return 0;

unregister_chrdev_region:
	unregister_chrdev_region(devid, 1);

	return rc;
}

static void __exit i2c_exit(void)
{
	dev_t devid = MKDEV(major, 0);

	device_destroy(class, devid);
	class_destroy(class);
	cdev_del(&dev->cdev);
	unregister_chrdev_region(devid, 1);
	iounmap(dev->i2c_vir_addr);
	iounmap(dev->axi_reset_ctrl);
	iounmap(dev->gpio_select_reg);
	kfree(dev);
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("psc");
module_init(i2c_init);
module_exit(i2c_exit);
