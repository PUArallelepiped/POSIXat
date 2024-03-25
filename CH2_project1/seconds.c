#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/param.h>

#define BUFFER_SIZE 128
#define PROC_NAME "seconds"

static unsigned long start, end;

ssize_t proc_read(struct file *file, char __user *usr_buf,size_t count, loff_t *pos);

static struct proc_ops my_proc ={
    .proc_read = proc_read
};

int proc_init(void){
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    proc_create(PROC_NAME, 0666, NULL, &my_proc);
    start = jiffies;
    return 0;
}

void proc_exit(void){
    printk(KERN_INFO "Removing /proc/%s\n", PROC_NAME);
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char  __user *usr_buf,size_t count, loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed)
    {
        completed = 0;
        return 0;
    }

    printk(KERN_INFO "ahhhh you read me!!!\n");

    completed = 1;
    end = jiffies;
    rv = sprintf(buffer, "Time elapsed: %lu seconds\n", (end - start) / HZ);
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SECONDS MODULE");
