RM ?= rm -f

ifneq ($(KERNELRELEASE),) #KERNELRELEASE为内核源码顶层
obj-m := fpga.o 

else
#KDIR ?= /opt/linux-3.0.51-2.8.0-rc2
KDIR ?= /home/psc/work/linux-3.0.51-2.8.0-rc2

#/lib/modules/3.8.0-19-generic/build 
#KDIR:=/lib/modules/$(shell uname -r)/build

PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules 
# maybe no use
#ARCH:=arm CROSS_COMPILE:=arm-none-linux-gnueabi-

clean:
	$(RM) *.ko *.o *.mod.o *.mod.c *.symvers *.order
endif
