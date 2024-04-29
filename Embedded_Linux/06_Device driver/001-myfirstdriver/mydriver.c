#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
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
int buff[10];
ssize_t mydriver_write(struct file *f, const char __user *u, size_t s, loff_t *off)
{
	printk("write\n");
	copy_from_user(buff,u, s);
	return s;
}


struct proc_ops pops= {
	.proc_read = mydriver_read,
	.proc_write = mydriver_write,
};

struct proc_dir_entry *de = NULL ;

static int __init mydriver_init(void)
{
	printk("Hello\n");

	de=proc_create("proc_file", 0,
				   NULL,
				   &pops);

	return 0;	

}

static void __exit mydriver_exit(void)
{
	printk("Bye Bye\n");
	proc_remove(de);

}

module_init(mydriver_init);
module_exit(mydriver_exit);




