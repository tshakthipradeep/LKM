#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

#include <linux/list.h>
#include <linux/uaccess.h>

#define NODE	"07_char_list"

dev_t device_num;
struct cdev char_cdev;
struct class *char_class;
struct device *char_device;

struct linux_list
{
	char *buff;
	struct list_head node;	
};

struct list_head head;
int node_count = 0;


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
	struct linux_list *node = NULL;
	struct list_head *cursor = NULL;
	struct list_head *next = NULL;
 
	list_for_each_safe(cursor,next,&head)
	{
#if 0
		node = container_of(cursor,struct linux_list,node);
#else
		node = list_entry(cursor,struct linux_list,node);
#endif

		copy_to_user(buff,node->buff,count);

		vfree(node->buff);
		list_del(cursor);
		node_count--;

		break;
	}

	pr_alert("In char_read: count = %d\n",node_count);

	return count;
}

ssize_t char_write(struct file *file, const char __user *buff, size_t count, loff_t *off)
{
	struct linux_list *node = (struct linux_list *)vmalloc(sizeof(struct linux_list));
	node->buff = (char *)vmalloc(count);

	copy_from_user(node->buff,buff,count);

	list_add(&node->node,&head);
	node_count++;

	pr_alert("In char_write: count = %d\n",node_count);

	return count;
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

	INIT_LIST_HEAD(&head);

	return 0;
}

void __exit cleanup_module(void)
{
	struct list_head *node, *next;
 
	list_for_each_safe(node,next,&head)
	{
		list_del(node);
		node_count--;
	}

	device_destroy(char_class,device_num);
	class_destroy(char_class);
	cdev_del(&char_cdev);
	unregister_chrdev_region(device_num,1);

	pr_alert("Entering cleanup_module: count = %d\n",node_count);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("07: Char List Module");
