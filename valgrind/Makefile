CC     = gcc
#CFLAGS = -O3 -std=c11 -Wall -Werror -DNDEBUG
CFLAGS = -O3 -std=c11 -Wall -Werror -DNDEBUG

all: valgrind_test0 valgrind_test1 valgrind_test2 valgrind_test3

valgrind_test0: valgrind_test0.c
	$(CC) $(CFLAGS) -o valgrind_test0.out valgrind_test0.c

valgrind_test1: valgrind_test1.c
	$(CC) $(CFLAGS) -o valgrind_test1.out valgrind_test1.c

valgrind_test2: valgrind_test2.c
	$(CC) $(CFLAGS) -o valgrind_test2.out valgrind_test2.c

valgrind_test3: valgrind_test3.c
	$(CC) $(CFLAGS) -o valgrind_test3.out valgrind_test3.c
