# assembler-assignments: Assembler Assignments
##  Label Table Project, and <br> Assember

### Label Table Project
This repository contains most of the starter code (including a `Makefile`)
needed for the Label Table Project, in which students will write and
test functions that create and interact with a Label Table (useful for the
Assembler program to be written later).

This repository does *not* contain several utility functions for processing
command line arguments and printing error and debugging messages that were
included in the Disassembler Assignments repository.  You should copy (or
make symbolic links to) the following files from that repository to this one:
- `same.h`
- `process_arguments.h`
- `process_arguments.c`
- `printFuncs.h`
- `printError.c`
- `printDebug.c`
(Alternatively, you can download them from the project description page.)

Once you have copies (or links) to those files in the repository, you can
compile and run the skeleton test driver for the LabelTable project by
typing `make testLabelTable` (or just `make`), followed by
`./testLabelTable`.  If you're interested, you can also compile and run
the test driver that was written to test the getNTokens function provided
to you: `make testGetNTokens` followed by `./testGetNTokens`.

(See [here](http://www.cs.kzoo.edu/cs230/Projects/LabelTableProj.html)
for the full project description for the Label Table program.)

### Assembler
The repository also contains some additional starter code for the
follow-up assembler program, including a small, initial test file and
its expected output.
(See [here](http://www.cs.kzoo.edu/cs230/Projects/AssemblerProj.html)
for the project description for the assembler program.)
