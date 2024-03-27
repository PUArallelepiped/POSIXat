obj-m:= collatz.o

# kernel_DIR:= /usr/src/linux-headers-5.4.0-148-generic/
kernel_DIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

all:
	make -C $(kernel_DIR) M=$(PWD)
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
.PHONY:
	clean