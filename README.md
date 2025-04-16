#Lab -3 RISC-V RV64 Assembler

##Description
This project aims to create a RISC-V assembler capable of encoding instructions given in a text file 'input.s' and creating a hex file 

##Files used
'assembler.c'      
'load_instruction.c'  
's_instruction.c'
'b_instruction.c'  
'jal_instruction.c'   
'r_instruction.c'  
'i_instruction.c' 
'jalr_instruction.c'  
'u_instruction.c'
Makefile
README.md          

In addition, there is a 'tested_cases' file that contains all the cases I have tested.

##Usage instructions
-This implementation supports blank lines as well as comments 

-It supports both numbered and convention names for registers (x8, s0, fp, etc)

-Ensure that no line has exactly 48 number of characters as this leads to skipping a line

-The input needs to be in the format 
	add x1, x2, t1
	addi x1, x1, -12
	loop: beq x0, x0, loop
 
-If an error is detected, the line and the contents of the line will be printed and the program will end

-Keep label name to 10-15 characters as anything bigger will lead to undefined behaviour

-Label can contain any character except ';' as it denotes comment

-Line must start from 1st character 
