#
# Kernel module build rules
#
CROSS_COMPILE = arm-none-linux-gnueabi-
#EXTRA_CFLAGS = -I/opt/linux-3.0.51/arch/arm/mach-transcede/include -I/home/dingc/Lib_Headfiles/usr/include -I/home/dingc/Lib_Headfiles/include -L/home/dingc/Lib_Headfiles/usr/lib -L/home/dingc/Lib_Headfiles/lib -DTRADIO -D PKG_VERSION="2.7.0-rc2-0.1"
EXTRA_CFLAGS = -I/opt/linux-3.0.51/arch/arm/mach-transcede/include -DTRADIO -D PKG_VERSION="2.7.0-rc2-0.1"
CFLAGS = -Os -pipe -mcpu=cortex-a9
LDFLAGS = 
print_tradio_file=print_tradio.c\
				  sin1mhziqdata.c\
				  ltedlqam16iqdata.c\
				  ltedlqam64iqdata.c\
				  ltedl20mhziqdata.c\
				  trianglewave.c\
				  lteqpsk10mhz.c

.PHONY:all
all:PRINT_TRADIO_SLAVE_SYMBOL

RF_L1_Debugger:rf_l1_debuger.c
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L../src_init_app/ -ltradio -lm -o $@ $^

PRINT_TRADIO_SLAVE_SYMBOL:$(print_tradio_file)
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L../src_init_app/ -ltradio -lm -o $@ $^ 

.PHONY:clean
clean:
	$(shell rm -f PRINT_TRADIO_SLAVE_SYMBOL)
