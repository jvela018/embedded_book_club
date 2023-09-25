# Understanding Linux Kernel Module Basic Concepts


# Topics Covered

## Hello World

```c
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */

static int __init init_hello(void)
{
	printk(KERN_INFO "Hello world !.\n");
    return 0;
}

static void __exit exit_hello(void)
{
	printk(KERN_INFO "Goodbye world !.\n");
}


module_init(init_hello);
module_exit(exit_hello);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Club Finland");

```

## Create Makefile and Build Module

1. Save Hello World example as hello.c

2. Create Makefile and edit accordingly

```bash
obj-m := hello.o

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build

all default: 
    make -C ${KERNEL_SRC} M=$(shell pwd) modules

clean:
	make -C ${KERNEL_SRC} M=$(shell pwd) clean

``` 


## Load and Unload modules

```bash
sudo insmod hello.ko
```

```bash
sudo rmmod hello
```

## Play with parameters


```bash
module_param(name, type, perm)
```
