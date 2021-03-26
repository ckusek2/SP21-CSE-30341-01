CC= gcc
CFLAGS= -g -Wall -std=gnu99 -Iinclude
TARGETS= ndshell

all: $(TARGETS)

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o

.PHONY: all test clean

# Rules for proj3

proj3: proj3.o proj3.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) -o $@ $<
