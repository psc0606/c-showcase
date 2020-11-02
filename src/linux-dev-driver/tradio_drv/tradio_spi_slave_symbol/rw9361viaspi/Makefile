#
# Kernel module build rules
#
CROSS_COMPILE = arm-none-linux-gnueabi-
EXTRA_CFLAGS = -I/home/dingc/kernels/linux-3.0.51/arch/arm/mach-transcede/include -I/home/dingc/Lib_Headfiles/usr/include -I/home/dingc/Lib_Headfiles/include -L/home/dingc/Lib_Headfiles/usr/lib -L/home/dingc/Lib_Headfiles/lib -DTRADIO -D PKG_VERSION="2.7.0-rc2-0.1"
CFLAGS = -Os -pipe -mcpu=cortex-a9
LDFLAGS = 
.PHONY:all
all:RW9361_0 RW9361
RW9361_0: 
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L../src_init_app -ltradio -lm -o $@ \
				rw9361_0.c
RW9361: 
	$(CROSS_COMPILE)gcc $(EXTRA_CFLAGS) $(CFLAGS) $(LDFLAGS) -L../src_init_app -ltradio -lm -o $@ \
				rw9361_1.c
clean:
	$(shell rm RW9361_0 RW9361)
