#include <linux/module.h>
#include <linux/kernel.h>

static char *string = "Hello World !!!";
module_param(string,charp,S_IRWXU);

static int integer = 100;
module_param(integer,int,S_IRWXU);

int __init init_module(void)
{
	pr_alert("Entering init_module\n");

	pr_alert("Param 1: %s\n",string);
	pr_alert("Param 2: %d\n",integer);

	return 0;
}

void __exit cleanup_module(void)
{
	pr_alert("Entering cleanup_module\n");

	pr_alert("Param 1: %s\n",string);
	pr_alert("Param 2: %d\n",integer);
}

MODULE_PARM_DESC(string , " This paramter is used to hold string");
MODULE_PARM_DESC(integer , " This paramter is used to hold integer");

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("02: Param Plain Module");
