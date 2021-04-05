 #TestCases 

 VERSION or DATE: 25 May 2019

 AUTHOR: Maria Katrantzi
  
  The input file "testassembler.txt" includes sufficient test cases to check the 
 correctness of the functions we implemented in the pass2.c file. 

 In increasing order, the test cases are:

 0) add an existing label to the label table (mostly to check pass1.c)
 1-21) check different Instruction and Register names (except beq, bne, j, and jal instructions)
 22-39) check beq, bne, j, and jal instructions (using the MIPS code of the binToDec function)
 40) empty line
 41) line containing only a label
 42) label in the middle of an instruction
 43) label in the end of an instruction
 44) line starts with a comment
 45) comment in the middle of an instruction
 46) line contains only an Instruction name
 47) line does not contain an Instruction name
 48-50) invalid Instruction names
 51) invalid Register Name
 52-62) invalid instruction layouts
 63) invalid label (label not in the label table)