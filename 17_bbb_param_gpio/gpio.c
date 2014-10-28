#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/gpio.h>

#define GPIO_66	66

static int gpio_num = GPIO_66;
module_param(gpio_num,int,0);

static char *gpio_desc = "GPIO_66";
module_param(gpio_desc,charp,0);

int __init init_module(void)
{
	int ret = 0;

	pr_alert("Entering init_module\n");

	ret = gpio_request_one(gpio_num,GPIOF_OUT_INIT_LOW,gpio_desc);

	if(ret)
		pr_err("gpio_request_one on GPIO 66 failed\n");

	return 0;
}

void __exit cleanup_module(void)
{
	gpio_free(gpio_num);
	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("17: Param GPIO Module");
