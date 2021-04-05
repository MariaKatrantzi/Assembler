PROJECT TITLE: Assembler Programming Project

PURPOSE OF PROJECT: Write a program that will read in assembly language
and write out the corresponding machine language commands.

VERSION or DATE: 25 May 2019

HOW TO START THIS PROJECT: Open the assesmbler project within your text
editor. Use the "assembler.c" and "testassembler.txt" file to change
the output of the project.

AUTHOR: Maria Katrantzi

USER INSTRUCTIONS: To run the program, run "make assembler" and "./assembler 
testassembler.txt" on the terminal line.

For a sample Input, consider the following assembly code:
main:   lw $a0, 0($t0)
begin:  addi $t0, $zero, 0        # beginning
        addi $t1, $zero, 1
loop:   slt $t2, $a0, $t1         # top of loop
        bne $t2, $zero, finish
        add $t0, $t0, $t1
        addi $t1, $t1, 2
        j loop                    # bottom of loop
finish: add $v0, $t0, $zero

The corresponding Output has the following form:
10001101000001000000000000000000
00100000000010000000000000000000
00100000000010010000000000000001
00000000100010010101000000101010
00010101010000000000000000000011
00000001000010010100000000100000
00100001001010010000000000000010
00001000000000000000000000000011
00000001000000000001000000100000