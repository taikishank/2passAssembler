# Alejandro Pacheco, Taiki Shank
# cssc0803, cssc5097
# CS530, Spring 2025
# Assignment #2, Two-Pass Assembler
# Makefile

# Specify compiler
CC = g++
# Compiler flags, if you want debug info, add -g
CCFLAGS = -std=c++11 -g3 -Wall -c
CFLAGS = -g3 -c

OBJS = main.o pass1.o instruction.o labelMap.o pass2.o

PROGRAM = essy

$(PROGRAM) : $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

main.o : main.cpp main.h
	$(CC) $(CCFLAGS) -o main.o main.cpp

pass1.o : pass1.cpp pass1.h
	$(CC) $(CCFLAGS) -o pass1.o pass1.cpp

instruction.o : instruction.cpp instruction.h
	$(CC) $(CCFLAGS) -o instruction.o instruction.cpp

labelMap.o : labelMap.cpp labelMap.h
	$(CC) $(CCFLAGS) -o labelMap.o labelMap.cpp

pass2.o : pass2.cpp pass2.h
	$(CC) $(CCFLAGS) -o pass2.o pass2.cpp

clean :
	rm -rf $(OBJS) *~ $(PROGRAM)