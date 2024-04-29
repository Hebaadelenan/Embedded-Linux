#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heba");
MODULE_DESCRIPTION("Your module description");

#define RDONLY 0b01
#define WRONLY 0b10
#define RDWR   0b11
#define BUFF_SIZE 100

static int read_flag = 0;
char buff[BUFF_SIZE];
unsigned g_offset =0;
int perm = RDWR;

struct cdev mycdev ;
struct class *myclass;
struct device *mydevice;
dev_t device_number;



int check_openPermissions(int perm, int access_mode)
{
    //  0       --> allowed
    //  -EPERM  --> operation not permitted
    

    if(perm == RDWR)
    {
        return 0;
    }

    /* Read only */
    if((perm == RDONLY) && ((access_mode & FMODE_READ) && !(access_mode & FMODE_WRITE)))
    {
        return 0;
    }

    /* Write only */
    if((perm == WRONLY) && ((access_mode & FMODE_WRITE) && !(access_mode & FMODE_READ)))
    {
        return 0;
    }

    /* Operation not permitted */
    return -EPERM;
}

int my_open(struct inode *my_inode, struct file *my_file)
{
    int r_err = 0;
    r_err = check_openPermissions(perm, my_file->f_mode);
    printk("file opened\n");
    return r_err;
}

ssize_t mydriver_read(struct file *my_file, char __user *u, size_t s, loff_t *off_set) {
    
    printk("read\n");
    if (read_flag == 0) {
        copy_to_user(u, buff, BUFF_SIZE);
        read_flag = 1; // Set flag to indicate read has occurred
        return BUFF_SIZE; // Return the number of bytes written
    }
	read_flag = 0; // Set flag to indicate read has occurred
    return 0; // Return 0 to indicate end-of-file
}

ssize_t mydriver_write(struct file *f, const char __user *u, size_t s, loff_t *off)
{
	printk("write\n");
    if (f->f_flags & O_APPEND) {
        *off = g_offset; // Set offset to the end of the device buffer
    }
    else    // clear the device buffer (important for rewriting)
    {
        memset(buff, 0, BUFF_SIZE); // Clear the device buffer
    }

    copy_from_user(buff +(* off), u, s);

    *off += s;                      // Update the offset
    g_offset = *off;                  // Update the global offset

	return s;
}


struct file_operations fops =
{
    .read = mydriver_read,
	.write = mydriver_write,
    .open = my_open
};

static int __init mydriver_init(void)
{
	printk("Hello\n");
    int r_err = 0;
	r_err = alloc_chrdev_region(&device_number,0,1,"mydev");
    if(r_err<0)
    {
        printk("failed to allocate");
        return r_err;
    }
    cdev_init(&mycdev,&fops);
    cdev_add(&mycdev,device_number,1);
    mycdev.owner=THIS_MODULE;
    myclass=class_create("mydevclass");
    mydevice = device_create(myclass,NULL,device_number,NULL,"mydevdevice");
    


	return 0;	

}

static void __exit mydriver_exit(void)
{
	printk("Bye Bye\n");
	device_destroy(myclass,device_number);
    class_destroy(myclass);
    cdev_del(&mycdev);
    unregister_chrdev_region(device_number,1);

}

module_init(mydriver_init);
module_exit(mydriver_exit);




