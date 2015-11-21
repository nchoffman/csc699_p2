#makefile for project 2
EXE = cpan specCompress
CC = gcc
LFLAGS = -lm

all: $(EXE)

cpan: cpan.c header.c anread.c
	$(CC) -O $? -o $@ $(LFLAGS)

specCompress: specCompress.c header.c anread.c getMaxError.c clean.c sortBP.c printBP.c interpolate.c
	$(CC) -O -g $? -o $@ $(LFLAGS)

clean:
	rm -f $(EXE)
