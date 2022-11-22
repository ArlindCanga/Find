CC = gcc
CFLAGS = -c -g -ggdb
LDFLAGS= -g -ggdb

find: main.o logs.o find.o KMP.o
	$(CC) $(LDFLAGS) -o find main.o logs.o find.o KMP.o
main.o : main.c find.h
	$(CC) $(CFLAGS) -c main.c 
logs.o : logs.c logs.h find.h
	$(CC) $(CFLAGS) logs.c -o logs.o
find.o : find.c find.h KMP.h
	$(CC) $(CFLAGS) find.c -o find.o
KMP.o : KMP.c KMP.h find.h logs.h
	$(CC) $(CFLAGS) KMP.c -o KMP.o