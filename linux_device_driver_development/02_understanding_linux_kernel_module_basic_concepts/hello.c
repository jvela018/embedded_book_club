#include <linux/module.h>   /* Needed by all modules */


/* add formatting to the pr_* print macro */
#ifdef pr_fmt
	#undef pr_fmt
#endif
#define pr_fmt(fmt)  "[%s]:[ %s] " fmt, KBUILD_MODNAME , __func__


/* create a parameter that can be manupulated */ 
/* in command line and at run time */
char *name = "Embedded Book Club";
module_param(name, charp, S_IWUSR|S_IRUGO);


/* module loading entry function */
static int __init init_hello(void)
{
	pr_info("Hello %s!\n", name);
	return 0;
}


/* unloading module entry function */
static void __exit exit_hello(void)
{
	printk(KERN_INFO "Goodbye %s\n", name);
}


/* load/unload module macro */
module_init(init_hello);
module_exit(exit_hello);


/* meta */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Embedded Club Finland");
MODULE_DESCRIPTION("Simple example to load, unload modules and print messages to the kernel ring buffer");

