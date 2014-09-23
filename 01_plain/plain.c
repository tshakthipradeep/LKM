#include <linux/module.h>
#include <linux/kernel.h>

int __init init_module(void)
{
	pr_alert("Entering init_module\n");

	return 0;
}

void __exit cleanup_module(void)
{
	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("01: Plain Module");
