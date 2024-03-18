#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/types.h>

struct color
{
    char data[100];
    struct list_head list;
};

int my_init(void)
{
    static LIST_HEAD(color_list);
    struct color *Color;
    struct color *ptr;
    // Invoke the dmesg command to ensure that the list is properly constructed once the kernel module has been loaded.
    printk(KERN_INFO "Loading Module\n");

    // In the module entry point, create a linked list containing four struct color elements.

    Color = kmalloc(sizeof(*Color), GFP_KERNEL);
    strcpy(Color->data, "red");
    list_add_tail(&Color->list, &color_list);

    Color = kmalloc(sizeof(*Color), GFP_KERNEL);
    strcpy(Color->data, "blue");
    list_add_tail(&Color->list, &color_list);

    Color = kmalloc(sizeof(*Color), GFP_KERNEL);
    strcpy(Color->data, "yellow");
    list_add_tail(&Color->list, &color_list);

    Color = kmalloc(sizeof(*Color), GFP_KERNEL);
    strcpy(Color->data, "green");
    list_add_tail(&Color->list, &color_list);

    // Traverse the linked list and output its contents to the kernel log buffer.

    list_for_each_entry(ptr, &color_list, list)
    {

        printk(KERN_INFO "data:%s\n", ptr->data);
    }
    return 0;
}

void my_exit(void)
{
    static LIST_HEAD(color_list);
    struct color *ptr = NULL, *next = NULL;
    // Again, invoke the dmesg command to check that the list has been removed once the kernel module has been unloaded.
    printk(KERN_INFO "Removing Module\n");

    // In the module exit point, delete the elements from the linked list and return the free memory back to the kernel.

    list_for_each_entry_safe(ptr, next, &color_list, list)
    {

        list_del(&ptr->list);
        kfree(ptr);
    }

    if (list_empty(&color_list))
    {
        printk("color_list is empty\n");
    }
    else
    {
        printk("no");
    }
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
