# Alejandro Pacheco
# Red ID: 827078376
# Edoras account: cssc0803@edoras.sdsu.edu
# CS 530, Spring 2025
# Assignment #2, SIC/XE Disassembler
# Makefile


# Specify compiler
CC = g++
# Compiler flags, if you want debug info, add -g
CCFLAGS = -std=c++11 -g3 -Wall -c
CFLAGS = -g3 -c

OBJS = main.o

PROGRAM = essy

$(PROGRAM) : $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

main.o : main.cpp main.h
	$(CC) $(CCFLAGS) -o main.o main.cpp

pass1.o : pass1.cpp pass1.h
	$(CC) $(CCFLAGS) -o pass1.o pass1.cpp

clean :
	rm -rf $(OBJS) *~ $(PROGRAM)