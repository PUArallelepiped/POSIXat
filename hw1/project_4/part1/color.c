#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/types.h>

struct color
{
    int red;
    int blue;
    int green;
    int white;
    struct list_head list;
};

static LIST_HEAD(color_list);

int my_init(void)
{
    struct color *violet;
    struct color *ptr;
    // Invoke the dmesg command to ensure that the list is properly constructed once the kernel module has been loaded.
    printk("Color kernel module init\n");

    // In the module entry point, create a linked list containing four struct color elements.

    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 138;
    violet->blue = 43;
    violet->green = 226;
    violet->white = 100;
    list_add_tail(&violet->list, &color_list);

    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 120;
    violet->blue = 20;
    violet->green = 216;
    violet->white = 80;
    list_add_tail(&violet->list, &color_list);

    violet = kmalloc(sizeof(*violet), GFP_KERNEL);
    violet->red = 10;
    violet->blue = 10;
    violet->green = 26;
    violet->white = 90;
    list_add_tail(&violet->list, &color_list);

    // Traverse the linked list and output its contents to the kernel log buffer.

    printk("color link list : \n");

    list_for_each_entry(ptr, &color_list, list)
    {

        printk(KERN_CONT "{red:%d blue:%d green:%d white:%d }-> ", ptr->red, ptr->blue, ptr->green, ptr->white);
    }

    printk(KERN_CONT "NULL\n");

    return 0;
}

void my_exit(void)
{
    struct color *ptr = NULL, *next = NULL;

    // In the module exit point, delete the elements from the linked list and return the free memory back to the kernel.
    printk("Free color_list and Check if the linked list is empty or not\n");

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
        printk("color_list isn't empty\n");
    }

    // Again, invoke the dmesg command to check that the list has been removed once the kernel module has been unloaded.
    printk("Color kernel module exit\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_LICENSE("GPL");
