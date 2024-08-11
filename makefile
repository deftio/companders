# makefile for compander tests (simple integer compander lib for embedded systems)
# @author M A Chatterjee <deftio [at] deftio [dot] com>

CC=gcc
CFLAGS=-I. -Wall 
CFLAGS_TEST=-I. -Wall -ftest-coverage -fprofile-arcs
DEPS = companders.h
OBJ = companders.o compandit.o
TEST_OBJ = companders.o companders_fulltest.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS_TEST)

compandit: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS_TEST) -lm

companders_fulltest: $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS_TEST) -lm 


test: companders_fulltest
	./companders_fulltest

clean:
	rm  *.o  *.asm  *.lst *.sym *.rel *.s *.gc* -f *.info
