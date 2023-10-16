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

If you installed buildroot, your __dtc__ will be under \<buildroot\>/output/host/bin/.

- If you built your Kernel with buildroot, you'll find your __dtb__ binaries under \<buildroot\>/output/images

- You can see if the configuration file we used for buildroot had the appropiate configuration to build the device tree blobs. Go to the buildroot directory

```bash
make menuconfig
```

Search for __TREE__. You can also double check in the .config file.

- To cross compile the device tree you can manually
```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make dtbs
```

You can also specify the device tree instead of using dtbs, which will write all trees.

- For debugging purposes you can add PROC_DEVICETREE (I couldn't find this in buildroot/beaglebone config file). This is supposed to create /proc/device-tree directory, exposing to userspace the current device tree of a running system (live device tree).

- You can also convert the binary .dtb to dts (human readable form)
```bash
dtc -I dtb -O dts <buildroot>/output/images/am335x-boneblack.dtb -o am335x-boneblack.dts
```

# The device tree overlay
