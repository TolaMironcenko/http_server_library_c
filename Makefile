CC = gcc
CFLAGS = -Wall -o
CFILES = main.c lib/http/http.c lib/net/net.c lib/hashtab/hashtab.c lib/tree/tree.c lib/type/type.c
OUTFILES = main

all:
	make build
	make run

build:
	$(CC) $(CFLAGS) $(OUTFILES) $(CFILES)

run:
	./$(OUTFILES)