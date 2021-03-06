# -*- MakeFile -*-

CC = cc
CFLAGS = -lm

build: main.o pgm_lib.o
	$(CC) main.o pgm_lib.o -o main $(CFLAGS)

pgm_lib.o: pgm_lib.c pgm_lib.h
	$(CC) -c pgm_lib.c

main.o: main.c
	$(CC) -c main.c

clean:
	rm -f *.o main