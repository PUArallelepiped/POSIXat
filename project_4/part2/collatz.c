#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/init.h>
#include <linux/types.h>

// Design a kernel module named collatz that is passed an initial value as a module parameter.

static int p = 100;
module_param(p, int, 0644);
MODULE_PARM_DESC(p, "test for module parameter");

struct Collatz
{
    int data;
    struct list_head list;
};

static LIST_HEAD(Collatz_list);

int collatz_count(int n)
{
    // Your module will then generate and store the sequence in a kernel linked list when the module is loaded.

    struct Collatz *Collatz;
    struct Collatz *ptr;

    Collatz = kmalloc(sizeof(*Collatz), GFP_KERNEL);
    Collatz->data = n;
    list_add_tail(&Collatz->list, &Collatz_list);

    while (n > 1)
    {
        if (n % 2 == 0)
        { // for even numbers
            n = n / 2;
            Collatz = kmalloc(sizeof(*Collatz), GFP_KERNEL);
            Collatz->data = n;
            list_add_tail(&Collatz->list, &Collatz_list);
        }
        else
        { // for odd numbers
            n = n * 3 + 1;
            Collatz = kmalloc(sizeof(*Collatz), GFP_KERNEL);
            Collatz->data = n;
            list_add_tail(&Collatz->list, &Collatz_list);
        }
    }

    // Once the sequence has been stored, your module will traverse the list and output its contents to the kernel log buffer.

    printk("collatz link list : \n");
    list_for_each_entry(ptr, &Collatz_list, list)
    {

        printk(KERN_CONT "%d->", ptr->data);
    }

    printk(KERN_CONT "NULL\n");
    // Use the dmesg command to ensure that the sequence is properly generated once the module has been loaded.

    return 0;
}

static int __init my_test_init(void)
{

    printk("collatz kernel module init\n");
    printk("module patameter = %d\n", p);

    collatz_count(p);

    return 0;
}

static void __exit my_test_exit(void)
{
    struct Collatz *ptr = NULL, *next = NULL;

    // In the module exit point, delete the contents of the list and return the free memory back to the kernel.
    printk("Free Collatz_list and Check if the linked list is empty or not\n");
    list_for_each_entry_safe(ptr, next, &Collatz_list, list)
    {

        list_del(&ptr->list);
        kfree(ptr);
    }

    if (list_empty(&Collatz_list))
    {
        printk("Collatz_list is empty\n");
    }
    else
    {
        printk("Collatz_list isn't empty");
    }

    printk("collatz kernel module exit\n");

    // Again, use dmesg to check that the list has been removed once the kernel module has been unloaded.
}

module_init(my_test_init);
module_exit(my_test_exit);
MODULE_LICENSE("GPL");