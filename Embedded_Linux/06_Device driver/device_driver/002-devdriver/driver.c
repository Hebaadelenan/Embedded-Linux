#include <linux/init.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heba");
MODULE_DESCRIPTION("Your module description");
static int read_flag = 0;

ssize_t mydriver_read(struct file *f, char __user *u, size_t s, loff_t *off) {
    if (read_flag == 0) {
        printk("read\n");
        copy_to_user(u, "am here\n", 8);
        read_flag = 1; // Set flag to indicate read has occurred
        return 8; // Return the number of bytes written
    }
	read_flag = 0; // Set flag to indicate read has occurred
    return 0; // Return 0 to indicate end-of-file
}
char buff[10];
ssize_t mydriver_write(struct file *f, const char __user *u, size_t s, loff_t *off)
{
	printk("write\n");
	copy_from_user(buff,u, s);
	return s;
}


struct cdev mycdev ;
struct class *myclass;
struct device *mydevice;

struct file_operations fops =
{
    .read = mydriver_read,
	.write = mydriver_write
};
dev_t device_number;
static int __init mydriver_init(void)
{
	printk("Hello\n");
    int r_err = 0;
	r_err = alloc_chrdev_region(&device_number,0,1,"mydev");
    if(r_err<0)
    {
        printk("failed to allocate");
        return 0;
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




