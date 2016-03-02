CC:=gcc
PWD:=$(shell pwd)
KP:=/lib/modules/$(shell uname -r)/build

# usb device indentifier
KEY =idVendor=1256 idProduct=26720 bcdDevice=1024 iProduct=2 iSerialNumber=3

obj-m := usb_unlocker.o


build:
	make -C $(KP) M=$(PWD) modules

install:
	sudo insmod ./usb_unlocker.ko $(KEY)

uninstall:
	sudo rmmod usb_unlocker

clean:
	make -C $(KP) M=$(PWD) clean
