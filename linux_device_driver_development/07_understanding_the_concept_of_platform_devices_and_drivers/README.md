# Understanding the Concept of Platform Devices and Drivers

- Linux handles devices using buses - matching CPUs to devices
- Some buses allow discoverability using enumeration (e.g., USB, PCI, SATA)
- Not all buses support discoverability, as a result the platform concept was invented
- Non-discoverable devices are known as platform devices, because they use the concept of platform bus, maintaining the Linux concept where all devices are connected through buses.

## The Platform Core Abstraction in the Linux Kernel

The structure lives in platform_device.h

```c
struct platform_device {
    const char  *name;
    int     id; 
    bool        id_auto;
    struct device   dev;
    u64     platform_dma_mask;
    struct device_dma_parameters dma_parms;
    u32     num_resources;
    struct resource *resource;

    const struct platform_device_id *id_entry;
    char *driver_override; /* Driver name to force a match */

    /* MFD cell pointer */
    struct mfd_cell *mfd_cell;

    /* arch specific additions */
    struct pdev_archdata    archdata;
};
```