#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RomNumMath.h"

#define MAX_ORDER_NUMS 9
#define MAX_ORDER_THOU 3

// All Roman numerals in increasing magnitude order
const char numerals[NUM_ORDERS][MAX_ORDER_NUMS][5] = 
                                        {{"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
                                         {"X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
                                         {"C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
                                         {"M", "MM", "MMM", "", "", "", "", "", ""}};

// Base Roman numerals in increasing magnitude order
const char base_numerals[NUM_ORDERS][NUM_BASES] = {{'I', 'V', 'X'},
                                                   {'X', 'L', 'C'},
                                                   {'C', 'D', 'M'},
                                                   {'M', '\0', '\0'}};

// Initialize global_debugging to default setting
bool global_debugging = false;

//////
//  BaseCounter is a struct containing base numeral counter
//  variables.
//////
struct BaseCounter
{
    // Base numeral counter for order of magnitude
    int x1;
    int x5;
    int x10;

    // Subtractive form counter
    int sub;
};

//////
//  StrHolder is a struct for dividing up the a string into
//  chunks based on orders of magnitude and its pointer and
//  length variables are used for addition, subtraction and
//  comparison operations by various functions.
//////
struct StrHolder
{
    // Negation
    bool negative;

    // String lengths
    int orderLen[NUM_ORDERS];

    // Substring pointers
    char *sPtr[NUM_ORDERS][NUM_PTRS];

    // Order pointers
    char *orderPtr[NUM_ORDERS];

    // Main string pointer
    char *mainStr;
};

//////
//  debug_printf() is a wrapper function for printf that dynamically turns off
//  the printf output when not in debugger mode
//////
void debug_printf(const char* format, ...)
{
    if (global_debugging)
    {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }
}

//////
//  isCleanValidString() returns true/false whether a string represents a
//  clean and valid Roman numeral. Combining two previous functions
//  into one.
//////
bool isCleanValidString (const char *s)
{
    // Check for NULL string
    if (s == NULL)
    {
        return false;
    }

    // String is clean if it only contains valid Roman numeral characters
    char *validRomanChars = "IVXLCMD";
    debug_printf("Checking if '%s' is clean...\n", s);
    for (int i = 0; i < strlen(s); i++)
    {
        debug_printf("i = %d, c = '%c'\n", i, s[i]);
        if (strchr(validRomanChars, s[i]) == NULL)
        {
            // Invalid char found
            debug_printf("ERROR! Found invalid character '%c'\n", s[i]);
            return false;
        }
    }
    // All chars are valid
    debug_printf("'%s' is clean!\n", s);

    int length = strlen(s);
    debug_printf("'%s' at 0x%p, Length = %d\n", s, s, length);

    StrHolder vH;
    attachHolder(s, &vH, false);

    // Quick check - if any order lengths are negative
    // If so, they are out of order and hence invalid
    if (vH.orderLen[ONES] < 0 ||
        vH.orderLen[TENS] < 0 ||
        vH.orderLen[HUNS] < 0 ||
        vH.orderLen[THOU] < 0)
    {
        return false;
    }

    // Validation counters
    int ordersPresent = 0;
    int ordersValid = 0;

    if (vH.orderPtr[ONES] != NULL)
    {
        // Validate
        ordersPresent++;
        if (validateOrder(ONES, &vH))
        {
            ordersValid++;
        }
    }
    if (vH.orderPtr[TENS] != NULL)
    {
        // Validate
        ordersPresent++;
        if (validateOrder(TENS, &vH))
        {
            ordersValid++;
        }
    }
    if (vH.orderPtr[HUNS] != NULL)
    {
        // Validate
        ordersPresent++;
        if (validateOrder(HUNS, &vH))
        {
            ordersValid++;
        }
    }
    if (vH.orderPtr[THOU] != NULL)
    {
        // Validate
        ordersPresent++;
        if (validateOrder(THOU, &vH))
        {
            ordersValid++;
        }
    }

    // For each order of magnitude present there must be a valid string representation
    if (ordersPresent == ordersValid)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//////
//  addition() adds two Roman numerals and returns a pointer
//  to the output buffer or NULL if something goes wrong
//////
char *addition (const char *as, const char *bs, char *cs)
{
    char onesStr[BUFRSIZE];
    char tensStr[BUFRSIZE];
    char hunsStr[BUFRSIZE];
    char thouStr[BUFRSIZE];

    // Clear buffers
    memset(cs, '\0', BUFRSIZE);
    memset(onesStr, '\0', BUFRSIZE);
    memset(tensStr, '\0', BUFRSIZE);
    memset(hunsStr, '\0', BUFRSIZE);
    memset(thouStr, '\0', BUFRSIZE);

    debug_printf("\n\n\naddition buffers clear!\n");

    // String holders
    StrHolder aH;
    StrHolder bH;
    StrHolder cH;

    // Negation indicators
    bool asNegative = false;
    bool bsNegative = false;
    bool finalNegative = false;

    // Check for negative input numbers
    if (strchr(as, '-') != NULL)
    {
        asNegative = true;
        debug_printf("Parameter #1, %s, is negative\n", as);
    }
    if (strchr(bs, '-') != NULL)
    {
        bsNegative = true;
        debug_printf("Parameter #2, %s, is negative\n", bs);
    }

    if (asNegative && !bsNegative)
    {
        debug_printf("Reversing parameters and switching from addition to subtraction\n");
        // Negative symbol is ignored for the subtraction
        return subtraction(bs, as+1, cs);
    }
    else if (!asNegative && bsNegative)
    {
        debug_printf("Switching from addition to subtraction\n");
        // Negative symbol is ignored for the subtraction
        return subtraction(as, bs+1, cs);
    }
    else if (asNegative && bsNegative)
    {
        debug_printf("Handling addition with double negative\n");
        // Negative symbol is ignored until the end
        finalNegative = true;

        // Attach holders
        attachHolder(as+1, &aH, asNegative);
        attachHolder(bs+1, &bH, bsNegative);
        attachHolder(cs, &cH, true);
    }
    else
    {   // !asNegative && !bsNegative
        // Attach holders
        attachHolder(as, &aH, asNegative);
        attachHolder(bs, &bH, bsNegative);
        attachHolder(cs, &cH, false);
    }

    // Check input
    if (!isCleanValidString(aH.mainStr))
    {
        debug_printf("ERROR! %s is not a proper Roman numeral.\n", aH.mainStr);
        return NULL;
    }

    if (!isCleanValidString(bH.mainStr))
    {
        debug_printf("ERROR! %s is not a proper Roman numeral.\n", bH.mainStr);
        return NULL;
    }

    // Add two Roman numeral strings by orders of magnitude
    // carrying surplus to higher orders
    bool carry = false;

    carry = addOrder(&aH, &bH, onesStr, ONES, false);
    debug_printf("Ones (%s)", onesStr);
    debug_printf("%s\n", carry?" with carry":"");

    carry = addOrder(&aH, &bH, tensStr, TENS, carry);
    debug_printf("Tens (%s)", tensStr);
    debug_printf("%s\n", carry?" with carry":"");

    carry = addOrder(&aH, &bH, hunsStr, HUNS, carry);
    debug_printf("Hundreds (%s)", hunsStr);
    debug_printf("%s\n", carry?" with carry":"");

    carry = addOrder(&aH, &bH, thouStr, THOU, carry);
    debug_printf("Thousands (%s)", thouStr);
    debug_printf("%s\n", carry?" with carry":"");

    // Check output
    debug_printf("OUTPUT>> %s|%s|%s|%s\n ", thouStr, hunsStr, tensStr, onesStr);
    strncat(cH.mainStr, thouStr, strlen(thouStr));
    strncat(cH.mainStr, hunsStr, strlen(hunsStr));
    strncat(cH.mainStr, tensStr, strlen(tensStr));
    strncat(cH.mainStr, onesStr, strlen(onesStr));

    if (isCleanValidString(cH.mainStr))
    {
        if (!finalNegative)
        {
            debug_printf("CLEAN and VALID +++ %s strlen(%zu)\n", cs, strlen(cs));
            debug_printf("%s + %s = %s\n\n", as, bs, strlen(cs)!=0?cs:"nihil");
        }
        else
        {
            prependStr(cs, '-');
            debug_printf("CLEAN and VALID +++ %s strlen(%zu)\n", cs, strlen(cs));
            debug_printf("%s + %s = %s\n\n", as, bs, strlen(cs)!=0?cs:"nihil");
        }

        return cs;
    }
    else
    {
        debug_printf("ERROR! NOT CLEAN/VALID --- '%s' strlen(%zu)\n", cs, strlen(cs));
        debug_printf("%s + %s != %s\n\n", as, bs, strlen(cs)!=0?cs:"nihil");
        return NULL;
    }
}

//////
//  subtraction() finds the difference between two Roman numerals and returns a pointer
//  to the output buffer or NULL if something goes wrong. It can handle negative output.
//////
char *subtraction (const char *as, const char *bs, char *cs)
{
    char onesStr[BUFRSIZE];
    char tensStr[BUFRSIZE];
    char hunsStr[BUFRSIZE];
    char thouStr[BUFRSIZE];

    // Clear buffers
    memset(cs, '\0', BUFRSIZE);
    memset(onesStr, '\0', BUFRSIZE);
    memset(tensStr, '\0', BUFRSIZE);
    memset(hunsStr, '\0', BUFRSIZE);
    memset(thouStr, '\0', BUFRSIZE);

    debug_printf("\n\n\nsubtraction buffers clear!\n");

    // Compare strings and check input
    int val = romStrCmp(as, bs);
    bool negativeOutput = false;

    if (val > 0)
    {   // Expected norm
        debug_printf("CMP %s > %s %d\n", as, bs, val);
    }
    else if (val == BAD_NUMERAL_A || val == BAD_NUMERAL_B)
    {   // Bad input
        return NULL;
    }
    else if (val < 0)
    {   // Negative output
        debug_printf("CMP %s < %s %d\n", as, bs, val);
        negativeOutput = true;
    }
    else // (val == 0)
    {   // Equality means zero or nihil outcome
        debug_printf("CMP %s == %s %d\n", as, bs, val);
    }

    // String holders
    StrHolder aH;
    StrHolder bH;
    StrHolder cH;

    // Negation indicators
    bool asNegative = false;
    bool bsNegative = false;

    // Check for negative input numbers
    if (strchr(as, '-') != NULL)
    {
        asNegative = true;
        debug_printf("Parameter #1, %s, is negative\n", as);
    }
    if (strchr(bs, '-') != NULL)
    {
        bsNegative = true;
        debug_printf("Parameter #2, %s, is negative\n", bs);
    }

    if (asNegative && !bsNegative)
    {
        debug_printf("Switching from subtraction to addition of two negatives\n");
        // Negative symbol is ignored until the end
        addition(as+1, bs, cs);
        prependStr(cs, '-');
        return cs;
    }
    else if (!asNegative && bsNegative)
    {
        debug_printf("Switching from subtraction to addition\n");
        // Negative symbol is ignored for the addition
        return addition(as, bs+1, cs);
    }
    else if (asNegative && bsNegative)
    {
        debug_printf("Handling subtraction with double negative\n");
        // Subtracting a negative is like adding a positive so just switch the parameters

        // Attach holders
        if (!negativeOutput)
        {
            attachHolder(bs+1, &aH, !bsNegative);
            attachHolder(as+1, &bH, !asNegative);
            attachHolder(cs, &cH, false);
        }
        else
        {
            attachHolder(as+1, &aH, !bsNegative);
            attachHolder(bs+1, &bH, !asNegative);
            attachHolder(cs, &cH, false);
        }
    }
    else
    {   // !asNegative && !bsNegative
        // Attach holders
        if (!negativeOutput)
        {
            attachHolder(as, &aH, asNegative);
            attachHolder(bs, &bH, bsNegative);
            attachHolder(cs, &cH, false);
        }
        else
        {
            attachHolder(bs, &aH, asNegative);
            attachHolder(as, &bH, bsNegative);
            attachHolder(cs, &cH, false);
        }
    }

    // Subtract two Roman numeral strings by orders of magnitude
    // borrowing from higher orders as necessary
    bool borrow = false;

    borrow = subOrder(&aH, &bH, onesStr, ONES, false);
    debug_printf("Ones (%s)", onesStr);
    debug_printf("%s\n", borrow?" with borrow":"");

    borrow = subOrder(&aH, &bH, tensStr, TENS, borrow);
    debug_printf("Tens (%s)", tensStr);
    debug_printf("%s\n", borrow?" with borrow":"");

    borrow = subOrder(&aH, &bH, hunsStr, HUNS, borrow);
    debug_printf("Hundreds (%s)", hunsStr);
    debug_printf("%s\n", borrow?" with borrow":"");

    borrow = subOrder(&aH, &bH, thouStr, THOU, borrow);
    debug_printf("Thousands (%s)", thouStr);
    debug_printf("%s\n", borrow?" with borrow":"");

    // Check output
    debug_printf("OUTPUT>> %s|%s|%s|%s\n ", thouStr, hunsStr, tensStr, onesStr);
    strncat(cH.mainStr, thouStr, strlen(thouStr));
    strncat(cH.mainStr, hunsStr, strlen(hunsStr));
    strncat(cH.mainStr, tensStr, strlen(tensStr));
    strncat(cH.mainStr, onesStr, strlen(onesStr));

    if (isCleanValidString(cH.mainStr))
    {
        debug_printf("CLEAN and VALID +++ %s strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
        debug_printf("%s - %s = %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");

        if (negativeOutput)
        {
            prependStr(cs, '-');
            debug_printf("%s - %s = %s\n\n", bH.mainStr, aH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        }
        else
        {
            debug_printf("%s - %s = %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        }

        return cs;
    }
    else
    {
        debug_printf("ERROR! NOT CLEAN/VALID --- '%s' strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
        debug_printf("%s - %s != %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        return NULL;
    }
}

//////
//  attachHolder() associates an existing StrHolder struct with an existing Roman numeral string by analyzing
//  the string and attaching pointers and string length markers to string in order to facilitate
//  addition, subtraction and comparison operations that occur at various orders of magnitude
//  within the strings
//////
void attachHolder(const char *s, StrHolder *sh, bool isNegative)
{
    // Check for NULLs
    if (s == NULL || sh == NULL)
    {
        return;
    }

    // Negative string?
    sh->negative = isNegative;

    // Initialize everything
    // Attach string
    sh->mainStr = (char *) s;

    // String lengths
    sh->orderLen[ONES] = 0;
    sh->orderLen[TENS] = 0;
    sh->orderLen[HUNS] = 0;
    sh->orderLen[THOU] = 0;

    // Substring pointers
    for (int i = 0; i < NUM_ORDERS; i++)
    {
        for (int j = 0; j < NUM_PTRS; j++)
        {
            sh->sPtr[i][j] = NULL;
        }
    }

    // Order pointers
    sh->orderPtr[ONES] = NULL;
    sh->orderPtr[TENS] = NULL;
    sh->orderPtr[HUNS] = NULL;
    sh->orderPtr[THOU] = NULL;

    // Specify pointers to existing numerals
    // ONES
    // Attempt to attach pointer to the X1 (I) base numeral and find the start of this order of magnitude within the string
    if ((sh->sPtr[ONES][P1] = strchr(sh->mainStr, base_numerals[ONES][X1])) != NULL)
    {
        debug_printf("%c at 0x%p\n", base_numerals[ONES][X1], sh->sPtr[ONES][P1]);
        // We have ones
        sh->orderPtr[ONES] = sh->sPtr[ONES][P1];
    }
    // Attempt to attach pointer to the X5 (V) base numeral and find the start of this order of magnitude within the string
    attachP5Ptrs(ONES, sh);

    // TENS
    // Attempt to attach pointer to the X1 (X) base numeral and find the start of this order of magnitude within the string
    attachP1Ptrs(TENS, sh);
    // Attempt to attach pointer to the X5 (L) base numeral and find the start of this order of magnitude within the string
    attachP5Ptrs(TENS, sh);

    // HUNDREDS
    // Attempt to attach pointer to the X1 (C) base numeral and find the start of this order of magnitude within the string
    attachP1Ptrs(HUNS, sh);
    // Attempt to attach pointer to the X5 (D) base numeral and find the start of this order of magnitude within the string
    attachP5Ptrs(HUNS, sh);

    // THOUSANDS
    // Attempt to attach pointer to the X1 (M) base numeral and find the start of this order of magnitude within the string
    attachP1Ptrs(THOU, sh);

    if (sh->orderPtr[ONES] != NULL)
    {
        // orderPtr[ONES] points to first char in ones part of the string
        sh->orderLen[ONES] = strlen(sh->orderPtr[ONES]);
        debug_printf("orderPtr[ONES] at 0x%p, length = %d\n", sh->orderPtr[ONES], sh->orderLen[ONES]);
    }
    if (sh->orderPtr[TENS] != NULL)
    {
        // orderPtr[TENS] points to first char in tens part of the string
        if (sh->orderPtr[ONES] != NULL)
        {
            sh->orderLen[TENS] = sh->orderPtr[ONES] - sh->orderPtr[TENS];
        }
        else
        {
            sh->orderLen[TENS] = strlen(sh->orderPtr[TENS]);
        }
        debug_printf("orderPtr[TENS] at 0x%p, length = %d\n", sh->orderPtr[TENS], sh->orderLen[TENS]);
    }
    if (sh->orderPtr[HUNS] != NULL)
    {
        // orderPtr[HUNS] points to first char in hundreds part of the string
        if (sh->orderPtr[TENS] != NULL)
        {
            sh->orderLen[HUNS] = sh->orderPtr[TENS] - sh->orderPtr[HUNS];
        }
        else if (sh->orderPtr[ONES] != NULL)
        {
            sh->orderLen[HUNS] = sh->orderPtr[ONES] - sh->orderPtr[HUNS];
        }
        else
        {
            sh->orderLen[HUNS] = strlen(sh->orderPtr[HUNS]);
        }
        debug_printf("orderPtr[HUNS] at 0x%p, length = %d\n", sh->orderPtr[HUNS], sh->orderLen[HUNS]);
    }
    if (sh->orderPtr[THOU] != NULL)
    {
        // orderPtr[THOU] points to first char in thousands part of the string
        if (sh->orderPtr[HUNS] != NULL)
        {
            sh->orderLen[THOU] = sh->orderPtr[HUNS] - sh->orderPtr[THOU];
        }
        else if (sh->orderPtr[TENS] != NULL)
        {
            sh->orderLen[THOU] = sh->orderPtr[TENS] - sh->orderPtr[THOU];
        }
        else if (sh->orderPtr[ONES] != NULL)
        {
            sh->orderLen[THOU] = sh->orderPtr[ONES] - sh->orderPtr[THOU];
        }
        else
        {
            sh->orderLen[THOU] = strlen(sh->orderPtr[THOU]);
        }
        debug_printf("orderPtr[THOU] at 0x%p, length = %d\n", sh->orderPtr[THOU], sh->orderLen[THOU]);
    }
}

//////
//  isStringSubtractive() returns true if a Roman numeral string is subtractive meaning
//  a character of a lower order precedes a higher order character to diminsh
//  the numeric value of that character by a specific amount. Eg IV or XC. This
//  function only checks the first two characters in the string so it is typically
//  called by a function that has already divided a string into its respective orders
//  of magnitude.
//////
bool isStringSubtractive(const char *s)
{
    // Check for empty string or single char (neither can be subtractive)
    if (s == NULL)
    {
        return false;
    }
    else if (strlen(s) == 1)
    {
        return false;
    }

    // Show string where sequence starts and get first character
    char firstChar = s[0];

    if (strlen(s) >= 2)
    {
        // For  'I'
        switch (firstChar) {
            case 'I':
                if (s[1] == 'V')
                {
                    return true /*4*/;
                }
                else if (s[1] == 'X')
                {
                    return true /*9*/;
                }
                else
                {
                    return false /*0*/;
                }
                break;

            case 'X':
                if (s[1] == 'L')
                {
                    return true /*40*/;
                }
                else if (s[1] == 'C')
                {
                    return true /*90*/;
                }
                else
                {
                    return false /*0*/;
                }
                break;

            case 'C':
                if (s[1] == 'D')
                {
                    return true /*400*/;
                }
                else if (s[1] == 'M')
                {
                    return true /*900*/;
                }
                else
                {
                    return false /*0*/;
                }
                break;

            default:
                debug_printf("... not a valid subtractive string!\n");
                return false /*0*/;
                break;
        }
    }
    return false;
}

//////
//  addOrder() adds two strings and lower order carried surplus, saving the result into
//  another string in the current order of magnitude and returning true there is a resultant
//  surplus to be carried to a higher order of magnitude.
//////
bool addOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool carriedOver)
{
    bool carry = false;

    // If strings are empty, no further processing is necessary
    if (aH->orderLen[order] == 0 && bH->orderLen[order] == 0 && !carriedOver)
    {
        return false;
    }

    debug_printf("\n---------------- > addOrder %d\n", order);

    if (aH->orderPtr[order] != NULL && bH->orderPtr[order] == NULL && !carriedOver)
    {   // Just copy the AAA string
        strncat(cStr, aH->orderPtr[order], aH->orderLen[order]);
    }
    else if (bH->orderPtr[order] != NULL && aH->orderPtr[order] == NULL && !carriedOver)
    {   // Just copy the BBB string
        strncat(cStr, bH->orderPtr[order], bH->orderLen[order]);
    }
    else /* if (bH->orderPtr[order] != NULL && aH->orderPtr[order] != NULL) */
    {
        BaseCounter bC;
        bC.x1  = carriedOver?1:0;
        bC.x5  = 0;
        bC.x10 = 0;
        bC.sub = 0;

        debug_printf("\n\nx10 = %d, x5 = %d, x1 = %d, sub = %d\n", bC.x10, bC.x5, bC.x1, bC.sub);

        // Track subtractive forms
        if (isStringSubtractive(aH->orderPtr[order]))
        {
            bC.sub++;
        }
        if (isStringSubtractive(bH->orderPtr[order]))
        {
            bC.sub++;
        }

        // Tally characters in current order of magnitude in first string
        tallyChar(aH, order, &bC);

        // Tally characters in current order of magnitude in second string
        tallyChar(bH, order, &bC);

        debug_printf("pre tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", bC.x10, bC.x5, bC.x1, bC.sub);

        // Handle subtractive forms
        handleSubtractives(&bC);

        // Reset bC.sub
        bC.sub = 0;

        // Adjust x1 tally
        adjustX1Tally(&bC);

        // Adjust x5 tally
        adjustX5Tally(&bC);

        debug_printf("post tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", bC.x10, bC.x5, bC.x1, bC.sub);

        if (order == THOU && (bC.x10 > 0 || bC.x5 > 0))
        {
            // Only values up to MMMCMXCIX (3999) are supported
            debug_printf("ERROR! Fatal Error: Only values up to MMMCMXCIX (3999) are supported\n");
            return false;
        }

        // Post process to handle carrying and output result
        postProcAddOrder(order, &bC, &carry, cStr);
    }
    debug_printf("addOrder: got %s, returning carry:%d\n", cStr, carry);

    return carry;
}

//////
//  subOrder() subtracts two strings possibly borrowing from higher order, saving the result into
//  another string in the current order of magnitude and returning true if was required from a higher order
//  value
//////
bool subOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool borrowedFrom)
{
    bool borrow = false;

    // If strings are empty, no further processing is necessary
    if (aH->orderLen[order] == 0 && bH->orderLen[order] == 0 && !borrowedFrom)
    {
        return false;
    }

    debug_printf("\n---------------- > subOrder %d\n", order);

    // If B string is empty and the lower order of magnitude subtraction didn't require a borrow then just copy A
    if (aH->orderPtr[order] != NULL && bH->orderPtr[order] == NULL && !borrowedFrom)
    {
        strncat(cStr, aH->orderPtr[order], aH->orderLen[order]);
    }
    else
    {
        BaseCounter aBC;
        aBC.x1  = borrowedFrom?-1:0;
        aBC.x5  = 0;
        aBC.x10 = 0;
        aBC.sub = 0;

        BaseCounter bBC;
        bBC.x1  = 0;
        bBC.x5  = 0;
        bBC.x10 = 0;
        bBC.sub = 0;

        debug_printf("\n\nA tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aBC.x10, aBC.x5, aBC.x1, aBC.sub);

        // Track subtractive form of A
        if (isStringSubtractive(aH->orderPtr[order]))
        {
            aBC.sub = true;
            debug_printf("'%s' is subtractive\n", aH->orderPtr[order]);
        }

        // Tally characters in current order of magnitude in first string
        tallyChar(aH, order, &aBC);

        debug_printf("post A tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aBC.x10, aBC.x5, aBC.x1, aBC.sub);

        // Track subtractive form of B
        if (isStringSubtractive(bH->orderPtr[order]))
        {
            bBC.sub = true;
            debug_printf("'%s' is subtractive\n", bH->orderPtr[order]);
        }

        // Tally characters in current order of magnitude in first string
        tallyChar(bH, order, &bBC);

        debug_printf("post B tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", bBC.x10, bBC.x5, bBC.x1, bBC.sub);

        // Convert subtractive forms into non-subtractive forms to simplify subtraction
        // eg. IV: X(0), V(1), I(1), sub(1) = IV = (V - I) = IIIII - I = IIII = X(0), V(0), I(4), sub(0)
        //     IX: X(1), V(0), I(1), sub(1) = IX = (X - I) = V + (V - I) = V + (IIII) = X(0), V(1), I(4), sub(0)
        if (aBC.sub)
        {
            // Convert from subtractive forms
            borrow = convertFromSubtractive(&aBC);
            debug_printf("aBC conv... x10 = %d, x5 = %d, x1 = %d, sub = %d, borrow = %c\n", aBC.x10, aBC.x5, aBC.x1, aBC.sub, borrow?'t':'f');
        }

        if (bBC.sub)
        {
            // Convert from subtractive forms
            borrow = convertFromSubtractive(&bBC);
            debug_printf("bBC conv... x10 = %d, x5 = %d, x1 = %d, sub = %d, borrow = %c\n", bBC.x10, bBC.x5, bBC.x1, bBC.sub, borrow?'t':'f');
        }

        // Subtract B from A
        aBC.x10 -= bBC.x10;
        aBC.x5  -= bBC.x5;
        aBC.x1  -= bBC.x1;
        debug_printf("subtraction  x10 = %d, x5 = %d, x1 = %d, sub = %d, borrow = %c\n", aBC.x10, aBC.x5, aBC.x1, aBC.sub, borrow?'t':'f');

        // Reconcile negative numbers
        borrow = reconcileNegatives(&aBC);

        debug_printf("reconciled     x10 = %d, x5 = %d, x1 = %d, sub = %d, borrow = %c\n", aBC.x10, aBC.x5, aBC.x1, aBC.sub, borrow?'t':'f');

        // Convert back to subtractive form
        convertToSubtractive(&aBC);

        debug_printf("final form   x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aBC.x10, aBC.x5, aBC.x1, aBC.sub);

        // Return Output
        if (order == THOU && (aBC.x10 > 0 || aBC.x5 > 0))
        {
            // Only values up to MMMCMXCIX (3999) are supported
            debug_printf("ERROR! Fatal Error: Only values up to MMMCMXCIX (3999) are supported\n");
            return false;
        }

        // Post process to handle carrying and output result
        postProcSubOrder(order, &aBC, cStr);

    }
    debug_printf("subOrder: got %s, returning borrow:%d\n", cStr, borrow);

    return borrow;
}

//////
//  romStrCmpSH() compares two StrHolder associated strings in a specific order of magnitude
//  returning 1, -1 or 0 if the first is greater than, less than or equal to the second
//  It does no string validation of its own so that must be handled by a calling function or higher
//  It is recursive so it goes deeper and deeper into the strings until it gets a definitive answer
//////
int romStrCmpSH(StrHolder *aH, StrHolder *bH, OrderType order)
{
    debug_printf("--> romStrCmpSH: %s, %s %d\n", aH->orderPtr[order], bH->orderPtr[order], order);

    // Compare strings from the highest to the lowest order
    if (aH->orderPtr[order] != NULL && bH->orderPtr[order] == NULL)
    {
        if (!aH->negative)
        {
            debug_printf("%s > %s\n", aH->mainStr, bH->mainStr);
            return 1;
        }
        else
        {
            debug_printf("%s < %s\n", aH->mainStr, bH->mainStr);
            return -1;
        }
    }
    else if (aH->orderPtr[order] == NULL && bH->orderPtr[order] != NULL)
    {
        if (!bH->negative)
        {
            debug_printf("%s < %s\n", aH->mainStr, bH->mainStr);
            return -1;
        }
        else
        {
            debug_printf("%s > %s\n", aH->mainStr, bH->mainStr);
            return 1;
        }
    }
    else if (aH->orderPtr[order] == NULL && bH->orderPtr[order] == NULL)
    {
        if (order != ONES)
        {
            return romStrCmpSH(aH, bH, order-1);
        }
        else
        {
            return 0;
        }
    }
    else if (aH->orderPtr[order] != NULL && bH->orderPtr[order] != NULL)
    {   // Relative values used for ranking and thus comparing numerals are negative if the number is negative
        int aNumRelVal = romNumRelVal(aH->orderPtr[order], aH->orderLen[order], order);
        if (aH->negative)
        {
            aNumRelVal = -1 * (aNumRelVal+1);
        }

        int bNumRelVal = romNumRelVal(bH->orderPtr[order], bH->orderLen[order], order);
        if (bH->negative)
        {
            bNumRelVal = -1 * (bNumRelVal+1);
        }

        if (aNumRelVal > bNumRelVal)
        {
            debug_printf("%s > %s\n", aH->mainStr, bH->mainStr);
            return 1;
        }
        else if (aNumRelVal < bNumRelVal)
        {
            debug_printf("%s < %s\n", aH->mainStr, bH->mainStr);
            return -1;
        }
        else // (aNumRelVal == bNumRelVal)
        {
            if (order != ONES)
            {
                return romStrCmpSH(aH, bH, order-1);
            }
            else
            {
                return 0;
            }
        }
    }
    return 0;
}

//////
//  romStrCmp() compares two strings returning 1, -1 or 0 if the first is greater than,
//  less than or equal to the second. It does validate the strings first will return numbers
//  indicating that a string was invalid and which one was the problem. It associates StrHolders
//  with the validated strings and calls romStrCmpSH() to do the work starting at the highest
//  order of magnitude
//////
int romStrCmp(const char *as, const char *bs)
{
    debug_printf("\n--> romStrCmp: %s, %s\n", as, bs);

    // Negation indicators
    bool asNegative = false;
    bool bsNegative = false;

    // Check for negative input numbers
    if (strchr(as, '-') != NULL)
    {
        asNegative = true;
        debug_printf("Parameter #1, %s, is negative\n", as);
    }
    if (strchr(bs, '-') != NULL)
    {
        bsNegative = true;
        debug_printf("Parameter #2, %s, is negative\n", bs);
    }

    // Check input
    if (!isCleanValidString(asNegative?as+1:as))
    {
        debug_printf("ERROR! %s is not a proper Roman numeral.\n", as);
        return BAD_NUMERAL_A;
    }

    if (!isCleanValidString(bsNegative?bs+1:bs))
    {
        debug_printf("ERROR! %s is not a proper Roman numeral.\n", bs);
        return BAD_NUMERAL_B;
    }

    // String holders
    StrHolder aH;
    StrHolder bH;

    // Attach holders
    attachHolder(asNegative?as+1:as, &aH, asNegative);
    attachHolder(bsNegative?bs+1:bs, &bH, bsNegative);

    return romStrCmpSH(&aH, &bH, THOU);
}

//////
//  romNumRelVal() compares a string with every standard numeral in a specific order of magnitude.
//  until it finds a match to return the relative value for the input string within that order
//  of magnitude. If no match is found it returns -1.
//////
int romNumRelVal(const char *str, int orderLength, OrderType order)
{
    for (int i = 0; i < 9; i++)
    {
        if (strncmp(str, numerals[order][i], orderLength) == 0)
        {
            debug_printf("romNumRelVal(%s, %d, %s) = %d\n", str, order, numerals[order][i], i);
            return i;
        }
    }

    debug_printf("romNumRelVal(%s, %d) returns -1\n", str, order);
    return -1;
}

//////
//  prependStr() puts a char at the front of a string. Currently used for putting a '-' at 
//  the front of a Roman numeral to make it a negative number. It returns a pointer to the
//  new string.
//////
char *prependStr(char *str, char c)
{
    int length = 0;
    if ((length = strlen(str)) != 0)
    {
        memmove(str + 1, str, length + 1);
    }
    str[0] = c;
    return str;
}

//////
//  tallyChar() counts the number of base characters of a particular order of magnitude
//  a string. Base characters are the x1, x5 and x10 in an order eg. for ones that would
//  be x1=I, x5=V and x10=X.
//////
void tallyChar(StrHolder *sH, OrderType order, BaseCounter *bC)
{
    // Tally characters in first string
    for (int i = 0; i < sH->orderLen[order]; i++)
    {
        if (sH->orderPtr[order][i] == base_numerals[order][X10])
        {
            bC->x10++;
        }
        else if (sH->orderPtr[order][i] == base_numerals[order][X5])
        {
            bC->x5++;
        }
        else if (sH->orderPtr[order][i] == base_numerals[order][X1])
        {
            bC->x1++;
        }
    }
}

//////
// handleSubtractives() handles numeral forms that place a lower numeral before a higher one
// When removing a subtractive it has to take another non-subtractive
// with it. eg IV + IV = (V - I) + (V - I) = IIII + IIII = (IIII + I) + III = VIII
// the second V had to be split up to satisfy both subtractives
//             IX + IX = (X-I) + (X-I) = X + X - (I + I) = X + (X - II) = X + VIII = XVIII
// the second X had to be split up to satisfy both subtractives
//////
void handleSubtractives(BaseCounter *bC)
{
    for (int i = 0; i < bC->sub; i++)
    {
        if (bC->x1 >= 2)
        {
            bC->x1 -= 2;
        }
        else if (bC->x5 != 0)
        {
            bC->x5--;
            bC->x1 += 3; // sacrifice one+one for the subtractive
        }
        else if (bC->x10 != 0)
        {
            bC->x10--;   // split X into VV
            bC->x5++;    // one V goes here
            bC->x1 += 3; // sacrifice one+one for the subtractive
        }
        debug_printf("sub looping... x10 = %d, x5 = %d, x1 = %d, sub = %d\n", bC->x10, bC->x5, bC->x1, bC->sub);
    }
}

//////
// convertFromSubtractive() converts subtractive forms into non-subtractive forms to simplify subtraction
// eg. IV: X(0), V(1), I(1), sub(true) = IV = (V - I) = IIIII - I = IIII = X(0), V(0), I(4), sub(false)
//     IX: X(1), V(0), I(1), sub(true) = IX = (X - I) = V + (V - I) = V + (IIII) = X(0), V(1), I(4), sub(false)
//////
bool convertFromSubtractive(BaseCounter *bC)
{
    bool borrow = false;
    if (bC->x5 == 1)
    {
        bC->x5--;
        bC->x1 += 3; // sacrifice one+one for the subtractive
    }
    else if (bC->x10 == 1)
    {
        bC->x10--;   // split X into VV
        bC->x5++;    // one V goes here
        bC->x1 += 3; // sacrifice one+one for the subtractive
    }
    else if (bC->x1 == 1)
    {   // have a -I situation
        bC->x10++;
        borrow = true;
    }
    bC->sub = false;
    return borrow;
}

//////
// adjustX1Tally() adjusts the count of x1 base numerals to facilitate
// conversion back to subtractive forms
// eg. if x1 = 4, 5, 6 we get IV, V VI or XL, L, LX or CD, D, DC
//////
void adjustX1Tally(BaseCounter *bC)
{
    switch (bC->x1) {
        case 4:
            bC->x1 = 1;
            bC->x5++;
            bC->sub = 1;
            break;

        case 5:
            bC->x1 = 0;
            bC->x5++;
            break;

        case 6:
            bC->x1 = 1;
            bC->x5++;
            break;

        default:
            break;
    }
}

//////
// adjustX5Tally() adjusts the count of x5 base numerals due to
// restrictions because there can only be one x5 in a Roman numeral
// eg. if x5 = 2, 3 we get X, XV or C, CL or M, MD
//////
void adjustX5Tally(BaseCounter *bC)
{
    switch (bC->x5) {
        case 2:
            bC->x5 = 0;
            bC->x10++;
            break;

        case 3:
            bC->x5 = 1;
            bC->x10++;
            break;

        default:
            break;
    }
}

//////
// postProcAddOrder() outputs carrying status and fills out result
// string for current order of magnitude
//////
void postProcAddOrder(OrderType order, const BaseCounter *bC, bool *outCarry, char *outStr)
{
    if (bC->sub != 1)
    {
        debug_printf("output should be '");
        if (bC->x10 == 1)
        {
            *outCarry = true;
        }

        if (bC->x5 == 1)
        {
            debug_printf("%c", base_numerals[order][X5]);
            strncat(outStr, &(base_numerals[order][X5]), 1);
        }

        for (int i = 0; i < bC->x1; i++)
        {
            debug_printf("%c", base_numerals[order][X1]);
            strncat(outStr, &(base_numerals[order][X1]), 1);
        }
        debug_printf("'\n");
    }
    else if (bC->sub == 1 && bC->x5 != 0)
    {
        debug_printf("output should be '");
        if (bC->x10 == 1)
        {
            *outCarry = true;
        }

        if (bC->x1 == 1)
        {
            debug_printf("%c", base_numerals[order][X1]);
            strncat(outStr, &(base_numerals[order][X1]), 1);
        }

        if (bC->x5 == 1)
        {
            debug_printf("%c", base_numerals[order][X5]);
            strncat(outStr, &(base_numerals[order][X5]), 1);
        }
        debug_printf("'\n");
    }
    else
    {
        debug_printf("output should be '");
        if (bC->x1 == 1)
        {
            debug_printf("%c", base_numerals[order][X1]);
            strncat(outStr, &(base_numerals[order][X1]), 1);
        }

        if (bC->x10 == 1)
        {
            debug_printf("%c", base_numerals[order][X10]);
            strncat(outStr, &(base_numerals[order][X10]), 1);
        }
        debug_printf("'\n");
    }
}

//////
// postProcSubOrder() fills out result string for current order of magnitude
//////
void postProcSubOrder(OrderType order, const BaseCounter *bC, char *outStr)
{
    if (!bC->sub)
    {
        debug_printf("output should be '");
        if (bC->x5 == 1)
        {
            debug_printf("%c", base_numerals[order][X5]);
            strncat(outStr, &(base_numerals[order][X5]), 1);
        }

        for (int i = 0; i < bC->x1; i++)
        {
            debug_printf("%c", base_numerals[order][X1]);
            strncat(outStr, &(base_numerals[order][X1]), 1);
        }
        debug_printf("'\n");
    }
    else
    {
        debug_printf("output should be '");
        if (bC->x1 == 1)
        {
            debug_printf("%c", base_numerals[order][X1]);
            strncat(outStr, &(base_numerals[order][X1]), 1);
        }

        if (bC->x10 == 1)
        {
            debug_printf("%c", base_numerals[order][X10]);
            strncat(outStr, &(base_numerals[order][X10]), 1);
        }

        if (bC->x5 == 1)
        {
            debug_printf("%c", base_numerals[order][X5]);
            strncat(outStr, &(base_numerals[order][X5]), 1);
        }
        debug_printf("'\n");
    }
}

//////
// reconcileNegatives() figures out what to do with negative x1 and x5
// base numeral counts and returns whether we must borrow from a higher
// order of magnitude
//////
bool reconcileNegatives(BaseCounter *bC)
{
    bool borrow = false;
    // Reconcile negative numbers
    if (bC->x1 < 0)
    {
        if (bC->x5 >= 1)
        {
            bC->x5--;
            bC->x1 += 5;
        }
        else if (bC->x10 >= 1)
        {
            bC->x10--;
            bC->x5 = 1;
            bC->x1 += 5;
        }
        else
        {   // Must borrow
            bC->x10++;
            bC->x1 = bC->x10*10 + bC->x5*5 + bC->x1;
            bC->x10 = 0;
            bC->x5 = 0;
            borrow = true;
        }
    }
    else if (bC->x5 < 0)
    {   // need to borrow next order of magitude and calculate remainder eg. 10-8=2
        if (bC->x10 == 0)
        {
            bC->x10++;
            borrow = true;
        }
        bC->x1 = bC->x10*10 + bC->x5*5 + bC->x1;
        bC->x5 = 0;
    }
    return borrow;
}

//////
// convertToSubtractive() converts a non-subtractive base numeral count back
// to a subtractive form
// eg. IV: X(0), V(0), I(4), sub(0) = IIII = IIIII - I = (V - I) = IV = X(0), V(1), I(1), sub(1)
//     IX: X(0), V(1), I(4), sub(0) = V + (IIII) = V + (V - I) = (V + V) - I = (X - I) = IX
//                                  = X(1), V(0), I(1), sub(1)
// if x1 = 4, 5, 6, 9 we get IV, V, VI, IX or XL, L, LX, XC or CD, D, DC, CM
//////
void convertToSubtractive(BaseCounter *bC)
{
    switch (bC->x1) {
        case 4:
            bC->x1 = 1;
            bC->x5 = 1;
            bC->sub = true;
            break;

        case 5:
            bC->x1 = 0;
            bC->x5 = 1;
            break;

        case 6:
            bC->x1 = 1;
            bC->x5 = 1;
            break;

        case 7:
            bC->x1 = 2;
            bC->x5 = 1;
            break;

        case 8:
            bC->x1 = 3;
            bC->x5 = 1;
            break;

        case 9:
            bC->x1 = 1;
            bC->x10 = 1;
            bC->sub = true;
            break;

        default:
            break;
    }
}

//////
// attachP1Ptrs() locates and attaches pointers to the X1 base numerals
// in a string (X, C, M) as well as locating the position of this order
// of magnitude within the string
//////
void attachP1Ptrs(OrderType order, StrHolder *sH)
{
    if ((sH->sPtr[order][P1] = strchr(sH->mainStr, base_numerals[order][X1])) != NULL)
    {
        debug_printf("%c at 0x%p\n", base_numerals[order][X1], sH->sPtr[order][P1]);
        // Check if there is a subtractive
        if (sH->orderPtr[order-1] != NULL)
        {
            if (sH->orderPtr[order-1] > sH->sPtr[order][P1])
            {
                // Set order pointer for this order of magnitude
                sH->orderPtr[order] = sH->sPtr[order][P1];
            }
            else
            {
                // We have a subtractive form
                debug_printf("subtractive\n");
            }
        }
        else
        {
            // Set order pointer for this order of magnitude
            sH->orderPtr[order] = sH->sPtr[order][P1];
        }
    }
}

//////
// attachP5Ptrs() locates and attaches pointers to the X5 base numerals
// in a string (V, L, D) as well as locating the position of this order
// of magnitude within the string
//////
void attachP5Ptrs(OrderType order, StrHolder *sH)
{
    if ((sH->sPtr[order][P5] = strchr(sH->mainStr, base_numerals[order][X5])) != NULL)
    {
        debug_printf("%c at 0x%p\n", base_numerals[order][X5], sH->sPtr[order][P5]);
        if (sH->orderPtr[order] != NULL)
        {
            if (sH->sPtr[order][P5] < sH->orderPtr[order])
            {
                // Set order pointer for this order of magnitude
                sH->orderPtr[order] = sH->sPtr[order][P5];
            }
        }
        else
        {
            // Set order pointer for this order of magnitude
            sH->orderPtr[order] = sH->sPtr[order][P5];
        }
    }
}

//////
// validateOrder() validates a portion of a string within the specified order of magnitude
//////
bool validateOrder(OrderType order, StrHolder *sH)
{
    int i;
    if (order == THOU)
    {
        for (i = 0; i < MAX_ORDER_THOU; i++)
        {
            // Validate
            if ((strncmp(sH->orderPtr[order], numerals[order][i], sH->orderLen[order]) == 0))
            {
                return true;
            }
        }
    }
    else
    {
        for (i = 0; i < MAX_ORDER_NUMS; i++)
        {
            // Validate
            if ((strncmp(sH->orderPtr[order], numerals[order][i], sH->orderLen[order]) == 0))
            {
                return true;
            }
        }
    }
    return false;
}
