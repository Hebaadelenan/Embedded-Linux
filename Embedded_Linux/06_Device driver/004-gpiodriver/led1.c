#include "gpio_driver.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Heba");
MODULE_DESCRIPTION("Your module description");



struct private_dvc_data prvDevData = {
    .id = LED1,
    .perm = WRONLY,
    .size = BUFFER_SIZE,
    .buff[0]=1
};


struct platform_device mydev= {
    .name ="LED1", 
    .id = 1,
    .dev = {.platform_data = &prvDevData }
};




static int __init mydriver_init(void)
{

    platform_device_register(&mydev);

	return 0;	

}

static void __exit mydriver_exit(void)
{
    platform_device_unregister(&mydev);
}

module_init(mydriver_init);
module_exit(mydriver_exit);
