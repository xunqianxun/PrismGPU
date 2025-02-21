#include "qemu/osdep.h"
#include "qemu/units.h"
#include "hw/pci/pci.h"
#include "hw/hw.h"
#include "hw/pci/msi.h"
#include "qemu/timer.h"
#include "qom/object.h"
#include "qemu/main-loop.h" /* iothread mutex */
#include "qemu/module.h"
#include "qapi/visitor.h"

#define TYPE_PCI_PRISM_DEVICE "pci-prism"
DECLARE_INSTANCE_CHECKER(PRISMState, PCIPRISM, TYPE_PCI_PRISM_DEVICE)

//#define 

#define DMA_RUN 1
#define DMA_DIR(cmd) (((cmd) & 2) >> 1)
#define DMA_TO_DEVICE 0
#define DMA_FROM_DEVICE 1
#define DMA_DONE (1<<31)
#define DMA_ERROR (1<<30)

struct PRISMState {
		PCIDevice pdev;
		MemoryRegion mmio_bar0;
		MemoryRegion mmio_bar1;
        QemuConsole *con;
		uint32_t bar0[16];
		uint8_t bar1[8192];
		struct dma_state {
			dma_addr_t src;
			dma_addr_t dst;
			dma_addr_t cnt;
			dma_addr_t cmd;
		} *dma;

	};


static void prism_invalidate(void *opaque)
{
    PRISMState *s = opaque;
    return ;
}

static void prism_show(PRISMState *s, int fall_update){
    DisplaySurface *surface = qemu_console_surface(s->con);
    
}


static const GraphicHwOps prism_ops = {
    .invalidate  = prism_invalidate,
    .gfx_update  = prism_show,
};



static int check_range(uint64_t addr, uint64_t cnt)
{
	uint64_t end = addr + cnt;
	if(end > 4*1024)
		return -1;
	return 0;
}

static void fire_dma(PciechodevState *pciechodev)
{
	struct dma_state *dma = pciechodev->dma;
	dma->cmd &= ~(DMA_DONE | DMA_ERROR);

	if(DMA_DIR(dma->cmd) == DMA_TO_DEVICE) {
		if(check_range(dma->dst, dma->cnt) == 0) {
			pci_dma_read(&pciechodev->pdev, dma->src,
			pciechodev->bar1 + dma->dst, dma->cnt);
		} else
			dma->cmd |= (DMA_ERROR);
	} else {
		if(check_range(dma->src, dma->cnt) == 0) {
			pci_dma_write(&pciechodev->pdev, dma->dst,
			pciechodev->bar1 + dma->src, dma->cnt);
		} else
			dma->cmd |= (DMA_ERROR);
	}

	dma->cmd &= ~(DMA_RUN);
	dma->cmd |= (DMA_DONE);
}


static uint64_t prism_bar0_mmio_read(void *opaque, hwaddr addr, unsigned size){
    PRISMState *prism = opaque ;

}


static const MemoryRegionOps prism_bar0_ops = {
    .read = prism_bar0_mmio_read,
    .write = prism_bar0_mmio_write,
    .valid = {
        .min_access_size = 4,
        .max_access_size = 4,
    },
    .impl = {
        .min_access_size = 4,
        .max_access_size = 4,
    },
};

static uint64_t prism_bar1_mmio_read(void *opaque, hwaddr addr, unsigned size){
    PRISMState *prism = opaque;

    if(size == 1){
        uint8_t *ptr = (uint8_t *) &prism->bar1[addr];
        return *ptr;
    }else if(size == 2){
        uint16_t *ptr = (uint16_t *) &prism->bar1[addr];
        return *ptr;
    }else if(size == 4){
        uint32_t *ptr = (uint32_t *) &prism->bar1[addr];
        return *ptr;
    }else if(size == 8){
        uint64_t *ptr = (uint64_t *) &prism->bar1[addr];
        return *ptr;
    }
    return 0xffffffffffffffffL;
}

static void prism_bar1_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size){
    PRISMState *prism = opaque ;

    if(size == 1){
        uint8_t *ptr = (uint8_t *) &prism->bar[addr];
        *ptr = (uint8_t) val;
    }else if(size == 2){
        uint16_t *ptr = (uint16_t *) &prism->bar[addr];
        *ptr = (uint16_t) val;
    }else if(size == 4){
        uint32_t *ptr = (uint32_t *) &prism->bar[addr];
        *ptr = (uint32_t) val;
    }else if(size == 8){
        uint64_t *ptr = (uint64_t *) &prism->bar[addr];
        *ptr = (uint64_t) val;
    }
}

static const MemoryRegionOps prism_bar1_ops = {
    .read = prism_bar1_mmio_read,
    .write = prism_bar1_mmio_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
    .valid = {
        .min_access_size = 1,
        .max_access_size = 8,
    },
    .impl = {
        .min_access_size = 1,
        .max_access_size = 8,
    },
};


static void pci_prism_realize(PCIDevice *dev, Error errp){
    PRISMState *prism = PCIPRISM(dev) ;
    uint8_t *pci_config = dev->config;

    prism->con = graphic_console_init(DEVICE(dev), 0, &prism_ops, prism);
    pci_config_set_interrupt_pin(pci_config, 1);

    memset(prism->bar0, 0, 64);
    memset(prism->bar1, 0,4096);


    prism->dma = (struct dma_state *) &prism->bar0[4];

    memory_region_init_io(&prism->mmio_bar0, OBJECT(prism), &prism_bar0_ops, prism, "prismctl_mmio", 64);
    pci_register_bar(pdev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &prism->mmio_bar0);
    memory_region_init_io(&prism->mmio_bar1, OBJECT(prism), &prism_bar1_ops, prism, "prismvram_mmio", 8192);
    pci_register_bar(pdev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY)

}


static void pci_prism_uninit(PCIDevice *pdev){
    return ;
}


static void prism_instance_init(Object *obj){
    return ;
}

static void prism_class_init(ObjectClass *klass, void *data){
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = pci_prism_realize;
    k->exit = pci_prism_uninit;
    k->vendor_id = PCI_VENDOR_ID_PRISM;
    k->device_id = PCI_VENDOR_ID_PRISM_GPU;
    k->revision = 0x01 ;
    k->class_id = PCI_CLASS_DISPLAY_OTHER;

    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}




static void pci_prism_device_register_types(void)
{

	static const TypeInfo prism_pci_device_info = {
		.name          = TYPE_PCI_PRISM_DEVICE,
		.parent        = TYPE_PCI_DEVICE,
		.instance_size = sizeof(PRISMState),
		.instance_init = prism_instance_init,
		.class_init    = prism_class_init,
		.interfaces = interfaces[] = {
            { INTERFACE_CONVENTIONAL_PCI_DEVICE },
		    { },
        },
	};
	//registers the new type.
	type_register_static(&prism_pci_device_info);
}

type_init(pci_prism_device_register_types)




