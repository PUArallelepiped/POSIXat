obj-m:= color.o

kernel_DIR:= /usr/src/linux-headers-5.4.0-148-generic/

PWD := $(shell pwd)

all:
	make -C $(kernel_DIR) M=$(PWD)
claen:
	rm *.o *.ko *.mod.c
.PHONY:
	clean