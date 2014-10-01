#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/interrupt.h>

static char my_tasklet_data[] = "This is my tasklet data";

void my_tasklet_function(unsigned long data)
{
	pr_alert("%s: %s\n",__func__,(char *)data);
}

DECLARE_TASKLET(my_tasklet,my_tasklet_function,(unsigned long)&my_tasklet_data);

int __init init_module(void)
{
	pr_alert("Entering init_module\n");
	tasklet_schedule(&my_tasklet);	
	return 0;
}

void __exit cleanup_module(void)
{
	pr_alert("Entering cleanup_module\n");
	tasklet_kill(&my_tasklet);	
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("13: Tasklet Module");
