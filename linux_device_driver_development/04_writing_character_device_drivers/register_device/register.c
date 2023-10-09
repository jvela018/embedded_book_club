#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/fs.h>
#include <linux/cdev.h>

/* add formatting to the pr_* print macro */
#ifdef pr_fmt
    #undef pr_fmt
#endif
#define pr_fmt(fmt)  "[%s]:[ %s] " fmt, KBUILD_MODNAME , __func__


/* create a parameter that can be manupulated */ 
/* in command line and at run time */
char *name = "Embedded Book Club";
module_param(name, charp, S_IWUSR|S_IRUGO);
 

/* global parameters */
dev_t dev;
unsigned baseminor=0;
unsigned count=1;
int err;
int cadd_err;

struct cdev cdev;
struct file_operations fops;

struct class *class;
struct device *device;

/* module loading entry function */
static int __init init_hello(void)
{
	pr_info("Hello %s!\n", name);

	/* create major and minor number*/
	err = alloc_chrdev_region (&dev, baseminor, count, "device_numbers");
	pr_info("Device number <major>:<minor> %d:%d\n", MAJOR(dev), MINOR(dev));

	/* register char device sysfs */
	cdev_init (&cdev, &fops);
	cadd_err = cdev_add (&cdev, dev, count);

	/* create class and device to expose to /dev */
	class = class_create(THIS_MODULE, "ebc_class");
	device = device_create(class, NULL, dev, NULL, "ebc_device");


	return 0;
}

/* module unloading entry function */
static void __exit exit_hello(void)
{
	pr_info("Goodbye %s!\n", name);

	/* remove device from /dev */
	device_destroy(class, dev);

	/* remove class associated with device */	
	class_destroy(class);

	/* remove character driver */
	cdev_del(&cdev);

	/* remove device numbers */
	unregister_chrdev_region(dev, count);
	
}

/* load/unload module macro */
module_init(init_hello);
module_exit(exit_hello);

/* meta */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Club Finland");
MODULE_DESCRIPTION("Simple example to show the steps to register a char device onto sysfs");

