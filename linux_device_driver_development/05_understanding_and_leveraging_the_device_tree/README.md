# Understanding and Leveraging the Device Tree

A device tree is a hardware description file, with a JSON-like formatting style (key-value pairs).

# Topics Covered

- Device tree syntax and properties
- Inheritance
- Device tree compilation
- Representing and addressing: SPI, I2C and memory-mapped devices
- Handling Resources


## Basic concepts

- Support enabled in the kernel by enabling CONFIG_OF (OF = Open Firmware)
- The of.h and of_device.h headerfiles can be used in your driver to use the device tree API
- Go through __The device tree naming convention__ and __An introduction to the concept of aliases, labels, phandles and paths__ (p. 177 - 179)
- The device tree compiler (DTC) replace nodes prefixed by & with a phandle, removes node lables and replaces them. WIth respect to aliases, they are replaced by full paths.

## Device Tree sources and compilers

- Device tree comes in two flavors: textual form / source (__DTS__) and binary blob (__DTB__)

    ### DTS
    ```
    /dts-v/;
    
    /* multi-line comment */
    // line comment
    
    node-label: noden-name[@address]{
        property = ...

    };
    ```

    The properties can be string, list of strings, one-integer cell, integer list, mixed list, byte array and/or boolean. 

- The device tree blob (__DTB__) is also known as flattened device tree (__FDT__)
- The extensions are .dts, dtb, .dtbo (for binary overlays) and .dtsi (i means __include__) where the common files are.
- The device tree allows the use of #includes, #define and other directives, enabling the use of different source files.
- The device tree compiler (__DTC__) sources can be found as a standalone upstream project in the git repo
[DTC ](https://git.kernel.org/pub/scm/utils/dtc/dtc.git/)

, or in the kernel under scripts.
```bash
    ls /lib/modules/$(shell uname -r)/build/scripts/
```

- If you download it from the git repo you will need to install bison, flex and pkg-config if are not already available

```bash
sudo apt-get install flex bison pkg-config
```

And then build dtc
```bash
make NO_PYTHON=1
```

If you installed buildroot, your __dtc__ will be under \<buildroot\>/output/host/bin/, after building the kernel image.

- If you built your Kernel with buildroot, you'll find your __dtb__ binaries under \<buildroot\>/output/images, after building the kernel image

- You can see if the configuration file we used for buildroot had the appropiate configuration to build the device tree blobs. Go to the buildroot directory

```bash
make menuconfig
```

Search for __TREE__. You can also double check in the .config file.

- To cross compile the device tree you can manually using the kernel's __dtc__
```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make dtbs
```

You can also specify the device tree instead of using dtbs, which will write all trees.

- For debugging purposes you can add PROC_DEVICETREE (I couldn't find this in buildroot/beaglebone config file). This is supposed to create /proc/device-tree directory, exposing to userspace the current device tree of a running system (live device tree).

- You can also convert the binary .dtb to dts (human readable form)
```bash
dtc -I dtb -O dts <buildroot>/output/images/am335x-boneblack.dtb -o am335x-boneblack.dts
```

__Checkout the alias_labels_phandles_paths__ example.

# The device tree overlay

Device tree overlays enable loading changes on the DT at runtime. 

The syntax looks something like

```bash
/dts-v1/;
/plugin/;
/{

    fragment@1{

        target = <&cpu1>;
        __overlay__{
            status = "okay";
            label : node {
                property=...
            };
        };
    };
};
```

Note that in the example above, there's a reference to a phandle. Since both base and overlay are compiled separately, there's no symbol resolution as is. To be able to use the overlay, on the tree you'll need to compile both base and overlay using the -@ command. For example

```bash
dtc -@ -I dts -O dtb -o base.dtb base.dts
```
The -@ recognizes the plugin command and generates \_\_fixups\_\_ and \_\_local_fixups\_\_ nodes

```bash
dtc @ -I dts -O dtb -o my-overlay.dtbo my-overlay.dts
```
Since there's no plugin, dtc knows that this is a base tree, and generates of \_\_symbols\_\_ nodes.


## Building the device tree with the kernel
If you're building it with the kernel, you need to add the overlay to the Makefile. or example

```bash
dtb-y += my-overlay.dtbo
```

## Loading Device tree overlays

There are different ways to load device trees. The book focuses on loading DT via configs (configure kernel with CONFIG_CONFIGFS and CONFIG_OF_OVERLAY)

Mount config under /sys/kernel/config if the kernel didn't do it automatically at bootup. 

To add overlays you add a directory for them under sys/kernel//config/device-tree/overlays/\<your-overlay\>

Once it's set up, you can echo the path to the overlay directory or simply __cat__ the device tree overlay __DTBO__. For example:

### Path
```bash
echo /path/to/your-overlay > /sys/kernel/config/device-tree/overlays/foo/path
```

### DTBO
```bash
cat my-overlaydtbo > /sys/kernel/config/device-tree/overlays/foo/dtbo
```
To remove it just remove the directory where the overlay lives.

# Representing and adressing devices

There are two ways to address devices depending if they are

- Memory mapped
- Non-memory mapped 

## Non-memory mapped
If they're Non-memory mapped  like __SPI__ or __I2C__ devices, they're controlled by their parent node (bus controller driver) and will have indirect access on behalf of the CPU. 

The #size-cells property is 0 and the size element in the addressing tuple is empty. Then the __reg__ property only has the location address in the bus but no size.

## Memory-mapped 

The reg property defines address and size, so the #address-cells and #size-cells properties are at least 1. 


# Handling resources
Once probed there are resources assigned to the device

The underlying structure is called resource

```c
struct resource {
    resource_size_t start;
    resource_size_t end;
    const char *name;
    unsigned long flags;

};
```

Flags depedns on the type of resource IORESOURCE_IO, IORESOURCE_MEM..etc. (p197)

These resources are usually not assigned directly with the low level resource struct but by using generic API platform_get resource.

There are two ways to use this API, either using the index of the device or directly by name

See page 199.