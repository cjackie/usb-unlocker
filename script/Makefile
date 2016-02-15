CC = gcc
CFLAG = -Werror -Wall

BLOWFISH_OBJS = blowfish/bf_cfb64.o blowfish/bf_enc.o blowfish/bf_skey.o 

.PHONY: blowfish clean


usb_unlocker_helper: usb_unlocker_helper.o blowfish
	$(CC) $(CFLAG) usb_unlocker_helper.o $(BLOWFISH_OBJS) -o usb_unlocker_helper

usb_unlocker_helper.o: usb_unlocker_helper.c 
	$(CC) $(CFLAG) -c usb_unlocker_helper.c -o usb_unlocker_helper.o

blowfish:
	cd blowfish && $(MAKE)

clean:
	cd blowfish && $(MAKE) clean
	rm *.o




