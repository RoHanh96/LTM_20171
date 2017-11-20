CC=gcc
CFLAGS=-I.
OBJ2 = main.o
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
all:	 main
main: $(OBJ2)
	gcc -o $@ $^ $(CFLAGS) 

clean:
	rm *.o
