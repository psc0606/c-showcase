PKG_VERSION:=0.1
default:
	$(MAKE) -C /opt/linux-3.0.51/   -I/opt/linux-3.0.51/arch/arm/mach-transcede/include/ SUBDIRS=$(PWD)
.PHONY:clean
clean:
	rm -f *.o modules.order Module.symvers *.ko *.mod.c
obj-m += tradiodrv.o
tradiodrv-objs := tradio.o radtimers.o 
