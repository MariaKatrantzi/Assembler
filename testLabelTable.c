/*
 * Test Driver to test the functions to access and manipulate a label table.
 * It includes the following tests:
 *
 * The main method includes sufficient test cases to check the correctness of the 
 * functions we implemented in the LabelTable.c file. Test cases are used to test
 * both the static and dynamic table. For the static table, we test the printLabels
 * function when the table is empty and when we have added five label entries. We also
 * test the findLabel and testSearch functions for different labels. We follow a similar
 * procedure to test the dynamic table after we have initialized it using the tableInit 
 * function. In addition, we use the addLabel method to add label entries to the table 
 * and we test the tableResize function by adding a label beyond its current capacity.
 * Lastly, we try to add an existing label entry name to the table in order to verify
 * that an error message will pop-up.
 *  
 * Author: Maria Katrantzi
 *        with assistance from: TAs
 *
 * Creation Date:  4/16/2019
 *   Modified:  4/23/2019   Updated to call functions from the LabelTable.c file.
 *   Modified:  4/24/2019   Updated to add test cases and documentation.
 * 
 */

#include "assembler.h"

const int SAME = 0; /* useful for making strcmp readable */
                    /* e.g., if (strcmp (str1, str2) == SAME) */

static void testSearch(LabelTable *table, char *searchLabel);

int main(int argc, char *argv[])
{
    /* Process command-line argument (if provided) for
     *    debugging indicator (1 = on; 0 = off).
     */
    (void)process_arguments(argc, argv);

    /* Can turn debugging on or off here (debug_on() or debug_off())
     * if not specified on the command line.
     */

    /* Create 2 tables, one static and one dynamic, for testing purposes */
    LabelTable testTable1; /* will be a table with static entries */
    LabelTable testTable2; /* will be a table with dynamic entries */

    printf("\n");
    printf("===== Testing with static table =====\n");
    printf("\n");

    /* Set nbrLabels to 0 and test printLabels on empty table */
    testTable1.capacity = 0;
    testTable1.nbrLabels = 0;
    testTable1.entries = NULL;
    printLabels(&testTable1);
    printf("\n");

    /* Use testSearch to test findLabel in an empty table */
    testSearch(&testTable1, "Maria1");

    /* Initialize testTable1 with a static array of size 5 */
    LabelEntry staticEntries[5];
    testTable1.capacity = 5;
    testTable1.nbrLabels = 5;
    testTable1.entries = staticEntries;

    /* Add 5 label entries to testTable1 */
    staticEntries[0].label = "Maria1";
    staticEntries[0].address = 1000;
    staticEntries[1].label = "Maria2";
    staticEntries[1].address = 1005;
    staticEntries[2].label = "Maria3";
    staticEntries[2].address = 1010;
    staticEntries[3].label = "Maria4";
    staticEntries[3].address = 1015;
    staticEntries[4].label = "Maria5";
    staticEntries[4].address = 1020;

    /* Test printLabels and findLabel with static testTable1 */
    printLabels(&testTable1);
    printf("\n");

    int x = findLabel(&testTable1, "Maria1");
    if (x == -1)
        printf("The label you're looking for does not exist.\n");
    else
        printf("The label was found at address %d.\n", findLabel(&testTable1, "Maria1"));

    printf("\n");

    /* Use testSearch to test findLabel */
    testSearch(&testTable1, "Maria1");
    testSearch(&testTable1, "main");

    printf("===== Testing with dynamic table =====\n");

    /* Initialize testTable2 as a dynamic (changeable size) table */
    tableInit(&testTable2);
    printf("\n");

    /* Test printLabels, findLabel, and addLabel with dynamic testTable2 */
    printLabels(&testTable2);
    printf("\n");

    /* Use testSearch to test findLabel in an empty table */
    testSearch(&testTable2, "end");

    addLabel(&testTable2, "main", 1025);
    addLabel(&testTable2, "for1", 1030);
    addLabel(&testTable2, "for2", 1035);
    addLabel(&testTable2, "for3", 1040);
    addLabel(&testTable2, "for4", 1045);

    printLabels(&testTable2);
    printf("\n");

    /* Add a label beyond the current capacity */
    addLabel(&testTable2, "end", 1050);

    int y = findLabel(&testTable2, "main");
    if (y == -1)
        printf("The label you're looking for does not exist.\n");
    else
        printf("The label found at address %d.\n", findLabel(&testTable2, "main"));

    printf("\n");

    /* Use testSearch to test findLabel */
    testSearch(&testTable2, "Maria1");
    testSearch(&testTable2, "end");

    /* Add an existing label */
    addLabel(&testTable2, "for2", 1055);

    printf("\n");
    printLabels(&testTable2);
}

/*
 * testSearch tests the findLabel function, printing out the label being
 * searched for and either the address where it was found or, if it was
 * not found, an address of -1 and a message that the label was not in
 * the table.  
 *  @param  table        a pointer to the table through which to search
 *  @param  searchLabel  the label to search for
 */
static void testSearch(LabelTable *table, char *searchLabel)
{
    int address;

    printf("Looking for %s...\n", searchLabel);

    address = findLabel(table, searchLabel);

    if (address == -1)
    {
        printf("\tthe label you're looking for does not exist.\n");
        printf("\n");
    }
    else
    {
        printf("\tthe label is in address %d.\n", address);
        printf("\n");
    }
}
