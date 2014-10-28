#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/gpio.h>

#define GPIO_66	66

int __init init_module(void)
{
	int ret = 0;

	pr_alert("Entering init_module\n");

	ret = gpio_request_one(GPIO_66,GPIOF_OUT_INIT_LOW,"GPIO 66");

	if(ret)
		pr_err("gpio_request_one on GPIO 66 failed\n");

	return 0;
}

void __exit cleanup_module(void)
{
	gpio_free(GPIO_66);
	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("16: Simple GPIO Module");
