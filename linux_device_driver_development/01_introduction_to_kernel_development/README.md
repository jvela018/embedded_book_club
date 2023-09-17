# Introduction to Kernel Development

This chapter covers how to set up your development environment, how to configure your kernel and how to build the kernel.

# Topics covered

## Setting up your environment

1. Install packages in Debian system
    ```
    sudo apt update;
    sudo apt install gawk wget git diffstat \ 
        unxip texinfo gcc-multiplib \
        build-essential chrpath socat \
        libsdl1.2-dev xterm ncurses-dev \
        lzop libelf-dev make flex bison
    ```

2. Install toolchain (binutils + cross compiler)

    ```
    sudo apt install gcc binutils;
    sudo apt install gcc-arm-linux-gnueabihf \ 
        binutils-arm-linux-gnueabihf
    ```

3. Check cross compiler version

    ```
    arm-linux-gnueabihf-gcc --version
    ```

## Downloading Kernel Source

1. Clone Linus' stable release

    ```
    git clone https://github.com/torvalds/linux.git linux-stable --depth 1
    ```

## Configuring the Kernel

1. Use menuconfig target. You might want to export ARCH and CROSS_COMPILE for simplicity. This way you can reuse these variables in the same shell, during the build process

    ```
    ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make menuconfig
    ```

    ## Building the Kernel

    1. The make target will build all: vmlinux, zImage modules, and dtbs. The author of the book builds using nprocs*2

    ```
    ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- make -j$(nprocs)
    ```
You can also build each step at the time (e.g., make zImage, make modules )

2. When installing the modules (if needed), set an installation path so that it doesn't (over)write anything on the host machine.

    ```
    ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=<DIR> make modules_install
    ```