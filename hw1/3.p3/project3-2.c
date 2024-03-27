#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define MAJOR_NUM 61
#define MODULE_NAME "project3-2"

#define CHRDEVBASE_MAJOR 200
#define CHRDEBBASE_NAME "chrdevbase"

static void dfs_traverse(char *buff, struct task_struct *task)
{
    struct task_struct *child;
    printk(KERN_INFO "%s pid: %d | name: %s | state: %ld \n", buff, task->pid, task->comm, task->stats);
    // printk(KERN_INFO "pid: %d | name: %s | state: %ld \n", task->pid, task->comm, task->stats);

    list_for_each_entry(child, &task->children, sibling)
    {
        dfs_traverse(buff - 4, child);
    }
}

static int demo_init(void)
{
    printk(KERN_INFO "%s: started\n", MODULE_NAME);
    char buff[4 * 10 + 1] = "🤗🤗🤗🤗🤗🤗🤗🤗🤗🤗";

    struct task_struct *init_task_ptr = &init_task;
    dfs_traverse(buff + 4 * 9, init_task_ptr);
    return 0;
}
static void demo_exit(void)
{
    printk(KERN_INFO "%s: removed\n", MODULE_NAME);
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("List tasks using DFS");
MODULE_AUTHOR("someone");
