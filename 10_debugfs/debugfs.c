#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/debugfs.h>
#include <linux/uaccess.h>

static u8 u8_value = 8;
static u16 u16_value = 16;
static u32 u32_value = 32;

static struct dentry *debugfs_dir;
static struct dentry *debugfs_u8;
static struct dentry *debugfs_u16;
static struct dentry *debugfs_u32;

int __init init_module(void)
{
	pr_alert("Entering init_module\n");

	debugfs_dir = debugfs_create_dir("debugfs",NULL);

	if(debugfs_dir == NULL)
	{
		pr_alert("debugfs_create_dir failed");
		goto error1;
	}
	else
	if(IS_ERR(debugfs_dir))
	{
		pr_alert("debugfs_create_dir failed");
		goto error1;
	}

	debugfs_u8 = debugfs_create_u8("u8_value",0666,debugfs_dir,&u8_value);

        if(debugfs_u8 == NULL)
	{
                pr_alert("debugfs_create_u8 failed");
		goto error2;
	}
        else
        if(IS_ERR(debugfs_u8))
	{
                pr_alert("debugfs_create_u8 failed");	
		goto error2;
	}

        debugfs_u16 = debugfs_create_u16("u16_value",0666,debugfs_dir,&u16_value);

        if(debugfs_u16 == NULL)
        {
                pr_alert("debugfs_create_u16 failed");
                goto error3;
        }
        else
        if(IS_ERR(debugfs_u16))
        {
                pr_alert("debugfs_create_u16 failed");
                goto error3;
        }

        debugfs_u32 = debugfs_create_u32("u32_value",0666,debugfs_dir,&u32_value);

        if(debugfs_u32 == NULL)
        {
                pr_alert("debugfs_create_u8 failed");
                goto error4;
        }
        else
        if(IS_ERR(debugfs_u32))
        {
                pr_alert("debugfs_create_u32 failed");
                goto error4;
        }

	goto done;
error4:
	debugfs_remove(debugfs_u32);
error3:
	debugfs_remove(debugfs_u16);
error2:
	debugfs_remove(debugfs_u8);
error1:
	debugfs_remove(debugfs_dir);
done:
	return 0;	
}

void __exit cleanup_module(void)
{
	pr_alert("Entering cleanup_module\n");
	debugfs_remove(debugfs_u8);
	debugfs_remove(debugfs_u16);
	debugfs_remove(debugfs_u32);
	debugfs_remove(debugfs_dir);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("10: debugfs Module");
