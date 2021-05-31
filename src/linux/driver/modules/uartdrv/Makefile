# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

# Add debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  DEBFLAGS = -O -g # "-O" is needed to expand inlines
else
  DEBFLAGS = -O2
endif

LDFLAGS	:= -r

TOPDIR_OUT		:=$(TOPDIR)/..
TARGET_CROSS = arm-none-linux-gnueabi-
PKG_VERSION		:=1.0.1
DRIVERMODULE	:=uartdrv.ko
INSTDIR				:=bin
DYNADIR				:=$(TOPDIR_OUT)/$(INSTDIR)#End


#CFLAGS += $(DEBFLAGS) -I$(LDDINCDIR)

ifneq ($(KERNELRELEASE),)
# call from kernel build system

obj-m	:= uartdrv.o

else

#KERNELDIR ?= /home/work/zctt/new/bsp-2.7.0-rc2-src/build_dir/linux-transcede-2200_t2200/linux-3.0.51
PWD       := $(shell pwd)
KERNELDIR ?= /home/psc/linux-3.0.51_USB/

default:
	$(MAKE) -C "$(KERNELDIR)" \
	    CROSS_COMPILE="$(TARGET_CROSS)" \
	    SUBDIRS="$(PWD)" \
	    EXTRA_CFLAGS="$(BUILDFLAGS) -Werror -Wall -DNDEBUG -DPKG_VERSION=\"\\\"$(PKG_VERSION)\\\"\"" \
	    modules;
#	mv -f $(DRIVERMODULE) $(DYNADIR)/$(DRIVERMODULE)
#	$(MAKE) -C $(KERNELDIR) M=$(PWD) LDDINCDIR=$(PWD)/../include modules

endif



clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions *.order *.symvers;
	rm -rf $(DYNADIR)/$(DRIVERMODULE)

install:
	if [ ! -d $(DYNADIR) ]; then \
		mkdir -p $(DYNADIR); \
	fi
	mv -f $(DRIVERMODULE) $(DYNADIR)/$(DRIVERMODULE)
	#ln -s $(DRIVERMODULE).$(PKG_VERSION) $(DYNADIR)/$(DRIVERMODULE)

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
