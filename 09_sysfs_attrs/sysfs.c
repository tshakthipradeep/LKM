#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/uaccess.h>

static struct kobject *kobj;

static char attr_1_value[PAGE_SIZE] = "";
static char attr_2_value[PAGE_SIZE] = "";
static char attr_3_value[PAGE_SIZE] = "";

ssize_t sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	if(strcmp(attr->attr.name,"attr_1") == 0)
	{
		strncpy(buf,attr_1_value,strlen(attr_1_value));
		return strlen(attr_1_value);
	}
	else
	if(strcmp(attr->attr.name,"attr_2") == 0)
        {
                strncpy(buf,attr_2_value,strlen(attr_2_value));
                return strlen(attr_2_value);
        }
	else
        if(strcmp(attr->attr.name,"attr_3") == 0)
        {
                strncpy(buf,attr_3_value,strlen(attr_3_value));
                return strlen(attr_3_value);
        }

	return 0;
}

ssize_t sysfs_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
	int len = (count > PAGE_SIZE) ? PAGE_SIZE : count;

        if(strcmp(attr->attr.name,"attr_1") == 0)
        {
		strncpy(attr_1_value,buf,len);
        }
	else
	if(strcmp(attr->attr.name,"attr_2") == 0)
        {
                strncpy(attr_2_value,buf,len);
        }
	else
        if(strcmp(attr->attr.name,"attr_3") == 0)
        {
                strncpy(attr_3_value,buf,len);
        }
	
	return len;
}

static struct kobj_attribute attr_1 = __ATTR(attr_1,0666,sysfs_show,sysfs_store);
static struct kobj_attribute attr_2 = __ATTR(attr_2,0666,sysfs_show,sysfs_store);
static struct kobj_attribute attr_3 = __ATTR(attr_3,0666,sysfs_show,sysfs_store);

static struct attribute *attrs[] = {
	&attr_1.attr,
	&attr_2.attr,
	&attr_3.attr,
	NULL
};

static struct attribute_group attr_group = {
	.name = "attr_group",
	.attrs = attrs,
};

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

	ret = sysfs_create_group(kobj,&attr_group);

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
MODULE_DESCRIPTION("09: sysfs attrs Module");
