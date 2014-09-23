#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/semaphore.h>

struct task_struct *kthread_1;
struct task_struct *kthread_2;

struct semaphore lock1;
struct semaphore lock2;

int kthread_1_function(void *data)
{
	while(1)
	{

		/**
		*
		* If sleep function is moved out of semaphore down and up, alternate prints are
		* not seen. But if sleep function is between down and up, it works fine.
		*
		* If delay function is moved out of semaphore down and up, alternate prints are
		* not seen and on removal of the module, entire system hangs. But if delay function 
		* is between down and up it works fine.
		*
		**/

		down(&lock1);
		pr_alert("In Kernel Thread 1!!!\n");	
		up(&lock2);
		
		msleep(100);
		//kthread_should_stop must be used to check if this thread should be stopped
		if(kthread_should_stop())
		{
			up(&lock2);
			return 1;
		}
	}

	return 0;
}

int kthread_2_function(void *data)
{
        while(1)
        {
		down(&lock2);
                pr_alert("In Kernel Thread 2!!!\n");
		up(&lock1);

		msleep(1000);
                //kthread_should_stop must be used to check if this thread should be stopped
                if(kthread_should_stop())
		{
			up(&lock1);
                        return 2;
		}
        }

        return 0;
}

/*

This module creates two kernel threads both with unique prints. Both kernel 
threads print message every one second. We will use a spinlock to sync
these two threads to print the message alternatively. To check the difference
between with spinlock and without spinlock, comment the USE_SPINLOCK macro
above.

*/

int __init init_module(void)
{
	int ret = 0;

	pr_alert("Entering init_module\n");

	//Initialize spinlock and set lock state to locked
	sema_init(&lock1,1);
	sema_init(&lock2,0);
	//spin_lock(&lock);

	kthread_1 = kthread_run(kthread_1_function,NULL,"kthread_1_module");

        if(IS_ERR(kthread_1))
        {
                ret = PTR_ERR(kthread_1);
                pr_alert("Error creating kernel thread = %d\n",ret);
        }
	
        kthread_2 = kthread_run(kthread_2_function,NULL,"kthread_2_module");

        if(IS_ERR(kthread_2))
        {
                ret = PTR_ERR(kthread_2);
                pr_alert("Error creating kernel thread = %d\n",ret);
        }

	return 0;

}

void __exit cleanup_module(void)
{
	int ret = 0;

	//To stop the kernel thread created, call kthread_stop
	//Calling this function when the kernel thread has already stopped, will generate a NULL pointer exception

	/**
	*
	* Sets kthread_should_stop() for @k to return true, wakes it, and
	* waits for it to exit. This can also be called after kthread_create()
	* instead of calling wake_up_process(): the thread will exit without
	* calling threadfn().
	*
	* If threadfn() may call do_exit() itself, the caller must ensure
	* task_struct can't go away.
	*
	* Returns the result of threadfn(), or %-EINTR if wake_up_process()
	* was never called.
	*
	**/

	ret = kthread_stop(kthread_1);

	if(ret == -EINTR)
	{
		pr_alert("Function wake_up_process was not called\n");
	}
	else
	{
		pr_alert("%d was returned by the kernel thread 1\n",ret);
	}

        ret = kthread_stop(kthread_2);

        if(ret == -EINTR)
        {
                pr_alert("Function wake_up_process was not called\n");
        }
        else
        {
                pr_alert("%d was returned by the kernel thread 2\n",ret);
        }

	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("08: Kernel Thread Module");
