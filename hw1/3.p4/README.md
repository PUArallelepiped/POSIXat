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
sudo insmod color.ko //loads the "color.ko" kernel module into the Linux kernel
sudo dmesg
```

![alt text](screenshot/image-2.png)

```
sudo rmmod color.ko //remove kernel module
sudo dmesg
```

![alt text](screenshot/image-3.png)

#### Part II – Parameter Passing

###### Assignment :

-Design a kernel module named collatz that is passed an initial value as a module parameter.

-Your module will then generate and store the sequence in a kernel linked list when the module is loaded.

-Once the sequence has been stored, your module will traverse the list and output its contents to the kernel log buffer.

-Use the dmesg command to ensure that the sequence is properly generated once the module has been loaded.

-In the module exit point, delete the contents of the list and return the free memory back to the kernel.

-Again, use dmesg to check that the list has been removed once the kernel module has been unloaded.

```
sudo insmod collatz.ko //loads the "collatz.ko" kernel module into the Linux kernel , default start=100
sudo insmod collatz.ko start=11 //loads the "collatz.ko" kernel module into the Linux kernel , set start=11
sudo dmesg
```

![alt text](screenshot/image.png)

```
sudo rmmod collatz.ko //remove kernel module
sudo dmesg
```

![alt text](screenshot/image-1.png)
