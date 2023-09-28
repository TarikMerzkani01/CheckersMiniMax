# Checkers Tarik Makefile
AN=$(shell head -1 00_ID.txt)
CLASS=$(shell tail -1 00_ID.txt)

CFLAGS = -Wall -Werror -g -Og
# CFLAGS = -Wall -g -O3
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	checkers\

all : $(PROGRAMS)

clean :
	rm -f $(PROGRAMS) *.o vgcore.* 

help :
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'

# build .o files from corresponding .c files
%.o : %.c
	$(CC) -c $<

# checkers building
checkers : checkersmain.o checkfuncs.o
	$(CC) -o checkers checkersmain.c checkfuncs.c

# Testing Targets
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

