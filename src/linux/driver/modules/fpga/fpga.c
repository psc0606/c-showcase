#include <linux/cdev.h>
#include <linux/device.h>
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

#define DEVNAME "fpga"
#define APB_BUS_BASE_ADDR 0xfe100000UL
#define APB_MAP_SIZE 	  0xffffUL
#define FPGA_BASE_ADDR    0xa8000000UL
#define FPGA_SIZE         2*1024

/* register's offsize*/
#define EX_CS_EN_R 	  0x4
#define EX_CS1_BASE_R 	  0xc
#define EX_CS1_SEG_R 	  0x20
#define EX_CS1_CFG_R	  0x34
#define EX_CS1_TMG1_R	  0x48
#define EX_CS1_TMG2_R	  0x5c
#define EX_CS1_TMG3_R	  0x70

static struct class *class;
static int major = 0;

struct fpga_dev {
	struct cdev cdev;
	u32 *apb_vir_addr;
	u8 *fpga_vir_addr; //fpga unit:8bit
} *dev;

static int fpga_open(struct inode *inode, struct file *filp)
{
	filp->private_data = dev;

	/*Extent BUS initlitian*/
	dev->apb_vir_addr = ioremap_nocache(APB_BUS_BASE_ADDR, APB_MAP_SIZE);
	dev->fpga_vir_addr = ioremap_nocache(FPGA_BASE_ADDR, FPGA_SIZE);

//	if(!access_ok(VERIFY_WRITE, dev->vir_addr, APB_MAP_SIZE))
	if(!dev->apb_vir_addr || !dev->fpga_vir_addr)
		printk(KERN_WARNING "invaild ioremap");
	
	/* setup for CS1 */
	*(volatile u32*)(dev->apb_vir_addr + EX_CS_EN_R/4) |= (1<<2);
	*(volatile u32*)(dev->apb_vir_addr + EX_CS1_BASE_R/4) = 0x28000;
	*(volatile u32*)(dev->apb_vir_addr + EX_CS1_SEG_R/4)  = 0x28001;
	*(volatile u32*)(dev->apb_vir_addr + EX_CS1_CFG_R/4)  = (1<<11) | (1 << 10);
	*(volatile u32*)(dev->apb_vir_addr + EX_CS1_TMG1_R/4) = (20<<24) | (40<<16) | (60);
	*(volatile u32*)(dev->apb_vir_addr + EX_CS1_TMG2_R/4) = (18<<24) | (15<<16) | (8<<8) | (9);
	return 0;
}

static ssize_t fpga_read(struct file *filp, char __user *buf, size_t count, loff_t *f_ops) {
	struct fpga_dev *dev = filp->private_data;
	unsigned long p = *f_ops;

	if(copy_to_user(buf, dev->fpga_vir_addr+p, count))
		return -EFAULT;
	else {
		*f_ops += count;
	}

	return count;
}

static ssize_t fpga_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_ops)
{
	struct fpga_dev *dev = filp->private_data;
	unsigned long p = *f_ops;
	
	if(copy_from_user(dev->fpga_vir_addr+p, buf, count))
		return -EFAULT;
	else
		*f_ops += count;

	return count;
}

static loff_t fpga_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	if(offset < 0)
	{
		return -EINVAL;
	}
	switch(orig){
		case 0:
			ret = offset;
			break;
		case 1:
			ret = filp->f_pos + offset;
			break;
		default:
			return -EINVAL;

	}

	filp->f_pos = ret;
	printk("llseek:%x\n",(unsigned int)filp->f_pos );
	return ret;
}

static int fpga_release(struct inode *inode, struct file *filp)
{
	return 0;
}

struct file_operations fpga_ops = {
	.owner   = THIS_MODULE,
	.open    = fpga_open,
	.release = fpga_release,
	.read 	 = fpga_read,
	.write   = fpga_write,
	.llseek  = fpga_llseek,
};

static int __init fpga_init(void)
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
	dev = kmalloc(sizeof(struct fpga_dev), GFP_KERNEL);
	if(!dev)  {
		return ENOMEM;
	}
	memset(dev, 0, sizeof(struct fpga_dev));  
	cdev_init(&dev->cdev, &fpga_ops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &fpga_ops;

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

static void __exit fpga_exit(void)
{
	dev_t devid = MKDEV(major, 0);

	device_destroy(class, devid);
	class_destroy(class);
	cdev_del(&dev->cdev);
	unregister_chrdev_region(devid, 1);
	iounmap(dev->apb_vir_addr);
	iounmap(dev->fpga_vir_addr);
	kfree(dev);
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("psc");
module_init(fpga_init);
module_exit(fpga_exit);
