/*
 * Label Table: data structure and associated functions
 *
 * This file provides the data structure and declarations for a group
 * of associated functions useful for creating, maintaining, and using
 * a table of assembly language instruction labels and the memory
 * addresses associated with them.
 *
 * Author: Alyce Brady
 *
 * Creation Date:   2/16/99
 *   Modified:  12/20/2000   Updated postcondition information.
 *
*/

#ifndef LABEL_H
#define LABEL_H

/* THE DATA STRUCTURES */

/* The first type definition defines the type for a single entry in the
 * table.  The second defines the type for the table as a whole.
 */

typedef struct
{
        char *label; /* label name */
        int address; /* address of label */
} LabelEntry;

typedef struct
{
        int capacity;  /* capacity of the table */
        int nbrLabels; /* actual nbr of entries in table */
        LabelEntry *entries;
} LabelTable;

/* THE FUNCTIONS */

void tableInit(LabelTable *table);
/* Postcondition: table is initialized to indicate that there
         *       are no label entries in it.
         */

int tableResize(LabelTable *table, int newSize);
/* Postcondition: table now has the capacity to hold newSize
         *      label entries.  If the new size is smaller than the
         *      old size, the table is truncated after the first
         *      newSize entries.
         * Returns 1 if everything went OK; 0 if memory allocation error
         *      or table doesn't exist.
         */

int addLabel(LabelTable *table, char *labelName, int memLoc);
/* Postcondition: if label was already in table, the table is 
         *      unchanged; otherwise a new entry has been added to the 
         *      table with the specified label name and instruction address
         *      (memory location) and the table has been resized if necessary.
         * Returns 1 if no fatal errors occurred; 0 if memory allocation error
         *      or table doesn't exist.
         */

int findLabel(LabelTable *table, char *label);
/* Returns the address associated with the label; -1 if label is
         *       not in the table or if table doesn't exist.
         */

void printLabels(LabelTable *table);
/* Postcondition: all the labels in the table, with their
         *      associated addresses, have been printed to the standard
         *      output.
         */

#endif
