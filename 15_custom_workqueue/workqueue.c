#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/interrupt.h>
#include <linux/delay.h>

void my_work_function(struct work_struct *work);
struct workqueue_struct *my_workqueue;

DECLARE_WORK(my_work,my_work_function);

void my_work_function(struct work_struct *work)
{
	pr_alert("%s: %d\n",__func__,smp_processor_id());
	msleep(1000);
	
	/*
	* Comment the below function if you want to run this work only once. If left
	* uncommented this work will run every one second.
	*/

	schedule_work(&my_work);
}

int __init init_module(void)
{
	pr_alert("Entering init_module\n");
	
	/*
	* Other function that you can try is create_singlethread_workqueue.
	*/

	//my_workqueue = create_singlethread_workqueue("my_custom_workqueue");
	my_workqueue = create_workqueue("my_custom_workqueue");

	if(IS_ERR(my_workqueue))
	{
		pr_alert("Could not create workqueue\n");
		return PTR_ERR(my_workqueue);
	}

	queue_work(my_workqueue,&my_work);

        /*
        * Other function that you can try is schedule_work_on to schedule the work
	* on a specific CPU.
        */

	schedule_work(&my_work);	
	return 0;
}

void __exit cleanup_module(void)
{
	pr_alert("Entering cleanup_module\n");
	flush_work(&my_work);	
	cancel_work_sync(&my_work);
	destroy_workqueue(my_workqueue);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("15: Custom Workqueue Module");
