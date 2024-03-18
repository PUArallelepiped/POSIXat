# Project 4: Kernel Data Structures

#### Part I – Inserting, Deleting Elements to/from the linked lists

###### Assignment :

–In the module entry point, create a linked list containing four struct
color elements.

–Traverse the linked list and output its contents to the kernel log buffer.

–Invoke the dmesg command to ensure that the list is properly constructed once the kernel module has been loaded.

–In the module exit point, delete the elements from the linked list and return the free memory back to the kernel.

–Again, invoke the dmesg command to check that the list has been removed once the kernel module has been unloaded.

```
sudo insmod hellod.ko //loads the "hellod.ko" kernel module into the Linux kernel
sudo rmmod hellod.ko //remove kernel module
dmesg //displays the kernel ring buffer
```
