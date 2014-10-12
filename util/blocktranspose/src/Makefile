# blocktranspose Makefile

CC=clang
CFLAGS= -Wall -Wextra -Werror -pedantic -O2 -pipe -march=native

.PHONY: all
all: blocktranspose

blocktranspose: main.c
	$(CC) -o blocktranspose $(CFLAGS) main.c

.PHONY: clean
clean:
	rm -f blocktranspose
