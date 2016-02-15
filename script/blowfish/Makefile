CC = gcc
CFLAG = -Wall -Werror

OBJS = bf_cfb64.o bf_enc.o bf_skey.o 
HDRS = bf_pi.h bf_locl.h blowfish.h

.PHONY:	clean

build: $(OBJS)

$(OBJS): %.o : %.c $(HDRS)
	$(CC) $(CFLAG) -c $< -o $@

clean: 
	rm *.o

