#!/bin/bash
CONFIG_MODULE_SIG:=n
MODULE_NAME := project3-1
ccflags-y := -Wall
# kernel_DIR := /usr/src/linux-headers-4.9.201-tegra-ubuntu18.04_aarch64/kernel-4.9/
kernel_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m := $(MODULE_NAME).o
all: 
	make -C $(kernel_DIR) M=$(PWD) modules
	
clean: 
	rm *.o *.ko *.mod.c *.order .*cmd *.symvers *.mod
install:
	# sudo mknod /dev/$(MODULE_NAME) c 61 0
	# ls -al /dev/$(MODULE_NAME)
	sudo insmod $(MODULE_NAME).ko
run:
	sudo echo 1 > /dev/$(MODULE_NAME)
uninstall:
	# sudo rm /dev/$(MODULE_NAME)
	sudo rmmod $(MODULE_NAME).ko

.PHONY: 
	clean
