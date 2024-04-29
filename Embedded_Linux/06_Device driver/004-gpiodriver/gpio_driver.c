#include "gpio_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heba");
MODULE_DESCRIPTION("Your module description");

static int read_flag = 0;

struct private_driver_data prv_driver =
{
    .device_count =3,
};

const struct platform_device_id mytable[] = {
 {.name ="LED0", .driver_data = LED0},
 {.name ="LED1", .driver_data = LED1},
 {.name ="LED2", .driver_data = LED2},
 {}
};

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
    struct private_dvc_data *pcdev = container_of(my_inode->i_cdev,struct private_dvc_data, mycdev);
    my_file->private_data = pcdev;
    r_err = check_openPermissions(pcdev->perm, my_file->f_mode);
    printk("file opened\n");
    return r_err;
}

ssize_t mydriver_read(struct file *my_file, char __user *u, size_t s, loff_t *off) {
    
    printk("read\n");
    struct private_dvc_data *pcdev = (struct private_dvc_data *) my_file->private_data;
    
    if (read_flag == 0) {
        copy_to_user(u, pcdev->buff, pcdev->size);
        read_flag = 1; // Set flag to indicate read has occurred
        return 8; // Return the number of bytes written
    }
	read_flag = 0; // Set flag to indicate read has occurred
    return 0; // Return 0 to indicate end-of-file
}


ssize_t mydriver_write(struct file *f, const char __user *u, size_t s, loff_t *off)
{
	printk("write\n");
    struct private_dvc_data *pcdev = f->private_data;

    memset(pcdev->buff, 0, pcdev->size); // Clear the device buffer
    
    copy_from_user(pcdev->buff, u, 1);
    switch(pcdev->buff[0])
    {
        case '0':
            gpio_set_value(START_PIN + pcdev->id, 0);
            break;
        case '1':
            gpio_set_value(START_PIN + pcdev->id, 1);
            break;
        default:
            printk("Invalid data\n");
            return -EINVAL;
    }
	return s;
}

struct file_operations fops =
{
    .open = my_open,
    .read = mydriver_read,
	.write = mydriver_write,
    .owner = THIS_MODULE 
    
};





int my_remove(struct platform_device * p)
{
    struct private_dvc_data* pcdev;
    pcdev = dev_get_platdata(&p->dev);
    device_destroy(prv_driver.myclass,prv_driver.device_number +pcdev->id);
    cdev_del(&pcdev->mycdev);
    return 0;
}



int my_probe(struct platform_device *mytab)
{
        printk("device detected\n");
        struct private_dvc_data* pcdev;

        pcdev = dev_get_platdata(&mytab->dev);
        
        printk("My default buff data %d \n",pcdev->buff[0]);       
        cdev_init(&pcdev->mycdev,&fops);
        cdev_add(&pcdev->mycdev,prv_driver.device_number + mytab->id,1);
        //mycdev[i].owner=THIS_MODULE;
        prv_driver.mydevice = device_create(prv_driver.myclass,NULL,prv_driver.device_number + mytab->id,NULL,"mydevdevice%d",mytab->id); 

        return 0;

}



struct platform_driver platform_driver_data =
{
    .probe = my_probe,
    .remove = my_remove,
    .id_table = mytable,
    .driver = {
        .name = "mydrive"
    }

};


static int __init mydriver_init(void)
{
	printk("Hello Gpio\n");
    int r_err = 0;

    
	r_err = alloc_chrdev_region(&prv_driver.device_number,0,3,"mydev");

    if(r_err<0)
    {
        printk("failed to allocate\n");
        return 0;
    }

    prv_driver.myclass=class_create("mydevclass");

    platform_driver_register(&platform_driver_data);
	
    return 0;	

}



static void __exit mydriver_exit(void)
{
	printk("Bye Bye\n");

    platform_driver_unregister(&platform_driver_data); //Optional
    class_destroy(prv_driver.myclass);
    unregister_chrdev_region(prv_driver.device_number,3);
}

module_init(mydriver_init);
module_exit(mydriver_exit);




