#
# Kernel module build rules
#
CROSS_COMPILE = arm-none-linux-gnueabi-
EXTRA_CFLAGS = -I/home/dingc/kernels/linux-3.0.51/arch/arm/mach-transcede/include -I/home/dingc/Lib_Headfiles/usr/include -I/home/dingc/Lib_Headfiles/include -L/home/dingc/Lib_Headfiles/usr/lib -L/home/dingc/Lib_Headfiles/lib -DTRADIO -D PKG_VERSION="2.7.0-rc2-0.1"
CFLAGS = -Os -pipe -mcpu=cortex-a9
LDFLAGS = 
.PHONY=all
all:PRINT_TRADIO #RF_L1_Debugger
#RF_L1_Debugger
#PRINT_TRADIO
RF_L1_Debugger:
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L../src_init_app/ -ltradio -lm -o $@ \
				rf_l1_debuger.c
PRINT_TRADIO:
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L../src_init_app/ -ltradio -lm -o $@ \
				print_tradio.c

clean:
	$(shell rm PRINT_TRADIO)
