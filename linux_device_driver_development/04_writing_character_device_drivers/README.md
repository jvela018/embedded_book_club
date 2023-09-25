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
```

First column determines the type of file. If it starts with a __c__ or a __b__, it's a character or a block device. The major number represents the device type or associated driver, whereas the minor number the device itself. 


## Character Driver Main Data Structures

### cdev

Character devices are represented as an instance of a structure named cdev, found in __include/linux/cdev.h__

For simplicity, let's assume you didn't go through the first two chapters of the book. Your ubuntu distribution has its own headerfiles you can dig into, and if you don't have them, you can just use your package manager to get them. You can find cdev.h under __/usr/src/linux-headers-5.15.0-84-generic/include/linux/__. Note that these are my headefiles version, so you might have to change the version to look into what you've got.

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
        loff_t (*llseek) (struct file *, loff_t, int);                              // move cursor. Success: new position. Fail: negative value
        ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);           // retreive data. Sucess: number of bytes read. Fail: -EINVAL
        ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);    // send data. Sucess: number of bytes written. Fail: -EINVAL
        int (*flush) (struct file *, fl_owner_t id);                                // invoked when structured is released. Sucess: int. Fail: NULL
        __poll_t (*poll) (struct file *, struct poll_table_struct *);               // in the book the return type is unsigned int. Returns bitmask describing status of the device. Used to query whether device is writable, readable or in special state. If method not implemented, it's assumed that the device is always readable, writeable and in no special state. 
        int (*mmap) (struct file *, struct vm_area_struct *);                       // Used for memory mapped devices. Sucess: int. Fail: -ENODEV
        int (*open) (struct inode *, struct file *);                                // Backend of open system call. If not implemented it will always be successful.
        int (*release) (struct inode *, struct file *);                             // Response to the close system call. It's not mandatory and can be NULL like open.
        int (*fsync) (struct file *, loff_t, loff_t, int datasync);                 // Backend of fsync. Flush any pending data. If not implemented, it will fail and return -EINVAL.
        long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);        // Backend of ioctl system call. Extends predefined commands beyond reading and writing. Sucess: non-negative value. Fail: -ENOTTY no such ioctl for device
        
        
        ...

```

### File representation in the Kernel

There are two main structures that are fundamental to the file operations. If you pay attention to the arguments taken by the file operation methods, there are two that are consistent throughout all of them: __inode__ and __file__ . They're both defined in __linux/fs.h__

 
```c
    struct inode {
    ...
    union {
        struct pipe_inode_info  *i_pipe;
        struct cdev     *i_cdev;
        char            *i_link;
        unsigned        i_dir_seq;
    };
    ...

```

The structure __i_cdev__ enables direct interaction to the character device structure cdev. 

File is NOT FILE from the file descriptor in userspace. It's a filesystem structure that holds information about a file.

```c
    struct file {
        ...
        struct path          f_path;
        struct inode        *f_inode;   /* cached value */
        const struct file_operations    *f_op;
        loff_t f_pos;
        void *private_data;
        ...

```
__fpath__ represents the path to de file, and __f_inode__ the underlying inode pointing to the opened file. Hence, you can interact with __cdev__ through the inode.


## How to create device nodes

- Device nodes makes the interaction with underlying devices possible. 
- Identifiers can be statically or dynamically allocated. 
- Most drivers still use static identifiers due to compatibility issues.
- I took most prototypes from [Kernel API](https://archive.kernel.org/oldlinux/htmldocs/kernel-api)


### dev_t

- The __dev_t__ type is a 32-bit tuple that stores major and minor device numbers 
- The major number is represented by 12 bits
- The minor number is represented by 20 bits.
- You can use the macros under __linux/kdev_t.h__ to retreive major, minor, device numbers, etc.

```c
#define MINORBITS   20
#define MINORMASK   ((1U << MINORBITS) - 1)

#define MAJOR(dev)  ((unsigned int) ((dev) >> MINORBITS))
#define MINOR(dev)  ((unsigned int) ((dev) & MINORMASK))
#define MKDEV(ma,mi)    (((ma) << MINORBITS) | (mi))

```

### Registration and deregistration of character device numbers

In __linux/fs.h__


1. Create device numbers

__Static allocation__

```c
    int register_chrdev_region (dev_t first,
 	    unsigned count,
 	    const char * name);

```

__Dynamic allocation__

```c
    int alloc_chrdev_region (dev_t * dev,
        unsigned baseminor,
        unsigned count,
        const char * name);

```

2. Initialize and register a character device driver on the system

- The book uses dynamic allocation in this chapter
- To register the device you'll need to have created a device number __dev_t__
- The character device must be initialized __cdev_init()__
- After initialization, you must add the character device __cdev_add()__

__Initialization__

```c
    void cdev_init (	struct cdev * cdev,
        const struct file_operations * fops);

```

__Add__

```c
    int cdev_add (	struct cdev * p,
        dev_t dev,
        unsigned count);

```

- The book mentiones __void cdev(struct dev *)__ as the reverse operation to __cdev_add__, however, I'll leave the operations related removing the device from the system in a single section at the end.

- Once the character device is added to the system, you need to make it physically available to the user (under /dev).
- We first need to either use an existing class or create one
- The __class_create__ and __device_create__ functions will create and register a device with __sysfs__
- The class will be visible under __/sys/class__

__Create Class__

```c
    struct class * class_create(struct module * owner,
                                    const char * name);

```

__Create Device__

- This will create a device under __/dev__ associated to the class in __sysfs__

```c
    struct device * device_create(struct class *class,
                                    struct device *parent,
                                    dev_t devt,
                                    void *drvdata,
                                    const char *fmt, ...);

```

    - The book has a character device driver example in page 151.


__Removing everything from the system__

    - Just like we implemented the steps to register a device on __sysfs__, everything needs to be removed when unloading the driver.
    - The steps are the same steps taken earlier, but in reverse order.

```c
    void device_destroy(struct class *cls, dev_t devt);
    void class_destroy(struct class *cls);
    void cdev_del(struct cdev *);
    void unregister_chrdev_region(dev_t, unsigned);

```

## Implementing file operations

- Expose the device methods to userspace by means of system calls

### Kernel Space vs User Space

Let's revisit the read/write methods 

```c
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);           // retreive data. Sucess: number of bytes read. Fail: -EINVAL
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);    // send data. Sucess: number of bytes written. Fail: -EINVAL

        ...

```

- The use of __user prevents the direct interaction between user space and kernel space
- This is a cookie used by a semantic checker called Sparse to let the developer know they're about to use an unstrusted pointer.
- Two exchange data two functions are available to copy from user space to kernel space and vice versa

```c
    unsigned long copy_from_user(void *to, const void __user *from, unsigned long count)
    
```

```c
    unsigned long copy_to_user(void __user *to, const void *from, unsigned long count)
    
```

    - count = bytes_to_copy
    - Upon success they return the number of bytes succesfully read/written


- I will only go over the open and the write operations. The rest can be read from the book.

### The Open File Operation 

```c
int (*open) (struct inode *inode, struct file *filp);                     
```

- If it's not implemented it will always succed but the driver won't be aware of it.
- The book goes through an example.
- Check out the chapter 4 example of the open file operation in the book's [Linux Device Driver Development Repo](https://github.com/PacktPublishing/Linux-Device-Driver-Development-Second-Edition)
- Let's go over the book example (p.154)
- Go over container_of


### The write file operation


```c
ssize_t (*write) (struct file *filp, const char __user *buf, size_t count, loff_t *pos);    // send data. Sucess: number of bytes written. Fail: -EINVAL
```

- *buf is the data buffer coming from user space
- count is the number of bytes requested
- *pos is the start position from which data is written


1. Check for bad or invalid requests. Here filesize is the device memory size (not mandatory)

```c
if (*pos >= filesize) return -EINVAL;
```

2. Make sure that you don't write beyond file size (not mandatory)

```c
if (*pos + count > filesize)
    count = filesize - *pos;
```

3. If the device is backed by physical memory (not mandatory)
```c
void *from = pos_to_address(*pos);
```

4. Copy data from user space into kernel memory

```c
if (copy_from_user(dev->buffer, buf, count) != 0){
    retval = -EFAULT;
    goto out;
}

5. Move data from buffer to physical device and update position

```c
write_error = device_write(dev->buffer, count)
if(write_error)
    return -EFAULT;

*pos += count;
return count;

```

### The ioctl file operation