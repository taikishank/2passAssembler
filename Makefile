# Specify compiler
CC = g++
# Compiler flags, if you want debug info, add -g
CCFLAGS = -std=c++11 -g3 -Wall -c
CFLAGS = -g3 -c

OBJS = main.o pass1.o instruction.o labelMap.o

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

clean :
	rm -rf $(OBJS) *~ $(PROGRAM)