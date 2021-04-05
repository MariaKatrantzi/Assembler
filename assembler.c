/* 
 * This is a driver to test pass1 and the Label Table functions.  It
 * reads its input from a file if a filename has been passed as a
 * command-line argument, or from the standard input otherwise.  It
 * constructs a table of instruction labels and addresses from the
 * input, where a label's address is determined by the address of its
 * instruction.  Instructions are assumed to be 4 bytes long, with the
 * first instruction starting at address 0.  Labels that appear anywhere
 * but at the beginning of a line are ignored.
 * For example, the following two lines of code would result in a table
 * with one entry, indicating that label A_LABEL is at address 4.
 *              add $t1, $t1, $t1   # This instruction is at address 0
 *    A_LABEL:  slt $t0, $t1, $t2   # This instruction is at address 4
 *              bne $t0, $zero, A_LABEL  # This instr. is at address 8
 *
 * USAGE:
 *      name [ filename ] [ 0|1 ]
 * where "name" is the name of the executable, "filename" is an optional
 * file containing the input to read, and " 0" or "1" specifies that
 * debugging should be turned off or on, respectively, regardless of any
 * calls to debug_on, debug_off, or debug_restore in the program.  Both
 * arguments are optional; if both are present they may appear in either
 * order.  If no filename is provided, the program reads its input from
 * stdin.  If no debugging choice is provided, the program prints
 * debugging messages, or not, depending on indications in the code.
 *
 * INPUT:
 * This program expects the input to consist of lines of MIPS
 * instructions, each of which may (or may not) contain a label at the
 * beginning of the line.  The input should have one MIPS instruction on
 * each line.  Labels should appear as the first token in the line,
 * followed immediately by a colon.  Labels may also appear elsewhere
 * in an instruction, but those labels are ignored by this program.
 *
 * OUTPUT:
 * The program prints the contents of the label table it has
 * constructed.  For the example above, the output might look like:
 *      Label: A_LABEL   Address: 4
 * (The actual formatting depends on the implementation of the
 * printLabels function in LabelTable.c.)
 *
 * ERROR CONDITIONS:
 * It is an error for the input to contain multiple instructions with
 * the same label at the beginning of the line.
 *
 * Author: Alyce Brady
 * Date: 2/18/99
 *
 * Modified by:  Alyce Brady, 6/10/2014
 *      Allow filename to be passed as a parameter.
 * Modified by:  Alyce Brady, 5/25/2015
 *      Support use of printErr and printDebug functions.
 *      Improve function documentation.
 * Modified by:  Maria Katrantzi, 5/25/2019
 *      Turn debugging off, and uncomment part of the code.
 * 
 */

#include "assembler.h"
#include "pass2.h"

const int SAME = 0; /* useful for making strcmp readable */
                    /* e.g., if (strcmp (str1, str2) == SAME) */

int main(int argc, char *argv[])
{
    FILE *fptr; /* file pointer */
    LabelTable table;

    /* Process command-line arguments (if any) -- input file name
     *    and/or debugging indicator (1 = on; 0 = off).
     */
    fptr = process_arguments(argc, argv);
    if (fptr == NULL)
    {
        return 1; /* Fatal error when processing arguments */
    }

    /* Can turn debugging on or off here (debug_on() or debug_off())
     * if not specified on the command line.
     */
    debug_off(); /* turn debugging off. */

    /* Call pass1 to generate the label table. */
    table = pass1(fptr);

    /* Print the label table if debugging is turned on. */
    if (debug_is_on())
        printLabels(&table);

    /* Rewind the file pointer to be back at the beginning
     * of the file and call pass2 passing it the label table.
     */
    rewind(fptr);
    pass2(fptr, table);

    (void)fclose(fptr);
    return 0;
}