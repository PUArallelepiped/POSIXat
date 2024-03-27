#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUM 61
#define MODULE_NAME "project3-1"

#define CHRDEVBASE_MAJOR 200
#define CHRDEBBASE_NAME "chrdevbase"

static int demo_init(void)
{
    printk(KERN_INFO "%s: started\n", MODULE_NAME);

    struct task_struct *task;
    for_each_process(task)
    {
        printk(KERN_INFO "pid: %d | pname: %s | state: %ld \n", task->pid, task->comm, task->stats);
    }
    return 0;
}
static void demo_exit(void)
{
    printk("%s: removed\n", MODULE_NAME);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List all tasks");
MODULE_AUTHOR("someone");
