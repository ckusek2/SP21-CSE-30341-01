CC=		gcc
CFLAGS=		-g -Wall -std=gnu99 -Iinclude
TARGETS=	textdetect

all:		$(TARGETS)

clean:
	@echo Cleaning...
	@rm -f $(TARGETS) *.o

.PHONY:		all test clean

# Rules for textdetect

textdetect: textdetect.o textdetect.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) -o $@ $<
