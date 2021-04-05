/**
 * LabelTable pass1 (FILE * fp)
 *      @param  fp  pointer to an open file (stdin or other file pointer)
 *                  from which to read lines of assembly source code
 *      @return a newly-created table containing labels found in the
 *              input file, each with the address of the instruction
 *              containing it (assuming the first line of input
 *              corresponds to address 0)
 *
 * This function reads the lines in an assembly source file and looks
 * for labeled statements.  It builds a table of labels and addresses.
 * It returns a copy of the table it created.  If an error occurs, the
 * function prints an error message and returns the table as it exists
 * at that point (possibly empty).
 *
 * Author: Alyce Brady
 * Date:   2/16/99
 *
 * Modified by:  Alyce Brady, 6/10/2014
 *      Take open file pointer as parameter, rather than filename.
 *
 */

#include "assembler.h"

LabelTable pass1 (FILE * fp)
  /* returns a copy of the label table that was constructed */
{
    LabelTable table;              /* the table of labels & addresses */
    int    PC = 0;                 /* the program counter */
    char * tokBegin, * tokEnd;     /* used to step thru inst */
    char   inst[BUFSIZ];           /* will hold instruction; BUFSIZ
                                      is max size of I/O buffer
                                      (defined in stdio.h) */

    /* create a small label table to begin with */
    tableInit (&table);
    if ( tableResize (&table, 10) == 0)
    {
        /* error message already printed */
        return table;
    }

    /* Continuously read next line of input until EOF is encountered.
     * Check each line to see if it has a label; if it does, add it
     * to the label table.
     */
    for (PC = 0; fgets (inst, BUFSIZ, fp); PC += 4)
    {
        /* If the line starts with a comment, move on to next line.
         * If there's a comment later in the line, strip it off
         *  (replace the '#' with a null byte).
         */
        if ( *inst == '#' ) continue;
        (void) strtok (inst, "#");

        /* Read the first token, skipping any leading whitespace. */
        tokBegin = inst;
        getToken (&tokBegin, &tokEnd);
            /* tokBegin now points to 1st non-whitespace character
             * in the token and tokEnd points to 1st punctuation mark
             * or whitespace after the end of the token.
             */

        /* Check each line to see if it has a label; if it does,
         * process it.
         */
        if ( *(tokEnd) == ':' )
        {
            /* Line has a label! */
            *tokEnd = '\0';      /* truncate everything after label */

            /* Add label to table */
            if (addLabel (&table, tokBegin, PC) == 0)
            {
                /* error message already printed */
                continue;
            }
        }
    }

    /* EOF, but don't close the file here. */
    return table;
}
