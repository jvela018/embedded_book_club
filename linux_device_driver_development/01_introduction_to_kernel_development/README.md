# Introduction to Kernel Development

This chapter covers how to set up your development environment, how to configure your kernel and how to build the kernel.

# Topics covered

## Setting up your environment

1. Install packages in Debian system
   
```bash
sudo apt update;
```
    
```bash
sudo apt install gawk wget git diffstat unxip texinfo gcc-multiplib build-essential chrpath socat libsdl1.2-dev xterm ncurses-dev lzop libelf-dev make flex bison minicom
```

2. Install toolchain (binutils + cross compiler)

```bash
sudo apt install gcc binutils;
```
    
```bash
sudo apt install gcc-arm-linux-gnueabihf binutils-arm-linux-gnueabihf
```

3. Check cross compiler version

```bash
arm-linux-gnueabihf-gcc --version
```

## Downloading Kernel Source

1. Clone Linus' stable release

```bash
git clone https://github.com/torvalds/linux.git linux-stable --depth 1
```

## Configuring the Kernel

1. Use menuconfig target. You might want to export ARCH and CROSS_COMPILE for simplicity. This way you can reuse these variables in the same shell, during the build process

```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make menuconfig
```

you can also start from a generic configuration file found under arch/arm/configs/*

```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make imx_v6_v7_defconfig
```

## Building the Kernel

1. The make target will build all: vmlinux, zImage modules, and dtbs. The author of the book builds using nprocs*2

```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j$(nprocs)
```
You can also build each step at the time (e.g., make zImage, make modules )

2. When installing the modules (if needed), set an installation path so that it doesn't (over)write anything on the host machine.

```bash
ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=<DIR> make modules_install
```

__NOTE THAT THIS IS NOT ENOUGH TO BOOT YOUR TARGET. YOU'LL NEED A ROOT FILE SYSTEM, LIBC, COMPILE TOOL CHAIN, AND BOOTLOADER__


## Buildroot

Building everything needed to setup the board (i.e., booloader, kernel, etc) can be quite challenging. Using a build system can be quite helpful. A good option for simple embedded systems is Buildroot. For more complex projects see the Yocto Project.

For a good starting point to use buildroot to build a minimal linux system for the beaglebone black: [Bill Van Leeuwen's blog](https://blog.billvanleeuwen.ca/creating-a-minimal-linux-system-for-the-beaglebone-black-with-buildroot) 


