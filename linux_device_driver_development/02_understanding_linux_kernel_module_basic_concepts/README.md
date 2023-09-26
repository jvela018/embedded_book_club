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

Check if module was loaded in __sysfs_

```bash
ls -al /sys/module | grep hello
```

Check the printk statement on the Kernel's ring buffer

```bash
dmesg | tail -1
```

```bash
sudo rmmod hello
```

Check if module was removed

## Play with parameters


```bash
module_param(name, type, perm)
```

# hello.c

- Load and unload module
- Change the variable name from command line 
- Print messages using printk and pr_fmt
- Format pr_fmt to include module and function information
- You can run the example manually or just run

```bash
./run.sh
```

If it doesn't run, run the command below and try again.

```bash
chmod +x run.sh
```

- The run.sh script
	- Removes all artifacts
	- Builds the module
	- Loads the module
	- Checks that the module can be found under sysfs (/sys/)
	- Prints the message sent to the kernel buffer
	- Unloads the module
	- Checks that the module has been removed from sysfs (/sys/)
	- Prints message sent to the kernel

- You can also interact with the module when you load it changing the variable name

```bash
./run.sh name=Mikko
```