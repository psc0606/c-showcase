#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <mach/usb_mmap.h>
#include <mach/irqs-2200.h>
#include <mach/irqs.h>

static u64 transcede_dwc_otg_dmamask = DMA_BIT_MASK(32);

static struct resource transcede_dwc_otg_resources[] = {
	[0] = {
		.name   = "usb2.0 otg-core",
		.start	= USB0_CFG_BASE,
		.end	= USB0_CFG_BASE + 0xFFFFFF,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name   = "usb2.0 otg-irq",
		.start  = IRQ_USB,
		.end    = IRQ_USB,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device transcede_dwc_otg = {
	.name = "dwc_otg",
	.id = -1,
	.dev = {
		.platform_data     = NULL,
		.dma_mask  = &transcede_dwc_otg_dmamask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.resource	= transcede_dwc_otg_resources,
	.num_resources = ARRAY_SIZE(transcede_dwc_otg_resources),
};

static int __init usb_ehci_init(void)
{
	return platform_device_register(&transcede_dwc_otg);
}

static void __exit usb_ehci_exit(void)
{

}

module_init(usb_ehci_init);
module_exit(usb_ehci_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_ALIAS("platform:transcede-ehci");
