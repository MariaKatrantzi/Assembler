# A simple makefile

GCC=gcc -Wall -Wextra -Wpedantic -Wformat -Wshadow -Wredundant-decls \
    -Wstrict-prototypes
# Can also use -Wtraditional or -Wmissing-prototypes

all:	testLabelTable

#  Switch to alternative versions of the all target as you're ready for them.
# all:	testLabelTable testgetNTokens
# all:	testLabelTable testgetNTokens testPass1
# all:	testLabelTable testgetNTokens testPass1 assembler

testLabelTable: assembler.h \
	LabelTable.o \
    	process_arguments.o \
	printDebug.o \
	printError.o \
    	testLabelTable.o
	$(GCC) -g process_arguments.o \
		LabelTable.o printDebug.o printError.o testLabelTable.o \
	    	-o testLabelTable

testGetNTokens: 	assembler.h \
	getToken.o \
	getNTokens.o \
	printDebug.o \
	printError.o \
    	testGetNTokens.o
	$(GCC) -g testGetNTokens.o getNTokens.o getToken.o \
	    printDebug.o printError.o -o testGetNTokens

testPass1: 	assembler.h \
    	LabelTable.o \
    	process_arguments.o \
	getToken.o \
	getNTokens.o \
	pass1.o \
	printDebug.o \
	printError.o \
	testPass1.o
	$(GCC) -g LabelTable.o process_arguments.o \
	    getNTokens.o getToken.o pass1.o \
	    printDebug.o printError.o testPass1.o -o testPass1

assembler: 	assembler.h \
  	pass2.h \
    	LabelTable.o \
    	process_arguments.o \
	getToken.o \
	getNTokens.o \
	pass1.o \
	pass2.o \
	printDebug.o \
	printError.o \
	assembler.o
	$(GCC) -g LabelTable.o process_arguments.o \
	    getNTokens.o getToken.o pass1.o pass2.o \
	    printDebug.o printError.o assembler.o -o assembler

assembler.h: same.h LabelTable.h getToken.h printFuncs.h process_arguments.h
	touch assembler.h

LabelTable.o: LabelTable.h LabelTable.c
	$(GCC) -c -g LabelTable.c 

process_arguments.o: process_arguments.h process_arguments.c
	$(GCC) -c -g process_arguments.c

printDebug.o: printFuncs.h printDebug.c
	$(GCC) -c -g printDebug.c

printError.o: printFuncs.h printError.c
	$(GCC) -c -g printError.c

testLabelTable.o: assembler.h LabelTable.h testLabelTable.c
	$(GCC) -c -g testLabelTable.c

getToken.o: getToken.h getToken.c
	$(GCC) -c -g getToken.c

getNTokens.o: getToken.h getNTokens.c
	$(GCC) -c -g getNTokens.c

testGetNTokens.o: assembler.h testGetNTokens.c
	$(GCC) -c -g testGetNTokens.c

pass1.o: assembler.h pass1.c
	$(GCC) -c -g pass1.c

testPass1.o: assembler.h testPass1.c
	$(GCC) -c -g testPass1.c

pass2.o: assembler.h pass2.c
	$(GCC) -c -g pass2.c

assembler.o: assembler.h assembler.c
	$(GCC) -c -g assembler.c

clean: 
	rm -rf *.o testLabelTable testGetNTokens testPass1 assembler