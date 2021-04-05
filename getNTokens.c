/*
 * This file contains the getNTokens function, which reads the specified
 * number of tokens from a string.  This function takes three parameters,
 * a string containing tokens, the number of tokens that should be in
 * the string, and an array large enough to take N strings.  The
 * function does not modify N, but it does modify both the original
 * string (inserting null bytes after each token) and the array of results.
 *
 * A token is defined as a string of characters, not including
 * whitespace, commas, parentheses, or a colon.  For example, the
 * following string contains 5 tokens, which are "loop", "add", "$t0",
 * "$t1", and "$t2".
 *      loop:   add $t0, $t1, $t2
 *
 * If the string contains exactly N tokens, getNTokens returns 1 and
 * fills the given array with pointers to N strings, one for each token.
 * If the string contains fewer than N tokens, getNTokens returns 0 and
 * puts a pointer to an error message in the first array element
 * ("Instruction contains fewer tokens than expected.").  If the string
 * contains more than N tokens, getNTokens again returns 0 and puts a
 * pointer to a different error message in the first array element
 * ("Instruction contains more tokens than expected.").
 *
 * The getNTokens function uses the getToken function.
 *
 * See getNTokens.h for more specific information about how getNTokens
 * behaves and for an example.
 *
 * Author:          Alyce Brady
 * Creation Date:   5/21/14
 *
 */

#include <stdio.h>

#include "getToken.h"

/* Define error messages (global within this file). */
static char * TOO_FEW = "Instruction contains fewer tokens than expected.";
static char * TOO_MANY = "Instruction contains more tokens than expected.";

/**
 * getNTokens -- read N tokens from instructionBuffer, putting the
 *               resulting tokens in results
 * Parameters:  instructionBuffer -- a string containing tokens
 *              N -- the expected number of tokens in instructionBuffer
 *              results -- an array of strings containing the tokens
 * Precondition:
 *              instructionBuffer is a valid pointer to a string &&
 *              N >= 1 &&
 *              results is a valid pointer to an array containing space
 *                  for at least N string pointers
 * Postcondition:
 *              If instructionBuffer contains N tokens, results is
 *              filled with strings, each containing one of those
 *              tokens and getNTokens returns 1.  If
 *              instructionBuffer contains fewer or more than N tokens, 
 *              getNTokens returns 0 and puts a pointer to an
 *              appropriate error message in results[0].
 */
int getNTokens (char * instructionBuffer, int N, char * results[])
{
    int i;
    char * tokBegin = instructionBuffer;
    char * tokEnd;

    /* We're not responsible for checking pre-condition, and we can't
     * check some aspects of it, but let's check the basics anyway.
     */
    if ( instructionBuffer == NULL || N < 1 || results == NULL )
        return 0;

// printDebug("About to get expected tokens.\n");
    /* Get the expected tokens. */
    for ( i = 0; i < N; i++, tokBegin = tokEnd + 1 )
    {
        getToken(&tokBegin, &tokEnd);
        if ( *tokBegin == '\0' )
        {
            /* Token expected, but no token found. */
            results[0] = TOO_FEW;
            return 0;
        }
// printDebug("Found token starting at %s.\n", tokBegin);

        /* Add this token to the results array. */
        results[i] = tokBegin;

        /* Does this token already end in a null byte? */
        if ( *tokEnd == '\0' )
        {
// printDebug("Token is at end of string.\n");
            /* Yes, which means token is at end of string;
             * is this OK, or too soon?
             */
            if ( i < N-1 )
            {
                /* Too soon, have not found N tokens. */
                results[0] = TOO_FEW;
                return 0;
            }

            /* OK; have found all tokens! */
            return 1;

        }
        else
        {
// printDebug("Token is not at end of string; rest of string is %s.\n", tokEnd);
            /* Insert null byte to turn token into a string.*/
            *tokEnd = '\0';
// printDebug("Token is %s; rest of string is %s.\n", tokBegin, tokEnd + 1);
        }
// printDebug("About to loop to next token.\n");
    }

    /* Have found all expected tokens, but last one did not end with a
     * null byte.  Is there another token in instructionBuffer, or do we
     * just have left-over whitespace?
     */
// printDebug("Is last 'token' an extra or just whitespace?\n");
    getToken(&tokBegin, &tokEnd);
    if ( *tokBegin != '\0' )
    {
        /* No token expected, but one is found. */
        results[0] = TOO_MANY;
        return 0;
    }

    return 1;
}

