# makefile for compandit tests (simple integer compander lib for embedded systems)
# @author M A Chatterjee <deftio [at] deftio [dot] com>

CC=gcc
CFLAGS=-I. -Wall
DEPS = companders.h
OBJ = companders.o compandit.o
TEST_OBJ = companders.o companders_fulltest.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compandit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm

companders_fulltest: $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -lm

test: companders_fulltest
	./companders_fulltest

clean:
	rm *.o compandit companders_fulltest -f
