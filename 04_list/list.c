#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>

#include <linux/fs.h>

struct linux_list
{
	struct list_head node;
	int value;
};

int count = 0;

dev_t device_num;

struct list_head head;

int __init init_module(void)
{
	int index = 0;
	struct linux_list *node = NULL;
	struct list_head *node_ptr = NULL;

	pr_alert("Entering init_module\n");

	INIT_LIST_HEAD(&head);

	for(index=0;index<20;index++)
	{
		node = (struct linux_list *)vmalloc(sizeof(struct linux_list));
		node->value = index;
		list_add_tail(&node->node,&head);
	}

        pr_alert("Using list_for_each method:\n");

        list_for_each(node_ptr,&head)
        {
                node = list_entry(node_ptr,struct linux_list,node);
                pr_alert("node->value = %d\n",node->value);
        }

	pr_alert("Using list_for_each_entry method:\n");

        list_for_each_entry(node,&head,node)
        {
                pr_alert("node->value = %d\n",node->value);
        }

	pr_alert("list_empty: %d\n",list_empty(&head));

	alloc_chrdev_region(&device_num,0,1,"list");

	return 0;
}

void __exit cleanup_module(void)
{
	struct list_head *node_ptr = NULL;
	struct list_head *node_next = NULL;
	struct linux_list *node = NULL;
	
        list_for_each_safe(node_ptr,node_next,&head)
        {
                node = list_entry(node_ptr,struct linux_list,node);
                pr_alert("node->value = %d\n",node->value);
                list_del(node_ptr);
                vfree(node);
        }

        pr_alert("list_empty: %d\n",list_empty(&head));

	unregister_chrdev_region(device_num,1);

	pr_alert("Entering cleanup_module\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("T Shakthi Pradeep");
MODULE_DESCRIPTION("05: List Module");
