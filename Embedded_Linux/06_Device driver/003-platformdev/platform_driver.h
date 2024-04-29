#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#define BUFFER_SIZE 100

struct irq_affinity;
struct mfd_cell;
struct property_entry;
struct platform_device_id;


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
    char buff[BUFFER_SIZE];
	struct cdev mycdev;
};

enum device_names
{
    AXZ=0,BXZ,CXZ,DXZ
};


