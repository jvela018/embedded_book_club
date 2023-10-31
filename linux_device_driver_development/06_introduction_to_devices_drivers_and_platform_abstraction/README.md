# Introduction to Devices, Drivers and Platform Abstraction

This chapter touches on the data structures that describe devices, drivers and buses, and their interaction with one another. Particularly, the device-driver matching mechanism.

Although, the structs are part of the Linux Device Model, it is addressed on another chapter in a comprehensive manner.

## Device base structure

The device struct helps extract physical/virtual device information. Struct can be found in the __device.h__ headerfile

```c
struct device {
        struct kobject kobj;
        struct device           *parent;

        struct device_private   *p;

        const char              *init_name; /* initial name of the device */
        const struct device_type *type;

        struct bus_type *bus;           /* type of bus device is on */
        struct device_driver *driver;   /* which driver has allocated this
                                           device */
        void            *platform_data; /* Platform specific data, device
                                           core doesn't touch it */
        void            *driver_data;   /* Driver data, set and get with
                                           dev_set_drvdata/dev_get_drvdata */
        struct mutex            mutex;  /* mutex to synchronize calls to
                                         * its driver.
                                         */

        struct dev_links_info   links;
        struct dev_pm_info      power;
        struct dev_pm_domain    *pm_domain;
        [...]
        dev_t                   devt;   /* dev_t, creates the sysfs "dev" */
        u32                     id;     /* device instance */

        spinlock_t              devres_lock;
        struct list_head        devres_head;
        [...]
}
```

As you see there are many members of the struct device data type, however the book only describes parent, kobj, bus, driver, platform_data, driver_data, pm_domain, of_node, and id.

Everything is documented quite well within the headerfile. For that reason I will only address two of the members:

- __platform_data__: This field is automatically set. It points to board specific structures, from the board setup file. Avoids the use of #ifdefs and contains chip variants, GPIO pin roles and interrupt lines.
- __of_node__: Device tree associated with device and filled by the Open Firmware (OF) core when device is declared in the device tree.