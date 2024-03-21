obj-m:= color.o

# kernel_DIR:= /usr/src/linux-headers-5.4.0-148-generic/
kernel_DIR := /lib/modules/$(shell uname -r)/build

PWD := $(shell pwd)

all:
	make -C $(kernel_DIR) M=$(PWD)
clean:
	rm *.o *.ko *.mod *.mod.c *.order *.symvers
.PHONY:
	clean