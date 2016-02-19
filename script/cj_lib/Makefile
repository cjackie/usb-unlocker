CC = gcc
CFLAG = -Werror -Wall

OBJS := cj_array.o

# condition for running tests TODO
TEST := 1

.PHONY: cj_lib clean

cj_lib: $(OBJS)

cj_array.o: cj_array.c cj_array.h
	$(CC) $(CFLAG) -c cj_array.c -o cj_array.o

clean:
	rm *.o *~
