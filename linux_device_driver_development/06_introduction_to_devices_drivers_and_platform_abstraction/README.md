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

__Note__: Devices struct are rarely used. Instead their higher level representation are used. For example:

- __struct i2c_client__
- __struct spi_device__
- __struct usb_device__
- __struct platform_device__

## Device Driver base structure

In the same manner, struct device_driver is the base structure for drivers. The structure lives device/driver.h

```c
struct device_driver {
        const char              *name;
        struct bus_type         *bus;

        struct module           *owner;
        const char              *mod_name;      /* used for built-in modules */

        bool suppress_bind_attrs;       /* disables bind/unbind via sysfs */
        enum probe_type probe_type;

        const struct of_device_id       *of_match_table;
        const struct acpi_device_id     *acpi_match_table;

        int (*probe) (struct device *dev);
        void (*sync_state)(struct device *dev);
        int (*remove) (struct device *dev);
        void (*shutdown) (struct device *dev);
        int (*suspend) (struct device *dev, pm_message_t state);
        int (*resume) (struct device *dev);
        const struct attribute_group **groups;
        const struct attribute_group **dev_groups;

        const struct dev_pm_ops *pm;
        void (*coredump) (struct device *dev);

        struct driver_private *p;
};
```

Once again, the names are self explanatory so I will only describe a short selection:

- __bus__: Mandatory field, representing the bus that the device of the driver belongs to. Driver registration fails otherwise, as the __bus__ structure calls the probe function which is responsible for matching driver-to-device.

- __of_match_table__: Array of struct of_device_id elements used for device tree matching
- __acpi_match_table__: Same as __of_match_table__ but for ACPI matching.

The methods: shutdown, suspend, resume and pm are optional, and they're used for power management.

### Driver Registration

Driver registration implies pushing the driver to the list maintained by the driver of the bus that it sits on top of.

Example:
- USB device driver is registered by inserting it into the list of drivers maintained by the USB controller driver

To register the device a function called driver_register() is used - not directly though. It will be a higher level representation depending on the bus, following this format __{bus_name}_register_driver()__. For example spi_register_driver().

These are usually during the registration/unregistration of drivers in the init/exit functions of the module. In those cases where only registration and unregistration is neeed (only action), the bus core provides specific helper macros. For example: __module_i2c_driver(\_\_spi_driver);__

If the bus controller doesn't provide these macros, you can use the more general way by using the platform_driver structure with the platform_driver_register() function within the init/exit functions and call them from the module_init and module_exit macros, __OR__ skip init/exit functions and use the module_platform_driver() macro.

### Expose devices in driver

How can the kernel know when a device pops up in the system (the bus)? If a device pops up, and the bus finds a driver that supports that device, it will call the driver's probe function (as long as the driver is loaded in userspace or auto-loading is set up). 

Let's have a look at one of the __bus-specific__ device driver structures. For example in __i2c.h__

```c
struct i2c_driver {
        unsigned int class;

        /* Standard driver model interfaces */
        int (*probe)(struct i2c_client *client, const struct i2c_device_id *id);
        void (*remove)(struct i2c_client *client);

        /* New driver model interface to aid the seamless removal of the
         * current probe()'s, more commonly unused than used second parameter.
         */
        int (*probe_new)(struct i2c_client *client);

        /* driver model interfaces that don't relate to enumeration  */
        void (*shutdown)(struct i2c_client *client);

        /* Alert callback, for example for the SMBus alert protocol.
         * The format and meaning of the data value depends on the protocol.
         * For the SMBus alert protocol, there is a single bit of data passed
         * as the alert response's low bit ("event flag").
         * For the SMBus Host Notify protocol, the data corresponds to the
         * 16-bit payload data reported by the slave device acting as master.
         */
        void (*alert)(struct i2c_client *client, enum i2c_alert_protocol protocol,
                      unsigned int data);

        /* a ioctl like command that can be used to perform specific functions
         * with the device.
         */
        int (*command)(struct i2c_client *client, unsigned int cmd, void *arg);

        struct device_driver driver;
        const struct i2c_device_id *id_table;
        [...]
};
```

__Note__: The __i2c_driver__ has a member which is a pointer to an id_table, which is an array of device IDs supported by the driver.

If we go back to the base device_driver structure at the top
```c
struct device_driver {
        [...]
        const struct of_device_id       *of_match_table;
        const struct acpi_device_id     *acpi_match_table;
        [...]
```

We see that there are two other types of table. The OF and the ACPI match tables. The OF exposes devices that are declared within the device tree, and the acpi_match_table for ACPI. 

The main difference between using the bus specific device id table and the of device id table is the syntax. See page 322. 

### Matching and module auto loading

