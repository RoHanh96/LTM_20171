CC=gcc
CFLAGS=-I.
OBJ2 = main.o
%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)
all:	 main
main: $(OBJ2)
	gcc -o $@ $^ $(CFLAGS) `mysql_config --cflags --libs`
clean:
	rm *.o
