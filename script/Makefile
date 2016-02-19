CC = gcc
CFLAG = -Werror -Wall

BLOWFISH_OBJS := blowfish/bf_cfb64.o blowfish/bf_enc.o blowfish/bf_skey.o 
CJ_LIB_OBJS := cj_lib/cj_array.o

.PHONY: blowfish clean cj_lib

usb_unlocker_helper: usb_unlocker_helper.o blowfish cj_lib
	$(CC) $(CFLAG) usb_unlocker_helper.o $(BLOWFISH_OBJS) $(CJ_LIB_OBJS) -o usb_unlocker_helper

usb_unlocker_helper.o: usb_unlocker_helper.c 
	$(CC) $(CFLAG) -c usb_unlocker_helper.c -o usb_unlocker_helper.o

blowfish:
	cd blowfish && $(MAKE)

cj_lib:
	cd cj_lib && $(MAKE)

clean:
	cd blowfish && $(MAKE) clean
	cd cj_lib && $(MAKE) clean
	rm *.o *.~
