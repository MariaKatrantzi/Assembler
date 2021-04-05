/*
 * Pass 2: data structure and associated functions
 *
 * This file provides the data structure and declarations for a group
 * of associated functions useful for translating an instruction from
 * assembly to machine language.
 *
 * Author: Maria Katrantzi
 *
 * Creation Date:	5/20/2019
 *   Modified:	5/25/2019   Updated functions' description.
 *
*/

#ifndef PASS2_H
#define PASS2_H

/* THE DATA STRUCTURES */

/* The struct Format is used to store the code(opcode or funct number)
 *  and the format('R', 'I', 'J') of an instruction.
 */

typedef struct
{
	int code;	 /* instruction code */
	char *opType; /* instruction format */
} Format;

/* THE FUNCTIONS */

void pass2(FILE *fp, LabelTable table);
/*  Reads lines from a file pointer and translates each instruction
		 * from assembly to machine language. The label table, which is 
		 * constructed in pass1, is used from other functions to check if
         * a given label exists in the table, and use its address.
		 */

void processInstruction(char *instName, char *restOfInstruction, int lineNum, LabelTable table, int PC);
/* Takes opcode (mnemonic name, e.g., "add"), pointer to the rest of 
         * the statement, line number, label table, and PC. Calls the 
         * getOpType function to get the opcode and, from that, determines
         * the instruction format type.
		 */

Format getOpType(char *instName, int lineNum);
/* Takes opcode (mnemonic name, e.g., "add") and returns
		 * 	opType ('R', 'I', 'J') and code (opcode or funct number).
		 *	Takes line number as input for printing error messages.
		 */

int getRegNbr(char *regName, int lineNum);
/* Takes register name (e.g., $t0) and returns register number.
		 *	Takes line number as input for printing error messages.
		 */

void assembleR(int opcode, char *restOfStmt, int line);
/* Takes opcode (actually funct number in most cases),
		 * pointer to the rest of the statement, and line number for
		 * printing error messages.
		 * Prints R-format instruction to standard output.
		 */

void assembleI(int opcode, char *restOfStmt, int lineNum, LabelTable table, int PC);
/* Takes opcode, pointer to the rest of the statement, line number
		 * (for printing error messages), label table, and PC.
		 * Prints I-format instruction to standard output.
		 */

void assembleJ(int opcode, char *restOfStmt, int line, LabelTable table);
/* Takes opcode, pointer to the rest of the statement,
		 * line number (for printing error messages), and label table.
		 * Prints J-format instruction to standard output.
		 */

void printBin(int value, int length);
/* Takes a numeric value and prints the binary format to
		 * standard output as characters.  The length specifies how
		 * many binary digits to print.
		 * E.g., printBin (3, 5) would print 00011 to stdout (the binary
		 * representation for the number 3, expressed using 5 characters).
		 */

#endif