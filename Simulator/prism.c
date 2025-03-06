#include "qemu/osdep.h"
#include "qemu/module.h"
#include "qemu/units.h"
#include "hw/pci/pci_device.h"
#include "hw/qdev-properties.h"
#include "migration/vmstate.h"
#include "hw/display/bochs-vbe.h"
#include "hw/display/edid.h"
#include "qapi/error.h"

#include "ui/console.h"
#include "ui/qemu-pixman.h"
#include "qom/object.h"

#define PRISM_DISPLY_INDEC_NB 0x04
#define PRISM_DIAPLY_ENABLE   0x01 
#define PRISM_DISPI_INDEX_ENABLE  0x01
#define PRISM_DISPI_CLOCK_OFFSET 0xffff << 4
#define PRISM_DISPI_FORMATE_OFFSET 0xf << 1
#define PRISM_DISPI_WIDTH_OFFSET  0xfff << 12
#define PRISM_DISPI_HEIGHT_OFFSET 0xfff 
#define PRISM_DISPI_REG1          1
#define PRISM_DISPI_REG2          2
#define PRISM_DISPI_REG3          3
#define PRISM_DISPI_REG4          4
#define PRISM_REGISTER_SIZE       4*4
#define PRISM_REGISTER_OFFSET     0x700


 typedef struct PrismDisplyMode
 {
    pixman_format_code_t format   ;
    uint32_t             bytepp   ;
    uint32_t             width    ;
    uint32_t             height   ;
    uint32_t             stride   ;
    uint64_t             offset   ;
    uint64_t             size     ;
 }PrismDisplyMode;


struct PrismDispltState
{
    PCIDevice            pci           ;

    QemuConsole         *con           ;
    MemoryRegion         vram          ;
    MemoryRegion         vbe           ;
    MemoryRegion         mmio          ;
    MemoryRegion         qext          ;
    MemoryRegion         edid          ;
    MemoryRegion         prismreg      ;

    uint64_t             vgamem        ; //vram size
    bool                 enable_edid   ;
    qemu_edid_info       edid_info     ;
    uint8_t              edid_blob[256];

    uint16_t             vbe_regs[VBE_DISPI_INDEX_NB]    ;
    uint32_t             prism_reg[PRISM_DISPLY_INDEC_NB];
    bool                 big_endian_fb;

    PrismDisplyMode      mode         ;
};

#define TYPE_PRISM_DISPLT   "prism"
OBJECT_DECLARE_SIMPLE_TYPE(PrismDispltState, PRISM_DISPLT)

static const VMStateDescription vmstate_prism_display = {
    .name = "prism",
    .fields = (const VMStateField[]) {
        VMSTATE_PCI_DEVICE(pci, PrismDispltState),
        VMSTATE_UINT16_ARRAY(vbe_regs, PrismDispltState, VBE_DISPI_INDEX_NB),
        VMSTATE_UINT32_ARRAY(prism_reg, PrismDispltState, PRISM_DISPLY_INDEC_NB),
        VMSTATE_BOOL(big_endian_fb, PrismDispltState),
        VMSTATE_END_OF_LIST()
    }
};

static uint64_t prism_vbe_read(void *ptr, hwaddr addr,
                                       unsigned size)
{
    PrismDispltState *s = ptr;
    unsigned int index = addr >> 1;

    switch (index) {
    case VBE_DISPI_INDEX_ID:
        return VBE_DISPI_ID5;
    case VBE_DISPI_INDEX_VIDEO_MEMORY_64K:
        return s->vgamem / (64 * KiB);
    }

    if (index >= ARRAY_SIZE(s->vbe_regs)) {
        return -1;
    }
    return s->vbe_regs[index];
}

static void prism_vbe_write(void *ptr, hwaddr addr,
                                    uint64_t val, unsigned size)
{
    PrismDispltState *s = ptr;
    unsigned int index = addr >> 1;

    if (index >= ARRAY_SIZE(s->vbe_regs)) {
        return;
    }
    s->vbe_regs[index] = val;
}


static const MemoryRegionOps prism_vbe_ops = {
    .read = prism_vbe_read,
    .write = prism_vbe_write,
    .valid.min_access_size = 1,
    .valid.max_access_size = 4,
    .impl.min_access_size = 2,
    .impl.max_access_size = 2,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

static uint64_t prism_preg_read(void *ptr, hwaddr addr,
                                       unsigned size)
{
    PrismDispltState *s = ptr;
    unsigned int index = addr >> 2;

    return s->prism_reg[index];
}

static void prism_preg_write(void *ptr, hwaddr addr,
                                    uint64_t val, unsigned size)
{
    PrismDispltState *s = ptr;
    unsigned int index = addr >> 2;

    s->vbe_regs[index] = val;
}

static const MemoryRegionOps prism_preg_ops = {
    .read = prism_preg_read,
    .write = prism_preg_write,
    .valid.min_access_size = 1,
    .valid.max_access_size = 4,
    .impl.min_access_size = 2,
    .impl.max_access_size = 2,
    .endianness = DEVICE_LITTLE_ENDIAN,
};


static uint64_t prism_qext_read(void *ptr, hwaddr addr,
                                        unsigned size)
{
    PrismDispltState *s = ptr;

    switch (addr) {
    case PCI_VGA_QEXT_REG_SIZE:
        return PCI_VGA_QEXT_SIZE;
    case PCI_VGA_QEXT_REG_BYTEORDER:
        return s->big_endian_fb ?
            PCI_VGA_QEXT_BIG_ENDIAN : PCI_VGA_QEXT_LITTLE_ENDIAN;
    default:
        return 0;
    }
}

static void prism_qext_write(void *ptr, hwaddr addr,
                                     uint64_t val, unsigned size)
{
    PrismDispltState *s = ptr;

    switch (addr) {
    case PCI_VGA_QEXT_REG_BYTEORDER:
        if (val == PCI_VGA_QEXT_BIG_ENDIAN) {
            s->big_endian_fb = true;
        }
        if (val == PCI_VGA_QEXT_LITTLE_ENDIAN) {
            s->big_endian_fb = false;
        }
        break;
    }
}

static const MemoryRegionOps prism_qext_ops = {
    .read = prism_qext_read,
    .write = prism_qext_write,
    .valid.min_access_size = 4,
    .valid.max_access_size = 4,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

/*                                  for prism register                                */    
/**************************************************************************************\

 [prism_reg 1]:
           reserve                         clock                     vedio formate   able
 31-------------------------21 20----------------------------5 4---------------------1 0

 [prism_reg 2]:
           reserve                         width                        height
 31-------------------------24 23-------------------------12 11------------------------0

  [prism_reg 3]:
                                       width offset
 31------------------------------------------------------------------------------------0

  [prism_reg 4]:
                                      height offset
 31------------------------------------------------------------------------------------0

\**************************************************************************************/


static int prism_display_get_mode(PrismDispltState *s, PrismDisplyMode *mode){
    uint16_t *vbe = s->vbe_regs ;
    uint32_t *preg = s->prism_reg;
    uint32_t virt_width ;
    memset(mode, 0, sizeof(*mode));

    if(vbe[VBE_DISPI_INDEX_ENABLE] & VBE_DISPI_ENABLED){
        switch (vbe[VBE_DISPI_INDEX_BPP]) {
        case 16:
            /* best effort: support native endianness only */
            mode->format = PIXMAN_r5g6b5;
            mode->bytepp = 2;
            break;
        case 32:
            mode->format = s->big_endian_fb
                ? PIXMAN_BE_x8r8g8b8
                : PIXMAN_LE_x8r8g8b8;
            mode->bytepp = 4;
            break;
        default:
            return -1;
        }
        mode->width  = vbe[VBE_DISPI_INDEX_XRES];
        mode->height = vbe[VBE_DISPI_INDEX_YRES];
        virt_width  = vbe[VBE_DISPI_INDEX_VIRT_WIDTH];
        if (virt_width < mode->width) {
            virt_width = mode->width; //确保虚拟分辨率宽度不小于实际显示宽度
        }
        mode->stride = virt_width * mode->bytepp; //是显示缓冲区每一行的字节数，即每行的内存跨度
        mode->size   = (uint64_t)mode->stride * mode->height;
        mode->offset = ((uint64_t)vbe[VBE_DISPI_INDEX_X_OFFSET] * mode->bytepp + //显示缓冲区在内存中的偏移量
                        (uint64_t)vbe[VBE_DISPI_INDEX_Y_OFFSET] * mode->stride);

        if (mode->width < 64 || mode->height < 64) {
            return -1;
        }
        if (mode->offset + mode->size > s->vgamem) {
            return -1;
        }
        return 0;
    }
    if(preg[PRISM_DISPI_INDEX_ENABLE] & PRISM_DIAPLY_ENABLE){
        switch ((preg[PRISM_DISPI_REG1] & PRISM_DISPI_FORMATE_OFFSET)>>1)
        {
        case 15:
            mode->format = s->big_endian_fb
                ? PIXMAN_BE_x8r8g8b8
                : PIXMAN_LE_x8r8g8b8;
                mode->bytepp = 4;
            break;
        //其他情况暂时不实现
        default:
            return -1;
        }
        mode->width = (preg[PRISM_DISPI_REG2] & PRISM_DISPI_WIDTH_OFFSET )>> 12 ;
        mode->height = preg[PRISM_DISPI_REG2] & PRISM_DISPI_HEIGHT_OFFSET ;
        virt_width = mode->width ; //暂不支持扩展显示
        if (virt_width < mode->width) {
            virt_width = mode->width; 
        }
        mode->stride = virt_width * mode->bytepp;
        mode->size   = (uint64_t)mode->stride * mode->height;
        mode->offset = ((uint64_t)preg[PRISM_DISPI_REG3] * mode->bytepp) + ((uint64_t)preg[PRISM_DISPI_REG4] * mode->stride) ;
        if (mode->width < 64 || mode->height < 64) {
            return -1;
        }
        if (mode->offset + mode->size > s->vgamem) {
            return -1;
        }
        return 0;
    }

    return 0;
}

static void prism_diaplay_update(void *opaque){
    PrismDispltState *s = opaque ;
    DirtyBitmapSnapshot *snap = NULL;
    bool full_update = false ;
    PrismDisplyMode mode;
    DisplaySurface *ds ;
    uint8_t *ptr ;
    bool dirty ;
    int y, ys, ret ;

    ret = prism_display_get_mode(s, &mode);

    if(ret < 0){
        return;
    }

    if(memcmp(&s->mode, &mode, sizeof(mode)) != 0){
        s->mode = mode;
        ptr = memory_region_get_ram_ptr(&s->vram);
        ds = qemu_create_displaysurface_from(mode.width,
                                             mode.height,
                                             mode.format,
                                             mode.stride,
                                             ptr + mode.offset);
        dpy_gfx_replace_surface(s->con, ds);
        full_update = true;
    }

    if(full_update){
        dpy_gfx_update_full(s->con);
    }
    else {
        snap = memory_region_snapshot_and_clear_dirty(&s->vram,
                                                      mode.offset, mode.size,
                                                      DIRTY_MEMORY_VGA);
        ys = -1;
        for (y = 0; y < mode.height; y++) {
            dirty = memory_region_snapshot_get_dirty(&s->vram, snap,
                                                     mode.offset + mode.stride * y,
                                                     mode.stride);
            if (dirty && ys < 0) {
                ys = y;
            }
            if (!dirty && ys >= 0) {
                dpy_gfx_update(s->con, 0, ys,
                               mode.width, y - ys);
                ys = -1;
            }
        }
        if (ys >= 0) {
            dpy_gfx_update(s->con, 0, ys,
                           mode.width, y - ys);
        }

        g_free(snap);
    }
    //pci_set_irq(&s->pci, 1);
}

static GraphicHwOps prism_display_gfx_ops = {
    .gfx_update = prism_diaplay_update,
};

static bool prism_get_endian_fb(Object *obj, Error **errp){
    PrismDispltState *s = PRISM_DISPLT(obj);

    return s->big_endian_fb;
}

static void prism_set_endian_fb(Object *obj, bool value,
                                            Error **errp)
{
    PrismDispltState *s = PRISM_DISPLT(obj);
    s->big_endian_fb = value ;
}

static void prism_display_realize(PCIDevice *dev, Error **errp){
    PrismDispltState *s = PRISM_DISPLT(dev);
    Object *obj = OBJECT(dev);
	uint8_t *pci_conf = dev->config;
    int ret ;

    if (s->vgamem < 4 * MiB) {
        error_setg(errp, "prism: video memory too small");
        return;
    }
    if (s->vgamem > 256 * MiB) {
        error_setg(errp, "prism: video memory too big");
        return;
    }

    s->vgamem = pow2ceil(s->vgamem);

    s->con = graphic_console_init(DEVICE(dev), 0, &prism_display_gfx_ops, s);
    pci_config_set_interrupt_pin(pci_conf, 1);
    memory_region_init_ram(&s->vram, obj, "prism-vram", s->vgamem, errp);
    memory_region_init_io(&s->vbe, obj, &prism_vbe_ops,s, "prism-bochs-interface", PCI_VGA_BOCHS_SIZE);
    memory_region_init_io(&s->prismreg, obj, &prism_preg_ops, s, "prism-register", PRISM_REGISTER_SIZE);
    memory_region_init_io(&s->qext, obj, &prism_qext_ops, s,"prism-qemu-extended-regs", PCI_VGA_QEXT_SIZE);
    memory_region_init_io(&s->mmio, obj, &unassigned_io_ops, NULL, "prism-mmio", PCI_VGA_MMIO_SIZE);
    memory_region_add_subregion(&s->mmio, PCI_VGA_BOCHS_OFFSET, &s->vbe);
    memory_region_add_subregion(&s->mmio, PCI_VGA_QEXT_OFFSET, &s->qext);
    memory_region_add_subregion(&s->mmio, PRISM_REGISTER_OFFSET, &s->prismreg);
    
    pci_set_byte(&s->pci.config[PCI_REVISION_ID], 2);
    pci_register_bar(&s->pci, 0, PCI_BASE_ADDRESS_MEM_PREFETCH, &s->vram);
    pci_register_bar(&s->pci, 2, PCI_BASE_ADDRESS_SPACE_MEMORY, &s->mmio);

    if (s->enable_edid) {
        qemu_edid_generate(s->edid_blob, sizeof(s->edid_blob), &s->edid_info);
        qemu_edid_region_io(&s->edid, obj, s->edid_blob, sizeof(s->edid_blob));
        memory_region_add_subregion(&s->mmio, 0, &s->edid);
    }

    if (pci_bus_is_express(pci_get_bus(dev))) {
        ret = pcie_endpoint_cap_init(dev, 0x80);
        assert(ret > 0);
    } else {
        dev->cap_present &= ~QEMU_PCI_CAP_EXPRESS;
    }

    memory_region_set_log(&s->vram, true, DIRTY_MEMORY_VGA);

}

static void prism_exit(PCIDevice *dev)
{
    PrismDispltState *s = PRISM_DISPLT(dev);

    graphic_console_close(s->con);
}

static void prism_display_init(Object *obj){
    PCIDevice *dev = PCI_DEVICE(obj);
    object_property_add_bool(obj, "prism-endian-framebuffer",
                             prism_get_endian_fb,
                             prism_set_endian_fb);
    dev->cap_present |= QEMU_PCI_CAP_EXPRESS; 
}

static Property prism_properties[] = {
    DEFINE_PROP_SIZE("vgamem", PrismDispltState, vgamem, 16 * MiB),
    DEFINE_PROP_BOOL("edid", PrismDispltState, enable_edid, true),
    DEFINE_EDID_PROPERTIES(PrismDispltState, edid_info),
    DEFINE_PROP_END_OF_LIST(),
};

static void prism_display_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->class_id  = PCI_CLASS_DISPLAY_OTHER;
    k->vendor_id = PCI_VENDOR_ID_AMD;
    k->device_id = PCI_DEVICE_ID_AMD_LANCE;

    k->realize   = prism_display_realize;
    k->romfile   = "vgabios-bochs-display.bin";
    k->exit      = prism_exit;
    dc->vmsd     = &vmstate_prism_display;
    device_class_set_props(dc, prism_properties);
    set_bit(DEVICE_CATEGORY_DISPLAY, dc->categories);
}

static const TypeInfo bochs_display_type_info = {
    .name           = TYPE_PRISM_DISPLT,
    .parent         = TYPE_PCI_DEVICE,
    .instance_size  = sizeof(PrismDispltState),
    .instance_init  = prism_display_init,
    .class_init     = prism_display_class_init,
    .interfaces     = (InterfaceInfo[]) {
        { INTERFACE_PCIE_DEVICE },
        { INTERFACE_CONVENTIONAL_PCI_DEVICE },
        { },
    },
};


static void prism_display_register_types(void)
{
    type_register_static(&bochs_display_type_info);
}

type_init(prism_display_register_types)

 
