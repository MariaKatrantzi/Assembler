/**
 * void pass2 (FILE * fp, LabelTable table)
 *      @param  fp  pointer to an open file (stdin or other file pointer)
 *                  from which to read lines of assembly source code
 *      @param  table  an existing Label Table
 *
 * This function reads the lines in an assembly source file and 
 * translates each instruction from assembly to machine language 
 * by calling other functions that process each instruction and 
 * determine the right format type.
 * 
 * Author: Maria Katrantzi
 *        with assistance from: Josh, Tim, Charlie
 *
 * Creation Date:  5/20/2019
 *   Modified:  5/25/2019   Completed functions and added documentation.
 *
 */

#include "assembler.h"
#include "pass2.h"

void pass2(FILE *fp, LabelTable table)
/*  Reads lines from a file pointer and translates each instruction
		 * from assembly to machine language. The label table, which is 
		 * constructed in pass1, is used from other functions to check if
         * a given label exists in the table, and use its address.
		 */

{
    int lineNum;             /* line number */
    int PC;                  /* program counter */
    char *tokBegin, *tokEnd; /* used to step thru inst */
    char inst[BUFSIZ];       /* will hold instruction; BUFSIZ
                                      is max size of I/O buffer
                                      (defined in stdio.h) */
    char *instrName;         /* instruction name (e.g., "add") */

    /* Continuously read next line of input until EOF is encountered.*/
    for (lineNum = 1, PC = 0; fgets(inst, BUFSIZ, fp); lineNum++, PC += 4)
    {
        /* If the line starts with a comment, move on to next line.
         * If there's a comment later in the line, strip it off
         *  (replace the '#' with a null byte).
         */
        if (*inst == '#')
            continue;
        (void)strtok(inst, "#");

        /* Read the first token, skipping any leading whitespace. */
        tokBegin = inst;
        getToken(&tokBegin, &tokEnd);
        /* tokBegin now points to 1st non-whitespace character
             * in the token and tokEnd points to 1st punctuation mark
             * or whitespace after the end of the token.
             */

        /* Skip label, if any */
        if (*(tokEnd) == ':')
        {
            /* Line has a label!  Get new token! */
            tokBegin = tokEnd + 1;
            getToken(&tokBegin, &tokEnd);
        }

        /* If empty line or line containing only a label, get next line */
        if (*tokBegin == '\0')
            continue;

        /* We have a valid token; turn it into a string and set
         * tokBegin to point to the character after the end.
         */
        *tokEnd = '\0';
        instrName = tokBegin;
        tokBegin = tokEnd + 1;

        printDebug("first non-label token is: %s.\n", instrName);

        /* Process instruction */
        processInstruction(instrName, tokBegin, lineNum, table, PC);
    }
}

void processInstruction(char *instName, char *restOfInstruction, int lineNum, LabelTable table, int PC)
/* Takes opcode (mnemonic name, e.g., "add"), pointer to the rest of 
         * the statement, line number, label table, and PC. Calls the 
         * getOpType function to get the opcode and, from that, determines
         * the instruction format type.
		 */
{
    /* Detrmine opcode or funct number */
    Format f = getOpType(instName, lineNum);

    char *rformat = "R";
    char *iformat = "I";
    char *jformat = "J";

    if (f.code != -1 && f.opType != NULL)
    {
        if (strcmp(f.opType, rformat) == SAME)
        {
            assembleR(f.code, restOfInstruction, lineNum); /* R-format */
        }

        else if (strcmp(f.opType, iformat) == SAME)
        {
            assembleI(f.code, restOfInstruction, lineNum, table, PC); /* I-format */
        }

        else if (strcmp(f.opType, jformat) == SAME)
        {
            assembleJ(f.code, restOfInstruction, lineNum, table); /* J-format */
        }
    }
}

Format getOpType(char *instName, int lineNum)
/* Takes opcode (mnemonic name, e.g., "add") and returns
		 * 	opType ('R', 'I', 'J') and code (opcode or funct number).
		 *	Takes line number as input for printing error messages.
		 */
{
    /* initialize the members of the Format struct */
    Format f;
    f.code = -1;
    f.opType = NULL;

    /* use an array of pointers to hold the R-format instructions ordered by their funct */
    char *arrayR[] =
        {
            "sll", "", "srl", "", "", "", "", "", "jr", "", "",
            "", "", "", "", "", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "add", "addu", "sub",
            "subu", "and", "or", "", "nor", "", "", "slt", "sltu"};

    /* use an array of pointers to hold the I-format instructions ordered by their opcode */
    char *arrayI[] =
        {
            "", "", "", "", "beq", "bne", "", "", "addi", "addiu", "slti",
            "sltiu", "andi", "ori", "", "lui", "", "", "", "", "", "", "", "", "",
            "", "", "", "", "", "", "", "", "", "",
            "lw", "", "", "", "", "", "", "", "sw"};

    /* use an array of pointers to hold the J-format instructions ordered by their opcode */
    char *arrayJ[] = {"", "", "j", "jal"};

    /* check if the input instruction name exists in any of the arrays, and return the code and format type*/
    int j;
    for (j = 0; j <= 3; j++)
    {
        if (strcmp(arrayJ[j], instName) == SAME)
        {
            f.code = j;
            f.opType = "J";
            return f;
        }
    }

    int i;
    for (i = 0; i <= 43; i++)
    {
        if (strcmp(arrayR[i], instName) == SAME)
        {
            f.code = i;
            f.opType = "R";
            return f;
        }

        else if (strcmp(arrayI[i], instName) == SAME)
        {
            f.code = i;
            f.opType = "I";
            return f;
        }
    }

    /* otherwise print an error message if the instruction name is invalid */
    printError("Unexpected error on line %d: %s is an invalid Instruction Name.\n", lineNum, instName);

    return f;
}

int getRegNbr(char *regName, int lineNum)
/* Takes register name (e.g., $t0) and returns register number.
		 *	Takes line number as input for printing error messages.
		 */
{
    /* use an array of pointers to hold the MIPS registers ordered by their number */
    char *regArray[] =
        {
            "$zero",
            "$at",
            "$v0", "$v1",
            "$a0", "$a1", "$a2", "$a3",
            "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
            "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
            "$t8", "$t9",
            "$k0", "$k1",
            "$gp", "$sp", "$fp", "$ra"};

    /* check if the input register name exists in the array, and return the index where it was found*/
    int k;
    for (k = 0; k <= 32; k++)
    {
        if (strcmp(regArray[k], regName) == SAME)
        {
            return k;
        }
    }

    /* otherwise print an error message if the register name is invalid */
    printError("Unexpected error on line %d: %s is an invalid Register Name.\n", lineNum, regName);

    return -1;
}

void assembleR(int opcode, char *restOfStmt, int lineNum)
/* Takes opcode (actually funct number in most cases),
		 * pointer to the rest of the statement, and line number for
		 * printing error messages.
		 * Prints R-format instruction to standard output.
		 */
{
    int ntok;
    char *parameters[3];

    /* call getNtokens to determine the layout and print the corresponding machine language instruction */
    if (opcode == 8)
    {
        ntok = getNTokens(restOfStmt, 1, parameters);

        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);

            /* check if the register name is $ra */
            if (one == 31)
            {
                printBin(0, 6);
                printBin(one, 5);
                printBin(0, 15);
                printBin(opcode, 6);
                printf("\n");
            }
        }
    }

    else if (opcode == 0 || opcode == 2)
    {
        ntok = getNTokens(restOfStmt, 3, parameters);
        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);
            int two = getRegNbr(parameters[1], lineNum);

            char *str = parameters[2];
            if (*(str) != '$')
            {
                /* convert string to integer */
                int shamt = atoi(str);

                if (one != -1 && two != -1)
                {
                    printBin(0, 11);
                    printBin(two, 5);
                    printBin(one, 5);
                    printBin(shamt, 5);
                    printBin(opcode, 6);
                    printf("\n");
                }
            }

            else
            {
                /* print error */
                printError("Unexpected error on line %d: invalid token %s for sll/ srl instruction.\n", lineNum, parameters[2]);
            }
        }
    }

    else
    {
        ntok = getNTokens(restOfStmt, 3, parameters);

        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);
            int two = getRegNbr(parameters[1], lineNum);
            int three = getRegNbr(parameters[2], lineNum);

            if (one != -1 && two != -1 && three != -1)
            {

                printBin(0, 6);
                printBin(two, 5);
                printBin(three, 5);
                printBin(one, 5);
                printBin(0, 5);
                printBin(opcode, 6);
                printf("\n");
            }
        }
    }
}

void assembleI(int opcode, char *restOfStmt, int lineNum, LabelTable table, int PC)
/* Takes opcode, pointer to the rest of the statement, line number
		 * (for printing error messages), label table, and PC.
		 * Prints I-format instruction to standard output.
		 */
{

    int ntok;
    char *parameters[3];

    /* call getNtokens to determine the layout and print the corresponding machine language instruction */
    if (opcode == 4 || opcode == 5)
    {

        ntok = getNTokens(restOfStmt, 3, parameters);
        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);
            int two = getRegNbr(parameters[1], lineNum);

            if (one != -1 && two != -1)
            {
                char *label = parameters[2];
                int offset = 0;

                /* create a pointer that stores the address of the input LabelTable */
                LabelTable *tablep;
                tablep = &table;

                /* check if the label exists in the table  */
                int add = findLabel(tablep, label);

                /* label is not in the table */
                if (add == -1)
                {
                    /* print error */
                    printError("Unexpected error on line %d: Label %s not found in the label table.\n", lineNum, label);
                }

                else
                {
                    /* calculate offset */
                    int NPC = PC + 4;
                    offset = (add - NPC) / 4;

                    printBin(opcode, 6);
                    printBin(one, 5);
                    printBin(two, 5);
                    printBin(offset, 16);
                    printf("\n");
                }
            }
        }
    }

    else if (opcode == 15)
    {
        ntok = getNTokens(restOfStmt, 2, parameters);

        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);

            if (one != -1)
            {
                char *str = parameters[1];

                if (*(str) != '$')
                {

                    /* convert string to integer */
                    int two = atoi(str);

                    printBin(opcode, 6);
                    printBin(0, 5);
                    printBin(one, 5);
                    printBin(two, 16);
                    printf("\n");
                }

                else
                {
                    /* print error */
                    printError("Unexpected error on line %d: invalid token %s for lui instruction.\n", lineNum, parameters[2]);
                }
            }
        }
    }

    else if (opcode == 35 || opcode == 43)
    {
        ntok = getNTokens(restOfStmt, 3, parameters);

        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);
            int two = getRegNbr(parameters[2], lineNum);

            if (one != -1 && two != -1)
            {
                char *str = parameters[1];

                if (*(str) != '$')
                {
                    /* convert string to integer */
                    int offset = atoi(str);

                    printBin(opcode, 6);
                    printBin(two, 5);
                    printBin(one, 5);
                    printBin(offset, 16);
                    printf("\n");
                }

                else
                {
                    /* print error */
                    printError("Unexpected error on line %d: invalid token %s for lw/ sw instruction.\n", lineNum, parameters[2]);
                }
            }
        }
    }
    else
    {
        ntok = getNTokens(restOfStmt, 3, parameters);

        if (ntok == 0)
        {
            /* parameters[0] contains error message */
            printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
        }

        else
        {
            int one = getRegNbr(parameters[0], lineNum);
            int two = getRegNbr(parameters[1], lineNum);

            if (one != -1 && two != -1)
            {
                char *str = parameters[2];

                if (*(str) != '$')
                {

                    /* convert string to integer */
                    int offset = atoi(str);

                    printBin(opcode, 6);
                    printBin(two, 5);
                    printBin(one, 5);
                    printBin(offset, 16);
                    printf("\n");
                }

                else
                {
                    /* print error */
                    printError("Unexpected error on line %d: invalid token %s for I-format instruction.\n", lineNum, parameters[2]);
                }
            }
        }
    }
}

void assembleJ(int opcode, char *restOfStmt, int lineNum, LabelTable table)
/* Takes opcode, pointer to the rest of the statement,
		 * line number (for printing error messages), and label table.
		 * Prints J-format instruction to standard output.
		 */
{
    int ntok;
    char *parameters[1];

    /* call getNtokens to determine the layout and print the corresponding machine language instruction */
    ntok = getNTokens(restOfStmt, 1, parameters);

    if (ntok == 0)
    {
        /* parameters[0] contains error message */
        printError("Unexpected error on line %d: %s\n", lineNum, parameters[0]);
    }

    else
    {
        char *label = parameters[0];
        int address = 0;

        /* create a pointer that stores the address of the input LabelTable */
        LabelTable *tablep;
        tablep = &table;

        /* check if the label exists in the table  */
        int add = findLabel(tablep, label);

        /* label is not in the table */
        if (add == -1)
        {
            /* print error */
            printError("Unexpected error on line %d: Label %s not found in the label table.\n", lineNum, label);
        }

        else
        {
            /* calculate address */
            address = add / 4;

            printBin(opcode, 6);
            printBin(address, 26);
            printf("\n");
        }
    }
}

void printBin(int n, int length)
/* Takes a numeric value and prints the binary format to
		 * standard output as characters.  The length specifies how
		 * many binary digits to print.
		 * E.g., printBin (3, 5) would print 00011 to stdout (the binary
		 * representation for the number 3, expressed using 5 characters).
		 */
{
    /* use malloc function to allocate space for the string */
    char *dest = (char *)malloc(sizeof(char) * (length + 1));

    /* create a count to keep track of the pointer's dest position */
    int count = 0;

    while (n > 0)
    {
        /* calculate remainder */
        int rem = n % 2;

        /* write appropriate character */
        if (rem == 0)
            *dest = '0';
        else
            *dest = '1';

        /* divide integer by 2 */
        n = n / 2;

        /* dest points to the next position in the string */
        dest++;

        /* increment count */
        count++;
    }

    /* fill up any empty spaces at the end of the string with '0' characters */
    while (count < length)
    {
        *dest = '0';
        dest++;
        count++;
    }

    /* add the null byte to end of the string */
    *dest = '\0';

    /* dest points to the character before the null byte */
    dest--;

    int k;

    /* print the characters in reverse order */
    for (k = 1; k <= length; k++)
    {
        printf("%c", *dest);
        dest--;
    }
}