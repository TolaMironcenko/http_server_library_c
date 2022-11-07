CC = gcc
CFLAGS = -Wall -Werror
CFILES = main.c lib/http/http.c lib/net/net.c lib/hashtab/hashtab.c lib/tree/tree.c lib/type/type.c
OUTFILES = main

all:
	@echo ">>>>>>>>>>>>>>>>>>>>>>>>>>"
	@make clean
	@make build
	@make run
	@echo "<<<<<<<<<<<<<<<<<<<<<<<<<<<<"

build:
	@echo "compiling..."
	@$(CC) $(CFLAGS) -o $(OUTFILES) $(CFILES)
	@echo "compile successful."

run:
	@echo "running..."
	@./$(OUTFILES)

clean:
	@echo "cleaning..."
	@rm -rf $(OUTFILES)
	@echo "clean successful."