# make file for compandit tests (simple integer compander lib for embedded systems)
# @author M A Chatterjee <deftio [at] deftio [dot] com>

CC=gcc
CFLAGS=-I. -Wall
DEPS = companders.h
OBJ = companders.o compandit.o  

                  
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

compandit: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) -lm

clean :
	rm  *.o

