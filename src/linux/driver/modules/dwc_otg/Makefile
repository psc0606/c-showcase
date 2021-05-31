#
# Makefile for DWC_otg Highspeed USB controller driver
#

ifneq ($(KERNELRELEASE),)

# Use one of the following flags to compile the software in host-only or
# device-only mode.
CPPFLAGS        += -DDWC_HOST_ONLY
CGG: CPPFLAGS	+= -DDWC_EN_ISOC
CPPFLAGS   		+= -DDWC_LINUX
#CPPFLAGS   		+= -DDEBUG

#CPPFLAGS        += -DDWC_DEVICE_ONLY
#CPPFLAGS	+= -Dlinux -DDWC_HS_ELECT_TST

#obj-$(CONFIG_USB_DWCOTG) += dwc_otg.o
obj-m += dwc_otg.o

dwc_otg-objs	:= dwc_otg_driver.o dwc_otg_attr.o 
dwc_otg-objs	+= dwc_otg_cil.o dwc_otg_cil_intr.o 
dwc_otg-objs	+= dwc_otg_hcd_linux.o dwc_otg_adp.o dwc_otg_cfi.o 
dwc_otg-objs	+= dwc_otg_hcd_ddma.o
dwc_otg-objs	+= dwc_otg_hcd.o dwc_otg_hcd_intr.o dwc_otg_hcd_queue.o
dwc_otg-objs	+= dwc_common_linux.o dwc_mem.o

kernrelwd := $(subst ., ,$(KERNELRELEASE))
kernrel3 := $(word 1,$(kernrelwd)).$(word 2,$(kernrelwd)).$(word 3,$(kernrelwd))

ifneq ($(kernrel3),2.6.20)
EXTRA_CFLAGS += $(CPPFLAGS)
endif

else
KDIR	:= /home/psc/linux-3.0.51_USB/
PWD		:= $(shell pwd)

default:
	$(MAKE) -C$(KDIR) M=$(PWD) modules
#	$(MAKE) -C$(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	rm -rf   *.o *.ko .*cmd *.mod.c .tmp_versions

endif
