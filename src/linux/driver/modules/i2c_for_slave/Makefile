RM ?= rm -f
CROSS_COMPILE_PREFIX ?= arm-none-linux-gnueabi-

ifneq ($(KERNELRELEASE),) #KERNELRELEASE为内核源码顶层
obj-m := i2cdrv.o 

else
KDIR ?= /opt/linux-3.0.51
#-2.8.0-rc2
#/lib/modules/3.8.0-19-generic/build 
#KDIR:=/lib/modules/$(shell uname -r)/build

PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

.PHONY:i2ctest
i2ctest:i2ctest.c
	$(CROSS_COMPILE_PREFIX)gcc $^ -o $@ 

# maybe no use
#ARCH:=arm CROSS_COMPILE:=arm-none-linux-gnueabi-
.PHONY:clean
clean:
	$(RM) *.ko *.o *.mod.o *.mod.c *.symvers *.order
endif
