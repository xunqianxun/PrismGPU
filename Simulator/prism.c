#include "qemu/osdep.h"
#include "qemu/units.h"
#include "hw/pci/pci.h"
#include "hw/hw.h"
#include "ui/console.h"
#include "migration/vmstate.h"
#include "hw/xen/xen.h"
//#include "prism.h"
#include "hw/pci/msi.h"
#include "qemu/timer.h"
#include "qom/object.h"
#include "qemu/main-loop.h" /* iothread mutex */
#include "qemu/module.h"
#include "qapi/visitor.h"

#define TYPE_PCI_PRISM_DEVICE "pci-prism"


//#define 
#define REG888 7
#define PRISM_VRAM_SIZE (8 * MiB)

#define DMA_RUN 1
#define DMA_DIR(cmd) (((cmd) & 2) >> 1)
#define DMA_TO_DEVICE 0
#define DMA_FROM_DEVICE 1
#define DMA_DONE (1<<31)
#define DMA_ERROR (1<<30)


//bar[0] gpu ctrl register
#define REG_PPD_OFFSET 0x07

#define REG_HORIZON_OFFSET 0x7ff8

#define REG_VERTICAL_OFFSET 0x7ff8000

//bar[2]
#define REG_CLOCK_OFFSET 0xff 

typedef struct PRISMState PRISMState;

DECLARE_INSTANCE_CHECKER(PRISMState, PCIPRISM, TYPE_PCI_PRISM_DEVICE)

struct PRISMState {
		PCIDevice pdev;
		MemoryRegion mmio_bar0;
		MemoryRegion mmio_bar1;
        QemuConsole *con;
		uint32_t bar0[16];
		uint8_t *bar1_ptr;
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

static void prism_show(void *opaque){

    PRISMState *s = opaque;

    int vertical, horizon, ppd;
    uint32_t d=0;
    //uint8_t clock;
    uint8_t r,g,b;
    uint8_t *pbuffer_ptr ;
    uint8_t *dest ;

    ppd = s->bar0[0] & REG_PPD_OFFSET;
    horizon = s->bar0[0] & REG_HORIZON_OFFSET;
    vertical = s->bar0[0] & REG_VERTICAL_OFFSET;
    //clock = s->bar0[1] & REG_CLOCK_OFFSET;
    pbuffer_ptr = s->bar1_ptr;

    DisplaySurface *surface = qemu_console_surface(s->con);
    dest = surface_data(surface);

    if(ppd == REG888) {
        for(int i=0; i<vertical; i++){ 
            for(int j=0; j<horizon; j++){
                r = *(pbuffer_ptr + 0) ;
                g = *(pbuffer_ptr + 1) ;
                b = *(pbuffer_ptr + 2) ;
                pbuffer_ptr += 3 ;

                d = d | (r << 16) | (g << 8) | b ;
                memcpy(dest, &d,4);
            }
        }   
    }   
    
    dpy_gfx_update(s->con, 0, 0, 1280, 720);

}


static const GraphicHwOps prism_ops = {
    .invalidate  = prism_invalidate,
    .gfx_update  = prism_show,
};



// static int check_range(uint64_t addr, uint64_t cnt)
// {
// 	uint64_t end = addr + cnt;
// 	if(end > 4*1024)
// 		return -1;
// 	return 0;
// }

// static void fire_dma(PciechodevState *pciechodev)
// {
// 	struct dma_state *dma = pciechodev->dma;
// 	dma->cmd &= ~(DMA_DONE | DMA_ERROR);

// 	if(DMA_DIR(dma->cmd) == DMA_TO_DEVICE) {
// 		if(check_range(dma->dst, dma->cnt) == 0) {
// 			pci_dma_read(&pciechodev->pdev, dma->src,
// 			*(pciechodev->bar1_ptr + dma->dst), dma->cnt);
// 		} else
// 			dma->cmd |= (DMA_ERROR);
// 	} else {
// 		if(check_range(dma->src, dma->cnt) == 0) {
// 			pci_dma_write(&pciechodev->pdev, dma->dst,
// 			*(pciechodev->bar1_ptr + dma->src), dma->cnt);
// 		} else
// 			dma->cmd |= (DMA_ERROR);
// 	}

// 	dma->cmd &= ~(DMA_RUN);
// 	dma->cmd |= (DMA_DONE);
// }


static uint64_t prism_bar0_mmio_read(void *opaque, hwaddr addr, unsigned size){
    PRISMState *prism = opaque ;

    return prism->bar0[addr/4];

}


static void prism_bar0_mmio_write(void *opaque, hwaddr addr, uint64_t val,
		unsigned size){
    PRISMState *prism = opaque;

    switch (addr)
    {
    case 0:
        prism->bar0[0] = val;
        break;
    case 1:
        prism->bar0[1] = val;
        break;
    
    default:
        break;
    }
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
        uint8_t *ptr = (uint8_t *) (prism->bar1_ptr + addr);
        return *ptr;
    }else if(size == 2){
        uint16_t *ptr = (uint16_t *) (prism->bar1_ptr + addr);
        return *ptr;
    }else if(size == 4){
        uint32_t *ptr = (uint32_t *) (prism->bar1_ptr + addr);
        return *ptr;
    }else if(size == 8){
        uint64_t *ptr = (uint64_t *) (prism->bar1_ptr + addr);
        return *ptr;
    }
    return 0xffffffffffffffffL;
}

static void prism_bar1_mmio_write(void *opaque, hwaddr addr, uint64_t val, unsigned size){
    PRISMState *prism = opaque ;

    if(size == 1){
        uint8_t *ptr = (uint8_t *) (prism->bar1_ptr + addr);
        *ptr = (uint8_t) val;
    }else if(size == 2){
        uint16_t *ptr = (uint16_t *) (prism->bar1_ptr + addr);
        *ptr = (uint16_t) val;
    }else if(size == 4){
        uint32_t *ptr = (uint32_t *) (prism->bar1_ptr + addr);
        *ptr = (uint32_t) val;
    }else if(size == 8){
        uint64_t *ptr = (uint64_t *) (prism->bar1_ptr + addr);
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


static void pci_prism_realize(PCIDevice *dev, Error **errp){
    PRISMState *prism = PCIPRISM(dev) ;
    uint8_t *pci_config = dev->config;
    Error *prism_err = NULL;
    
    memory_region_init_ram_nomigrate(&prism->mmio_bar1, OBJECT(prism), "prism_vram", PRISM_VRAM_SIZE, &prism_err);

    if(prism_err){
        // error_propagate(errp, prism_err);
        // return false;
    }

    vmstate_register_ram(&prism->mmio_bar1, DEVICE(prism));
    //xen_register_framebuffer(&prism->mmio_bar1);

    prism->bar1_ptr = memory_region_get_ram_ptr(&prism->mmio_bar1);

    prism->con = graphic_console_init(DEVICE(dev), 0, &prism_ops, prism);

    pci_config_set_interrupt_pin(pci_config, 1);

    memset(&prism->bar0, 0, 64);
    memset(prism->bar1_ptr, 0, PRISM_VRAM_SIZE);


    prism->dma = (struct dma_state *) &prism->bar0[4];

    memory_region_init_io(&prism->mmio_bar0, OBJECT(prism), &prism_bar0_ops, prism, "prismctl_mmio", 64);
    pci_register_bar(dev, 0, PCI_BASE_ADDRESS_SPACE_MEMORY, &prism->mmio_bar0);
    memory_region_init_io(&prism->mmio_bar1, OBJECT(prism), &prism_bar1_ops, prism, "prismvram_mmio", PRISM_VRAM_SIZE);
    pci_register_bar(dev, 1, PCI_BASE_ADDRESS_SPACE_MEMORY, &prism->mmio_bar1);

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
    k->vendor_id = 0x1754;
    k->device_id = 0xd325;
    k->revision = 0x01 ;
    k->class_id = PCI_CLASS_DISPLAY_OTHER;

    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}




static void pci_prism_device_register_types(void)
{
    static InterfaceInfo interfaces[] = {
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    };

	static const TypeInfo prism_pci_device_info = {
		.name          = TYPE_PCI_PRISM_DEVICE,
		.parent        = TYPE_PCI_DEVICE,
		.instance_size = sizeof(PRISMState),
		.instance_init = prism_instance_init,
		.class_init    = prism_class_init,
		.interfaces = interfaces,
	};
	//registers the new type.
	type_register_static(&prism_pci_device_info);
}

type_init(pci_prism_device_register_types)




