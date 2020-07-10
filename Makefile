CC     = gcc
#CFLAGS = -O3 -std=c11 -Wall -Werror -DNDEBUG
CFLAGS = -O3 -std=c11 -Wall -Werror -DNDEBUG -Wextra -Wpedantic

loesung: loesung.c misc.c
	$(CC) $(CFLAGS) -o loesung loesung.c misc.c
