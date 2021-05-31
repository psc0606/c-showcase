PKG_VERSION:=0.1
default:
	$(MAKE) -C /home/dingc/kernels/linux-3.0.51/   -I/home/dingc/kernels/linux-3.0.51/arch/arm/mach-transcede/include/ SUBDIRS=$(PWD)
clean:
	$(shell rm *.o modules.order Module.symvers *.ko *.mod.c)
obj-m += tradiodrv.o
tradiodrv-objs := tradio.o radtimers.o 
