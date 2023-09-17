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
        lzop libelf-dev make
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