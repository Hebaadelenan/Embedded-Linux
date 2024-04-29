#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/gpio.h>

#define BUFFER_SIZE 10
#define START_PIN 10
#define RDONLY 0b01
#define WRONLY 0b10
#define RDWR   0b11

struct private_driver_data
{
    int     device_count;
    dev_t   device_number;
    struct class*   myclass;
    struct device*  mydevice;  
};

struct private_dvc_data
{
    int id;
    ssize_t size;
    int perm;
    int buff[BUFFER_SIZE];
	struct cdev mycdev;
};

enum device_names
{
    LED0=0,LED1,LED2
};


