#
# Kernel module build rules
#
CROSS_COMPILE = arm-none-linux-gnueabi-
EXTRA_CFLAGS = -I/home/dingc/kernels/linux-3.0.51/arch/arm/mach-transcede/include -I/home/dingc/Lib_Headfiles/usr/include -I/home/dingc/Lib_Headfiles/include -L/home/dingc/Lib_Headfiles/usr/lib -L/home/dingc/Lib_Headfiles/lib -DTRADIO -D PKG_VERSION="2.7.0-rc2-0.1"
CFLAGS = -Os -pipe -mcpu=cortex-a9
LDFLAGS = 
init_tradio: libtradio.so
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L. -ltradio -lm -o $@ \
				init_radio.c

libtradio.so:
	$(CROSS_COMPILE)gcc -shared $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ \
    				ad9361radio.c \
				spi.c 		
clean:
	$(shell rm init_tradio libtradio.so)
