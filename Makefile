CC=gcc
CFLAGS=-Wall

main: main.o rfc.o
	$(CC) main.o rfc.o -o main.out