#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <asm/param.h>

#define BUFFER_SIZE 128
#define PROC_NAME "task_info"

static int current_pid;

ssize_t proc_read(struct file *file, char __user *usr_buf,size_t count, loff_t *pos);
ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos);

static struct proc_ops my_proc ={
    .proc_read = proc_read,
    .proc_write = proc_write
};

int proc_init(void){
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    proc_create(PROC_NAME, 0666, NULL, &my_proc);
    return 0;
}

void proc_exit(void){
    printk(KERN_INFO "Removing /proc/%s\n", PROC_NAME);
    remove_proc_entry(PROC_NAME, NULL);
}

ssize_t proc_read(struct file *file, char  __user *usr_buf,size_t count, loff_t *pos)
{
    static int completed = 0;
    char buffer[BUFFER_SIZE];
    struct task_struct *task;
    struct task_struct *task_child;
    struct list_head *list;
    int rv = 0;
    if (completed)
    {
        completed = 0;
        return 0;
    }
    task = pid_task(find_vpid(current_pid), PIDTYPE_PID);
    if (task == NULL){
        printk(KERN_INFO "task is NULL\n");
        return 0;
    }
    rv = sprintf(buffer, "command = [%s], pid = [%d], state = [%c]\n", task->comm, task->pid, task_state_to_char(task));
    completed = 1;
    copy_to_user(usr_buf, buffer, rv);
    return rv;
}

ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
    char *k_mem;
    // allocate kernel memory
    k_mem = kmalloc(count, GFP_KERNEL);
    if (copy_from_user(k_mem, usr_buf, count)){
        printk(KERN_INFO "Failed to copy data from the user\n");
        return -EFAULT;
    }
    k_mem[count] = 0;
    kstrtoint(k_mem, 10, &current_pid);
    printk(KERN_INFO "pid is %d\n", current_pid); 
    kfree(k_mem);
    return count;
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TASK INFO MODULE");