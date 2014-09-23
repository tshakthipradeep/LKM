#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kthread.h>
#include <linux/delay.h>

struct task_struct *kthread;

int kthread_function(void *data)
{
	while(1)
	{
		pr_alert("In Kernel Thread!!!\n");	
		msleep(1000);

		//kthread_should_stop must be used to check if this thread should be stopped
		if(kthread_should_stop())
			return 0;
	}

	return 0;
}

int __init init_module(void)
{
	int ret = 0;

	pr_alert("Entering init_module\n");

#if 0
	//This will create a kernel thread but will not start the thread
	kthread = kthread_create(kthread_function,NULL,"kthread_module");

	if(IS_ERR(kthread))
	{
		ret = PTR_ERR(kthread);
		pr_alert("Error creating kernel thread = %d\n",ret);
	}
	else
	{
		//To start the kernel thread created, call wake_up_process
		//Comment this function and try inserting this module
		wake_up_process(kthread);
	}
#else
	//kthread_run is a wrapper function which internally calls kthread_create and wake_up_process
	kthread = kthread_run(kthread_function,NULL,"kthread_module");

        if(IS_ERR(kthread))
        {
                ret = PTR_ERR(kthread);
                pr_alert("Error creating kernel thread = %d\n",ret);
        }
#endif	
	return 0;

}

void __exit cleanup_module(void)
{
	int ret = 0;

	//To stop the kernel thread created, call kthread_stop
	//Calling this function when the kernel thread has already stopped, will generate a NULL pointer exception
	ret = kthread_stop(kthread);

	if(ret == -EINTR)
	{
		pr_alert("Function wake_up_process was not called\n");
	}
	else
	{
		pr_alert("%d was returned by the kernel thread\n",ret);
	}

	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("08: Kernel Thread Module");
