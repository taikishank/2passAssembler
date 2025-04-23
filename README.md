# 2passAssembler
CS530 A2 Project

//Alejandro Pacheco, Taiki Shank
//cssc0803, cssc0830
//CS530, Spring 2025
//Assignment #2, Two-Pass Assembler
//README.md

README

File Manifest:
This repository has the following files
main.cpp, main.h, main.o
pass1.cpp, pass1.h, pass1.o
pass2.cpp, pass2.h, pass2.o
instruction.cpp, instruction.h, instruction.o
labelMap.cpp, labelMap.h, labelMap.o
Makefile


Compile and Operating Instructions:
Note, that the user must be using g++ compiler
TO COMPILE AND BUILD the program, the user should run the following commands in order:
make clean
make 
With these 2 commands, the essy execuable and the corresponding imports and files have successfully been loaded.

We have provided two files P3Sample.sic and P4Sample.sic

Having said that, we can call the following on edoras:
essy filename1 
OR
essy filename1 filename2(optional)

Should the user want to run this code locally on an IDE like VSCode, these would be the commands
./essy filename1 
OR
./essy filename1 filename2(optional)

Based on the commands you run with the respective files, you can expect a listing file (.l) and symbol table (.st) to be produced.
The listing file will contain information such as the addresses and object codes for each instruction while the symbol table will
store the specific addresses and other information that each literal and label will contain. 

Note that the inputted file name should be in .sic format
The program does not have additional instruction arguments the user should need to worry about.



Design Decisions:
Throughout this process, we followed a lot of the design decisions that we declared during the Software design document (SDD)

Our first decision was to create an instruction class which contianed the important information pertaining to every instruction. This includes the instruction's
address, label, instruction, operand, as well as the output string. The instruction class and its singular function made it much easier to access and store all the instructions and assembler directives that were provided in the input file.

With the instruction class implemented, the next design decision was to split the rest of the logic, functions, and objects into seperate files. The main seperate files were pass1, pass2, and labelMap, which contains the information for assembler directives, symbol tables, and literal tables. We would then use this information to access formats, opcodes, and addresses for certain instructions from the input file. Separating everything allowed for strong readability and debugging capabilities. 

As the 2 pass assembler dictates, pass 1 generates LITTAB, SYMTAB, stores data into our variable label map. We would use this information for pass 2 which would generate the object code and handle the generation of the displacement for each instruction. Once everything was completed with that file in pass 2, we would clear all the previous information and begin to process the next file (if present).

The main difference in our implementation from our SDD was the means of implementation. We found that pair programming was much more effective in producing the logic and writing code. It also allowed us to bounce ideas off each other to make sure we each understood different components of the code. We realized pair programming was very effective for the both of us, and allowed us to work through ideas quickly and for efficient debugging. 



Extra Features:
We did not decide to add any extra features to this program apart from some error handling. It does not account for every single inconsistency in an input file or input file name, however, it accounts for invalid labels, filenames, and opcodes that do not exist. We thought that this was a good way to test faulty .sic files and deepen our understanding of the 2-pass assembler. 



Known Issues:
One thing we noticed was that the JSUB routine opcode off by 1 for the address. We weren't too sure why this was happening, but we did notice that this was an issue. Apart from that, we are not aware of any inconsistencies in object code generation for assembler directives, literals, and regular instructions.



Lessons Learned:
Going from conceptualizing a 2 pass assembler to actual implementaion is signficantly more difficult than anticipated. From determining the flow of files, to algorithmically determining opcodes for each formats was quite a daunting task; the most important lesson we learned was that these large projects take a lot of time. Working on the code took long, but it was also important to step away at some points and return with a fresh mind.



