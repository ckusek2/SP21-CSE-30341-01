CC=		gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude
TARGETS=	ndshell

all:		$(TARGETS)

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o

.PHONY:		all test clean

# Rules for ndshell

ndshell: ndshell.o ndshell.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) -o $@ $<
