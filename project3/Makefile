CC= gcc
CFLAGS= -g -Wall -std=gnu99 -Iinclude
TARGETS= threadedRE

all: $(TARGETS)

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o

.PHONY: all test clean

# Rules for threadedRE
threadedRE: threadedRE.o threadedRE.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) -o $@ $< -pthread
