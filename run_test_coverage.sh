#!/bin/bash

#this shell script calls the code coverage testing program gcov (part of the gcc suite)
#you can run each command on your own at the command line

#fist clean all object files
make clean

#compile all the test program, link etc
make test

# run the example.out program ... with test coverage (see makefile for flags)
./companders_fulltest

# gcov is the gcc suite test coverage program.  We're interested in the coverage
# the companders.c file.  
gcov companders.c

# now the code coverage is in this file:
# companders.c.gcov  
# which can be viewed in any text editor 
