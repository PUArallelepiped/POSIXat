## CH3 Project2

---

In this project, you will write a kernel module that uses /proc file system for displaying a task’s information in a Linux system.

Be sure to review the programming project in Chap.2 before you begin this project

It can be completed using the Linux virtual machine provided with the textbook

Part I – Writing to the /proc file system
Part II – Reading from the /proc file system

#### install
```sh
make MODULE_NAME=task_info
sudo insmod task_info.ko
```

#### exec
```sh
# write pid into file
echo 1 > /proc/task_info
# see the task info
cat /proc/task_info
```

#### uninstall
```sh
sudo rmmod task_info
make clean
```

#### screenshot

![jiffies](screenshot/task_info.png)