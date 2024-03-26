CC = gcc
CFLAGS = -Wall -Werror
CFILES = src/main.c src/lib/http/http.c src/lib/net/net.c src/lib/hashtab/hashtab.c src/lib/tree/tree.c src/lib/type/type.c
OUTFILES = main

default: all

all: clean build

build:
	@echo "compiling..."
	@$(CC) $(CFLAGS) -o bin/$(OUTFILES) $(CFILES)
	@echo "compile successful."

run:
	@echo "running..."
	@./bin/$(OUTFILES)

clean:
	@echo "cleaning..."
	@rm -rf $(OUTFILES)
	@echo "clean successful."