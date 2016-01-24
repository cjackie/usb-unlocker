CC := gcc
PWD := $(shell pwd)
KP := /lib/modules/$(shell uname -r)/build

obj-m := usb_unlocker.o

build:
	make -C $(KP) M=$(PWD) modules

clean:
	make -C $(KP) M=$(PWD) clean
