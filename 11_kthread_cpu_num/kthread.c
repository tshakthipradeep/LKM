#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

struct task_struct **kthread;
int *nkthread;

int kthread_function(void *data)
{
	int *ikthread = (int *)data;

	while(1)
	{
		pr_alert("In Kernel Thread!!!\n");	
		pr_alert("Kernel Thread = %d CPU = %d!!!\n",*ikthread,smp_processor_id());	
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
	int ncpu = num_online_cpus();
	int i = 0;

	pr_alert("Entering init_module\n");
	pr_alert("Detected %d CPUs\n",ncpu);

	kthread = kzalloc(sizeof(struct task_struct *) * ncpu,GFP_KERNEL);
	nkthread = kzalloc(sizeof(int *) * ncpu,GFP_KERNEL);

	//kthread_run is a wrapper function which internally calls kthread_create and wake_up_process

	for(i=0;i<ncpu;i++)
	{
		nkthread[i] = i;
		kthread[i] = kthread_run(kthread_function,&nkthread[i],"kthread_module");

	        if(IS_ERR(kthread[i]))
        	{
                	ret = PTR_ERR(kthread[i]);
	                pr_alert("Error creating kernel thread = %d\n",ret);
        	}
	}

	return 0;

}

void __exit cleanup_module(void)
{
	int ret = 0;
	int ncpu = num_online_cpus();
        int i = 0;

	//To stop the kernel thread created, call kthread_stop
	//Calling this function when the kernel thread has already stopped, will generate a NULL pointer exception

	for(i=0;i<ncpu;i++)
	{
		ret = kthread_stop(kthread[i]);

		if(ret == -EINTR)
		{
			pr_alert("Function wake_up_process was not called\n");
		}
		else
		{
			pr_alert("%d was returned by the kernel thread\n",ret);
		}
	}

	kfree(nkthread);
	kfree(kthread);

	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("11: Kernel Thread CPU Module");
