#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/uaccess.h>

static struct kobject *kobj;

static char attr_value[PAGE_SIZE] = "";

ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	strncpy(buf,attr_value,strlen(attr_value));
	return strlen(attr_value);
}

ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int len = (count > PAGE_SIZE) ? PAGE_SIZE : count;

	strncpy(attr_value,buf,len);
	pr_alert("attr->name: %s\n",attr->attr.name);
	return count;
}

static struct kobj_attribute attr = __ATTR("sysfs_01",0666,sysfs_show,sysfs_store);

int __init init_module(void)
{
	int ret = 0;

	pr_alert("Entering init_module\n");

	/*
	* kobject_create_and_add will allocate zeored memory to kobj, call koject_init,
	* kobject_add, assign name and  assign parent.
	*
	* kernel_kobj is a kernel object which is part of the kernel. Using this as the
	* parent will create sysfs entry under /sys/kernel directory.
	*
	* Look for attr directory under /sys/kernel directory
	*/

	kobj = kobject_create_and_add("attr",kernel_kobj);
		
	if(kobj == NULL)
	{
		pr_err("kobject_create_and_add failed\n");
		ret = -ENOMEM;
		goto error;
	}

	ret = sysfs_create_file(kobj,&attr.attr);

	if(ret != 0)
	{
		pr_err("sysfs_create_file failed");
		goto error;
	}

	goto done;

error:
	kobject_put(kobj);
done:
	return ret;
}

void __exit cleanup_module(void)
{
	kobject_put(kobj);
	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("08: sysfs attr module");
