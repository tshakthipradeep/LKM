#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define NODE	"06_char"

dev_t device_num;
struct cdev char_cdev;
struct class *char_class;
struct device *char_device;

int char_open(struct inode *inode, struct file *file)
{
	pr_alert("In char_open, manjor: %d, minor: %d\n",
		imajor(inode),
		iminor(inode));

	file->private_data = (void *)inode;
	return 0;
}

int char_release(struct inode *inode, struct file *file)
{
	pr_alert("In char_release, manjor: %d, minor: %d\n",
		imajor((struct inode *)file->private_data),
		iminor((struct inode *)file->private_data));

	return 0;
}

long char_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	pr_alert("In char_unlocked_ioctl\n");
	return 0;
}

ssize_t char_read(struct file *file, char __user *buff, size_t count, loff_t *off)
{
	pr_alert("In char_read\n");
	return 0;
}

ssize_t char_write(struct file *file, const char __user *buff, size_t count, loff_t *off)
{
	pr_alert("In char_write\n");
	return 0;
}

struct file_operations char_fops = {
    .owner 		= THIS_MODULE,
    .read 		= char_read,
    .write 		= char_write,
    .unlocked_ioctl 	= char_unlocked_ioctl,
    .open 		= char_open,
    .release 		= char_release,
};

int __init init_module(void)
{
	int ret = 0;

	pr_alert("Entering init_module\n");

	ret = alloc_chrdev_region(&device_num,0,1,NODE);

	if(ret != 0)
	{
		pr_err("alloc_chrdev_region failed\n");
		return -1;
	}
	

	cdev_init(&char_cdev,&char_fops);
	char_cdev.owner = THIS_MODULE;
	char_cdev.ops = &char_fops;

	ret = cdev_add(&char_cdev,device_num,1);

	if(ret != 0)
        {
                pr_err("cdev_add failed\n");
                return -1;
        }

	char_class = class_create(THIS_MODULE,NODE);

	if(IS_ERR(char_class))
	{
		pr_err("class_create failed\n");
		ret = PTR_ERR(char_class);
		return ret;
	}

	char_device = device_create(char_class,NULL,device_num,NULL,NODE);

	if(IS_ERR(char_device))
	{
		pr_err("device_create failed\n");
		ret = PTR_ERR(char_device);
		return ret;
	}

	return 0;
}

void __exit cleanup_module(void)
{
	device_destroy(char_class,device_num);
	class_destroy(char_class);
	cdev_del(&char_cdev);
	unregister_chrdev_region(device_num,1);

	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("06: Char Module");
