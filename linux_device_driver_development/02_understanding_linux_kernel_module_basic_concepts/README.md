# Understanding Linux Kernel Module Basic Concepts


# Topics Covered

## Hello World

	```
	#include <linux/module.h>	/* Needed by all modules */
	#include <linux/kernel.h>	/* Needed for KERN_INFO */

	static int __init init_module(void)
	{
		printk(KERN_INFO "Hello world !.\n");
    	/* 
		* A non 0 return means init_module failed; module can't be loaded. 
		*/
    return 0;
	}

	static void __exit exit_module(void)
	{
	    printk(KERN_INFO "Goodbye world !.\n");
	}


	module_init(init_module);
	module_exit(exit_module);

	MODULE_LICENSE("GPL");
	MODULE_AUTHOR("Embedded Club Finland");
	```

## Create Makefile and Build Module

1. Save Hello World example as hello.c

2. Create Makefile and edit accordingly

	```
    obj-m := hello.o

    KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build

    all default: modules
    modules help clean:
    make -C $KERNEL_SRC M=$(shell pwd) @
	``` 


## Load and Unload modules

	```
	insmod hello.ko
	```

	```
	rmmod hello
	```

## Play with parameters


	```
	module_param(name, type, perm)
	```
