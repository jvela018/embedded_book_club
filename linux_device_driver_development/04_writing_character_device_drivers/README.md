# Writing Character Device Drivers

# Topics Covered

- Major and minor numbers
- Character device data structure intro
- Device nodes
- File operations

## Major and Minor Numbers

Devices file identification is envorced by unique identifier composed of a major and a minor number.
	
    ``` bash
    ls  -la /dev
    ``

First column determines the type of file. If it starts with a __c__ or a __b__, it's a character or a block device. The major number represents the device type or associated driver, whereas the minor number the device itself. 


## Character Driver Main Data Structures

### cdev

Character devices are represented as an instance of a structure named cdev, found in include/linux/cdev.h

For simplicity, let's assume you didn't go through the first two chapters of the book. Your ubuntu distribution has its own headerfiles you can dig into, and if you don't have them, you can just use your package manager to get them. You can find cdev.h under /usr/src/linux-headers-5.15.0-84-generic/include/linux/. Note that these are my headefiles version, so you might have to change the version to look into what you've got.

    ```bash
    vim /usr/src/linux-headers-5.15.0-84-generic/include/linux/cdev.h
    ```

Let's look at the data structure

    ```c
    struct cdev {
        struct kobject kobj;
        struct module *owner;
        const struct file_operations *ops;
        struct list_head list;
        dev_t dev;
        unsigned int count;
    } __randomize_layout;
    ```

### file_operations

The operations supported by a given device are represented by cdev->fops, where fops is an instance of the file_operations structure. 
These operations can be found under __linux/fs.h__. A snippet of this structure

    ```c
    struct file_operations {
        struct module *owner;                                                       // mandatory field pointing to the module owning this structure __THIS_MODULE
        loff_t (*llseek) (struct file *, loff_t, int);                              //
        ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);           //
        ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);    //
        int (*flush) (struct file *, fl_owner_t id);                                //
        __poll_t (*poll) (struct file *, struct poll_table_struct *); // in the book the return type is unsigned int
        int (*mmap) (struct file *, struct vm_area_struct *);
        int (*open) (struct inode *, struct file *);
        int (*release) (struct inode *, struct file *);
        int (*fsync) (struct file *, loff_t, loff_t, int datasync);
        ...
    ```

