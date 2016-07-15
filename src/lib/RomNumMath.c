#define HIDE_PRINTF

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RomNumMath.h"

#ifdef HIDE_PRINTF
#define printf(fmt, ...) (0)
#endif

// All Roman numerals in increasing magnitude order
const char numerals[NUM_ORDERS][9][5] = {{"I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"},
                                         {"X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"},
                                         {"C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"},
                                         {"M", "MM", "MMM", "", "", "", "", "", ""}};

// Represents bad input strings
const int BAD_NUMERAL_A = -999;
const int BAD_NUMERAL_B = -998;

//////
//  StrHolder is a struct for dividing up the a string into
//  chunks based on orders of magnitude and its pointer and
//  length variables are used for addition, subtraction and
//  comparison operations by various functions.
//////
struct StrHolder
{
    // String lengths
    int orderLen[NUM_ORDERS];

    // Substring pointers
    char *iPtr;
    char *vPtr;
    char *xPtr;
    char *lPtr;
    char *cPtr;
    char *dPtr;
    char *mPtr;

    // Order pointers
    char *orderPtr[NUM_ORDERS];

    // Main string pointer
    char *mainStr;
};

//////
//  intToRomNum() takes an integer value and returns a Roman numeral string
//  This function was created for sanity checking and is not used by the RomNumCalcApp
//////
char *intToRomNum(int num, char *s)
{
    // Create a Roman numeral string in an existing buffer
    // from an integer < 4000
    if (s == NULL)
    {
        printf("String buffer is empty! Exiting...\n");
        return NULL;
    }

    if (num > 3999)
    {
        printf("Invalid input (>3999)! Exiting...\n");
        return NULL;
    }
    s[0] = '\0';
    int thousands = num / 1000;
    int hundreds = (num - (thousands * 1000)) / 100;
    int tens = (num - (thousands * 1000) - (hundreds * 100)) / 10;
    int ones = (num - (thousands * 1000) - (hundreds * 100) - (tens * 10));
    int i = 0;
    printf("num = %d, T = %d, H = %d, t = %d, o = %d\n", num, thousands, hundreds, tens, ones);
    if (thousands > 0)
    {
        for (i = 0; i < thousands; i++)
        {
            strcat(s, "M");
        }
    }
    if (hundreds > 0)
    {
        if (hundreds < 4)
        {
            for (i = 0; i < hundreds; i++)
            {
                strcat(s, "C");
            }
        }
        else if (hundreds == 4)
        {
            strcat(s, "CD");
        }
        else if (hundreds == 5)
        {
            strcat(s, "D");
        }
        else if (hundreds == 6)
        {
            strcat(s, "DC");
        }
        else if (hundreds == 7)
        {
            strcat(s, "DCC");
        }
        else if (hundreds == 8)
        {
            strcat(s, "DCCC");
        }
        else if (hundreds == 9)
        {
            strcat(s, "CM");
        }
    }
    if (tens > 0)
    {
        if (tens < 4)
        {
            for (i = 0; i < tens; i++)
            {
                strcat(s, "X");
            }
        }
        else if (tens == 4)
        {
            strcat(s, "XL");
        }
        else if (tens == 5)
        {
            strcat(s, "L");
        }
        else if (tens == 6)
        {
            strcat(s, "LX");
        }
        else if (tens == 7)
        {
            strcat(s, "LXX");
        }
        else if (tens == 8)
        {
            strcat(s, "LXXX");
        }
        else if (tens == 9)
        {
            strcat(s, "XC");
        }
    }
    if (ones > 0)
    {
        if (ones < 4)
        {
            for (i = 0; i < ones; i++)
            {
                strcat(s, "I");
            }
        }
        else if (ones == 4)
        {
            strcat(s, "IV");
        }
        else if (ones == 5)
        {
            strcat(s, "V");
        }
        else if (ones == 6)
        {
            strcat(s, "VI");
        }
        else if (ones == 7)
        {
            strcat(s, "VII");
        }
        else if (ones == 8)
        {
            strcat(s, "VIII");
        }
        else if (ones == 9)
        {
            strcat(s, "IX");
        }
    }
    printf("num = %d, s = %s\n", num, s);
    return s;
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
    printf("Checking if '%s' is clean...\n", s);
    for (int i = 0; i < strlen(s); i++)
    {
        printf("i = %d, c = '%c'\n", i, s[i]);
        if (strchr(validRomanChars, s[i]) == NULL)
        {
            // Invalid char found
            fprintf(stderr, "Found invalid character '%c'\n", s[i]);
            return false;
        }
    }
    // All chars are valid
    printf("'%s' is clean!\n", s);

    int length = strlen(s);
    printf("'%s' at 0x%p, Length = %d\n", s, s, length);

    StrHolder vH;
    attachHolder(s, &vH);

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

    bool onesValid = false;
    bool tensValid = false;
    bool hunsValid = false;
    bool thouValid = false;

    if (vH.orderPtr[ONES] != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.orderPtr[ONES], "I",    vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "II",   vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "III",  vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "IV",   vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "V",    vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "VI",   vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "VII",  vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "VIII", vH.orderLen[ONES]) == 0) ||
            (strncmp(vH.orderPtr[ONES], "IX",   vH.orderLen[ONES]) == 0))
        {
            onesValid = true;
            ordersValid++;
        }
    }
    if (vH.orderPtr[TENS] != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.orderPtr[TENS], "X",    vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "XX",   vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "XXX",  vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "XL",   vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "L",    vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "LX",   vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "LXX",  vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "LXXX", vH.orderLen[TENS]) == 0) ||
            (strncmp(vH.orderPtr[TENS], "XC",   vH.orderLen[TENS]) == 0))
        {
            tensValid = true;
            ordersValid++;
        }
    }
    if (vH.orderPtr[HUNS] != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.orderPtr[HUNS], "C",    vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "CC",   vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "CCC",  vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "CD",   vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "D",    vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "DC",   vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "DCC",  vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "DCCC", vH.orderLen[HUNS]) == 0) ||
            (strncmp(vH.orderPtr[HUNS], "CM",   vH.orderLen[HUNS]) == 0))
        {
            hunsValid = true;
            ordersValid++;
        }
    }
    if (vH.orderPtr[THOU] != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.orderPtr[THOU], "M",   vH.orderLen[THOU]) == 0) ||
            (strncmp(vH.orderPtr[THOU], "MM",  vH.orderLen[THOU]) == 0) ||
            (strncmp(vH.orderPtr[THOU], "MMM", vH.orderLen[THOU]) == 0))
        {
            thouValid = true;
            ordersValid++;
        }
    }

    // For each order of magnitude present there is a valid string representation
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

    printf("\n\n\naddition buffers clear!\n");

    // String holders
    StrHolder aH;
    StrHolder bH;
    StrHolder cH;

    // Attach holders
    attachHolder(as, &aH);
    attachHolder(bs, &bH);
    attachHolder(cs, &cH);

    // Check input
    if (!isCleanValidString(aH.mainStr))
    {
        fprintf(stderr, "%s is not a proper Roman numeral.\n", aH.mainStr);
        return NULL;
    }

    if (!isCleanValidString(bH.mainStr))
    {
        fprintf(stderr, "%s is not a proper Roman numeral.\n", bH.mainStr);
        return NULL;
    }

    // Add two Roman numeral strings by orders of magnitude
    // carrying surplus to higher orders
    bool carry = false;

    carry = addOrder(&aH, &bH, onesStr, ONES, false);
    printf("Ones (%s)", onesStr);
    printf("%s\n", carry?" with carry":"");

    carry = addOrder(&aH, &bH, tensStr, TENS, carry);
    printf("Tens (%s)", tensStr);
    printf("%s\n", carry?" with carry":"");

    carry = addOrder(&aH, &bH, hunsStr, HUNS, carry);
    printf("Hundreds (%s)", hunsStr);
    printf("%s\n", carry?" with carry":"");

    carry = addOrder(&aH, &bH, thouStr, THOU, carry);
    printf("Thousands (%s)", thouStr);
    printf("%s\n", carry?" with carry":"");

    // Check output
    printf("OUTPUT>> %s|%s|%s|%s\n ", thouStr, hunsStr, tensStr, onesStr);
    strncat(cH.mainStr, thouStr, strlen(thouStr));
    strncat(cH.mainStr, hunsStr, strlen(hunsStr));
    strncat(cH.mainStr, tensStr, strlen(tensStr));
    strncat(cH.mainStr, onesStr, strlen(onesStr));

    if (isCleanValidString(cH.mainStr))
    {
        printf("CLEAN and VALID +++ %s strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
        printf("%s + %s = %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        return cs;
    }
    else
    {
        fprintf(stderr, "NOT CLEAN/VALID --- '%s' strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
        printf("%s + %s != %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
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

    printf("\n\n\nsubtraction buffers clear!\n");

    // Compare strings and check input
    int val = romStrCmp(as, bs);
    bool negativeOutput = false;

    if (val > 0)
    {   // Expected norm
        printf("CMP %s > %s %d\n", as, bs, val);
    }
    else if (val == BAD_NUMERAL_A || val == BAD_NUMERAL_B)
    {   // Bad input
        return NULL;
    }
    else if (val < 0)
    {   // Negative output
        printf("CMP %s < %s %d\n", as, bs, val);
        negativeOutput = true;
    }
    else // (val == 0)
    {   // Equality means zero or nihil outcome
        printf("CMP %s == %s %d\n", as, bs, val);
    }

    // String holders
    StrHolder aH;
    StrHolder bH;
    StrHolder cH;

    // Attach holders
    if (!negativeOutput)
    {
        attachHolder(as, &aH);
        attachHolder(bs, &bH);
    }
    else
    {   // Switch AAA and BBB for simple subtraction
        attachHolder(bs, &aH);
        attachHolder(as, &bH);
    }

    attachHolder(cs, &cH);

    // Subtract two Roman numeral strings by orders of magnitude
    // borrowing from higher orders as necessary
    bool borrow = false;

    borrow = subOrder(&aH, &bH, onesStr, ONES, false);
    printf("Ones (%s)", onesStr);
    printf("%s\n", borrow?" with borrow":"");

    borrow = subOrder(&aH, &bH, tensStr, TENS, borrow);
    printf("Tens (%s)", tensStr);
    printf("%s\n", borrow?" with borrow":"");

    borrow = subOrder(&aH, &bH, hunsStr, HUNS, borrow);
    printf("Hundreds (%s)", hunsStr);
    printf("%s\n", borrow?" with borrow":"");

    borrow = subOrder(&aH, &bH, thouStr, THOU, borrow);
    printf("Thousands (%s)", thouStr);
    printf("%s\n", borrow?" with borrow":"");

    // Check output
    printf("OUTPUT>> %s|%s|%s|%s\n ", thouStr, hunsStr, tensStr, onesStr);
    strncat(cH.mainStr, thouStr, strlen(thouStr));
    strncat(cH.mainStr, hunsStr, strlen(hunsStr));
    strncat(cH.mainStr, tensStr, strlen(tensStr));
    strncat(cH.mainStr, onesStr, strlen(onesStr));

    if (isCleanValidString(cH.mainStr))
    {
        printf("CLEAN and VALID +++ %s strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
        printf("%s - %s = %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");

        if (negativeOutput)
        {
            size_t len = strlen(cs);
            memmove(cs + 1, cs, strlen(cs) + 1);
            cs[0] = '-';
            printf("%s - %s = %s\n\n", bH.mainStr, aH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        }
        else
        {
            printf("%s - %s = %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        }

        return cs;
    }
    else
    {
        fprintf(stderr, "NOT CLEAN/VALID --- '%s' strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
        printf("%s - %s != %s\n\n", aH.mainStr, bH.mainStr, strlen(cH.mainStr)!=0?cH.mainStr:"nihil");
        return NULL;
    }
}

//////
//  attachHolder() associates an existing StrHolder struct with an existing Roman numeral string by analyzing
//  the string and attaching pointers and string length markers to string in order to facilitate
//  addition, subtraction and comparison operations that occur at various orders of magnitude
//  within the strings
//////
void attachHolder(const char *s, StrHolder *sh)
{
    // Check for NULLs
    if (s == NULL || sh == NULL)
    {
        return;
    }

    // Initialize everything
    // Attach string
    sh->mainStr = (char *) s;

    // String lengths
    sh->orderLen[ONES] = 0;
    sh->orderLen[TENS] = 0;
    sh->orderLen[HUNS] = 0;
    sh->orderLen[THOU] = 0;

    // Substring pointers
    sh->iPtr = NULL;
    sh->vPtr = NULL;
    sh->xPtr = NULL;
    sh->lPtr = NULL;
    sh->cPtr = NULL;
    sh->dPtr = NULL;
    sh->mPtr = NULL;

    // Order pointers
    sh->orderPtr[ONES] = NULL;
    sh->orderPtr[TENS] = NULL;
    sh->orderPtr[HUNS] = NULL;
    sh->orderPtr[THOU] = NULL;

    // Specify pointers to existing numerals
    // ONES
    if ((sh->iPtr = strchr(sh->mainStr, 'I')) != NULL)
    {
        printf("I at 0x%p\n", sh->iPtr);
        // We have ones
        sh->orderPtr[ONES] = sh->iPtr;
    }
    if ((sh->vPtr = strchr(sh->mainStr, 'V')) != NULL)
    {
        printf("V at 0x%p\n", sh->vPtr);
        if (sh->orderPtr[ONES] != NULL)
        {
            if (sh->vPtr < sh->orderPtr[ONES])
            {
                // We have ones
                sh->orderPtr[ONES] = sh->vPtr;
            }
        }
        else
        {
            // We have ones
            sh->orderPtr[ONES] = sh->vPtr;
        }
    }

    // TENS
    if ((sh->xPtr = strchr(sh->mainStr, 'X')) != NULL)
    {
        printf("X at 0x%p\n", sh->xPtr);
        // Check if there is a subtractive
        if (sh->orderPtr[ONES] != NULL)
        {
            if (sh->orderPtr[ONES] > sh->xPtr)
            {
                // We have tens
                sh->orderPtr[TENS] = sh->xPtr;
            }
            else
            {
                printf("subtractive\n");
            }
        }
        else
        {
            // We have tens
            sh->orderPtr[TENS] = sh->xPtr;
        }
    }
    if ((sh->lPtr = strchr(sh->mainStr, 'L')) != NULL)
    {
        printf("L at 0x%p\n", sh->lPtr);
        if (sh->orderPtr[TENS] != NULL)
        {
            if (sh->lPtr < sh->orderPtr[TENS])
            {
                // We have tens
                sh->orderPtr[TENS] = sh->lPtr;
            }
        }
        else
        {
            // We have tens
            sh->orderPtr[TENS] = sh->lPtr;
        }
    }

    // HUNDREDS
    if ((sh->cPtr = strchr(sh->mainStr, 'C')) != NULL)
    {
        printf("C at 0x%p\n", sh->cPtr);
        // Check if there is a subtractive
        if (sh->orderPtr[TENS] != NULL)
        {
            if (sh->orderPtr[TENS] > sh->cPtr)
            {
                // We have hundreds
                sh->orderPtr[HUNS] = sh->cPtr;
            }
            else
            {
                printf("subtractive\n");
            }
        }
        else
        {
            // We have hundreds
            sh->orderPtr[HUNS] = sh->cPtr;
        }
    }
    if ((sh->dPtr = strchr(sh->mainStr, 'D')) != NULL)
    {
        printf("D at 0x%p\n", sh->dPtr);
        if (sh->orderPtr[HUNS] != NULL)
        {
            if (sh->dPtr < sh->orderPtr[HUNS])
            {
                // We have hundreds
                sh->orderPtr[HUNS] = sh->dPtr;
            }
        }
        else
        {
            // We have hundreds
            sh->orderPtr[HUNS] = sh->dPtr;
        }
    }

    // THOUSANDS
    if ((sh->mPtr = strchr(sh->mainStr, 'M')) != NULL)
    {
        printf("M at 0x%p\n", sh->mPtr);
        // Check if there is a subtractive
        if (sh->orderPtr[HUNS] != NULL)
        {
            if (sh->orderPtr[HUNS] > sh->mPtr)
            {
                // We have thousands
                sh->orderPtr[THOU] = sh->mPtr;
            }
            else
            {
                printf("subtractive\n");
            }
        }
        else
        {
            // We have thousands
            sh->orderPtr[THOU] = sh->mPtr;
        }
    }

    if (sh->orderPtr[ONES] != NULL)
    {
        // orderPtr[ONES] points to first char in ones part of the string
        sh->orderLen[ONES] = strlen(sh->orderPtr[ONES]);
        printf("orderPtr[ONES] at 0x%p, length = %d\n", sh->orderPtr[ONES], sh->orderLen[ONES]);
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
        printf("orderPtr[TENS] at 0x%p, length = %d\n", sh->orderPtr[TENS], sh->orderLen[TENS]);
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
        printf("orderPtr[HUNS] at 0x%p, length = %d\n", sh->orderPtr[HUNS], sh->orderLen[HUNS]);
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
        printf("orderPtr[THOU] at 0x%p, length = %d\n", sh->orderPtr[THOU], sh->orderLen[THOU]);
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
                printf("... not a valid subtractive string!\n");
                return false /*0*/;
                break;
        }
    }
}

//////
//  addOrder() adds two strings and lower order carried surplus, saving the result into
//  another string in the current order of magnitude and returning true there is a resultant
//  surplus to be carried to a higher order of magnitude.
//////
bool addOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool carriedOver)
{
    int x10Count    = 0;
    char x1         = '\0';
    char x5         = '\0';
    char x10        = '\0';
    bool carry      = false;

    // Setup units
    switch (order) {
        case ONES:
            x1  = 'I';
            x5  = 'V';
            x10 = 'X';
            break;

        case TENS:
            x1  = 'X';
            x5  = 'L';
            x10 = 'C';
            break;

        case HUNS:
            x1  = 'C';
            x5  = 'D';
            x10 = 'M';
            break;

        case THOU:
            x1  = 'M';
            break;

        default:
            printf("incorrect order identifier '%d' exiting...", order);
            return false;
            break;
    }

    // If strings are empty, no further processing is necessary
    if (aH->orderLen[order] == 0 && bH->orderLen[order] == 0 && !carriedOver)
    {
        return false;
    }

    printf("\n---------------- > addOrder %d\n", order);

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
        int i = 0;
        int x1Count = carriedOver?1:0;
        int x5Count = 0;
        int subCount = 0;
        printf("\n\nx10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

        // Track subtractive forms
        if (isStringSubtractive(aH->orderPtr[order]))
        {
            subCount++;
        }
        if (isStringSubtractive(bH->orderPtr[order]))
        {
            subCount++;
        }

        // Tally characters
        for (i = 0; i < aH->orderLen[order]; i++)
        {
            if (aH->orderPtr[order][i] == x10)
            {
                x10Count++;
            }
            else if (aH->orderPtr[order][i] == x5)
            {
                x5Count++;
            }
            else if (aH->orderPtr[order][i] == x1)
            {
                x1Count++;
            }
        }
        for (i = 0; i < bH->orderLen[order]; i++)
        {
            if (bH->orderPtr[order][i] == x10)
            {
                x10Count++;
            }
            else if (bH->orderPtr[order][i] == x5)
            {
                x5Count++;
            }
            else if (bH->orderPtr[order][i] == x1)
            {
                x1Count++;
            }
        }
        printf("pre tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

        // Handle subtractives. When removing a subtractive it has to take another non-subtractive
        // with it. eg IV + IV = (V - I) + (V - I) = IIII + IIII = (IIII + I) + III = VIII
        // the second V had to be split up to satisfy both subtractives
        //             IX + IX = XXII = XVIII
        // the second X had to be split up to satisfy both subtractives
        for (i = 0; i < subCount; i++)
        {
            if (x1Count >= 2)
            {
                x1Count -= 2;
            }
            else if (x5Count != 0)
            {
                x5Count--;
                x1Count += 3; // sacrifice one+one for the subtractive
            }
            else if (x10Count != 0)
            {
                x10Count--;   // split X into VV
                x5Count++;    // one V goes here
                x1Count += 3; // sacrifice one+one for the subtractive
            }
            printf("sub looping... x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);
        }

        subCount = 0;

        // Adjust tally
        // if x1Count = 4, 5, 6 we get IV, V VI or XL, L, LX or CD, D, DC
        switch (x1Count) {
            case 4:
                x1Count = 1;
                x5Count++;
                subCount = 1;
                break;

            case 5:
                x1Count = 0;
                x5Count++;
                break;

            case 6:
                x1Count = 1;
                x5Count++;
                break;

            default:
                break;
        }

        // if x5Count = 2, 3 we get X, XV or C, CL or M, MD
        switch (x5Count) {
            case 2:
                x5Count = 0;
                x10Count++;
                break;

            case 3:
                x5Count = 1;
                x10Count++;
                break;

            default:
                break;
        }

        printf("post tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

        if (order == THOU && (x10Count > 0 || x5Count > 0))
        {
            // Only values up to MMMCMXCIX (3999) are supported
            fprintf(stderr, "Fatal Error: Only values up to MMMCMXCIX (3999) are supported\n");
            return false;
        }

        if (subCount != 1)
        {
            printf("output should be '");
            if (x10Count == 1)
            {
                carry = true;
            }

            if (x5Count == 1)
            {
                printf("%c", x5);
                strncat(cStr, &x5, 1);
            }

            for (i = 0; i < x1Count; i++)
            {
                printf("%c", x1);
                strncat(cStr, &x1, 1);
            }
            printf("'\n");
        }
        else if (subCount == 1 && x5Count != 0)
        {
            printf("output should be '");
            if (x10Count == 1)
            {
                carry = true;
            }

            if (x1Count == 1)
            {
                printf("%c", x1);
                strncat(cStr, &x1, 1);
            }

            if (x5Count == 1)
            {
                printf("%c", x5);
                strncat(cStr, &x5, 1);
            }
            printf("'\n");
        }
        else
        {
            printf("output should be '");
            if (x1Count == 1)
            {
                printf("%c", x1);
                strncat(cStr, &x1, 1);
            }

            if (x10Count == 1)
            {
                printf("%c", x10);
                strncat(cStr, &x10, 1);
            }
            printf("'\n");
        }
    }
    printf("addOrder: got %s, returning carry:%d\n", cStr, carry);

    return carry;
}

//////
//  subOrder() subtracts two strings possibly borrowing from higher order, saving the result into
//  another string in the current order of magnitude and returning true if was required from a higher order
//  value
//////
bool subOrder(StrHolder *aH, StrHolder *bH, char *cStr, OrderType order, bool borrowedFrom)
{
    char x1         = '\0';
    char x5         = '\0';
    char x10        = '\0';
    bool borrow     = false;

    switch (order) {
        case ONES:
            x1  = 'I';
            x5  = 'V';
            x10 = 'X';
            break;

        case TENS:
            x1  = 'X';
            x5  = 'L';
            x10 = 'C';
            break;

        case HUNS:
            x1  = 'C';
            x5  = 'D';
            x10 = 'M';
            break;

        case THOU:
            x1  = 'M';
            break;

        default:
            printf("incorrect order identifier '%d' exiting...", order);
            return false;
            break;
    }

    // If strings are empty, no further processing is necessary
    if (aH->orderLen[order] == 0 && bH->orderLen[order] == 0 && !borrowedFrom)
    {
        return false;
    }

    printf("\n---------------- > subOrder %d\n", order);

    // If B string is empty and the lower order of magnitude subtraction didn't require a borrow then just copy A
    if (aH->orderPtr[order] != NULL && bH->orderPtr[order] == NULL && !borrowedFrom)
    {
        strncat(cStr, aH->orderPtr[order], aH->orderLen[order]);
    }
    else
    {
        int i = 0;

        int aX1Count = borrowedFrom?-1:0;
        int aX5Count = 0;
        int aX10Count = 0;
        bool aSub = 0;

        int bX1Count = 0;
        int bX5Count = 0;
        int bX10Count = 0;
        bool bSub = 0;

        printf("\n\nA tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aX10Count, aX5Count, aX1Count, aSub);

        // Track subtractive forms
        if (isStringSubtractive(aH->orderPtr[order]))
        {
            aSub = true;
            printf("'%s' is subtractive\n", aH->orderPtr[order]);
        }

        // Tally characters for A
        for (i = 0; i < aH->orderLen[order]; i++)
        {
            if (aH->orderPtr[order][i] == x10)
            {
                aX10Count++;
            }
            else if (aH->orderPtr[order][i] == x5)
            {
                aX5Count++;
            }
            else if (aH->orderPtr[order][i] == x1)
            {
                aX1Count++;
            }
        }
        printf("post A tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aX10Count, aX5Count, aX1Count, aSub);

        // Track subtractive forms
        if (isStringSubtractive(bH->orderPtr[order]))
        {
            bSub = true;
            printf("'%s' is subtractive\n", bH->orderPtr[order]);
        }

        // Tally characters for B (to be subtracted)
        for (i = 0; i < bH->orderLen[order]; i++)
        {
            if (bH->orderPtr[order][i] == x10)
            {
                bX10Count++;
            }
            else if (bH->orderPtr[order][i] == x5)
            {
                bX5Count++;
            }
            else if (bH->orderPtr[order][i] == x1)
            {
                bX1Count++;
            }
        }
        printf("post B tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", bX10Count, bX5Count, bX1Count, bSub);

        // Convert subtractive forms into non-subtractive forms to simplify subtraction
        // eg. IV: X(0), V(1), I(1), sub(1) = IV = (V - I) = IIIII - I = IIII = X(0), V(0), I(4), sub(0)
        //     IX: X(1), V(0), I(1), sub(1) = IX = (X - I) = V + (V - I) = V + (IIII) = X(0), V(1), I(4), sub(0)
        if (aSub)
        {
            if (aX5Count == 1)
            {
                aX5Count--;
                aX1Count += 3; // sacrifice one+one for the subtractive
            }
            else if (aX10Count == 1)
            {
                aX10Count--;   // split X into VV
                aX5Count++;    // one V goes here
                aX1Count += 3; // sacrifice one+one for the subtractive
            }
            else if (aX1Count == 1)
            {   // have a -I situation
                aX10Count++;
                borrow = true;
            }
            aSub = false;
            printf("aSub conv... x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aX10Count, aX5Count, aX1Count, aSub);
        }

        if (bSub)
        {
            if (bX5Count == 1)
            {
                bX5Count--;
                bX1Count += 3; // sacrifice one+one for the subtractive
            }
            else if (bX10Count == 1)
            {
                bX10Count--;   // split X into VV
                bX5Count++;    // one V goes here
                bX1Count += 3; // sacrifice one+one for the subtractive
            }
            else if (bX1Count == 1)
            {   // have a -I situation
                bX10Count++;
                borrow = true;
            }
            bSub = false;
            printf("bSub conv... x10 = %d, x5 = %d, x1 = %d, sub = %d\n", bX10Count, bX5Count, bX1Count, bSub);
        }

        // Subtract B from A
        aX10Count -= bX10Count;
        aX5Count  -= bX5Count;
        aX1Count  -= bX1Count;
        printf("subtraction  x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aX10Count, aX5Count, aX1Count, aSub);

        // Reconcile negative numbers
        if (aX1Count < 0)
        {
            if (aX5Count >= 1)
            {
                aX5Count--;
                aX1Count += 5;
            }
            else if (aX10Count >= 1)
            {
                aX10Count--;
                aX5Count = 1;
                aX1Count += 5;
            }
            else
            {   // Must borrow
                aX10Count++;
                aX1Count = aX10Count*10 + aX5Count*5 + aX1Count;
                aX5Count = 0;
                borrow = true;
            }
        }
        else if (aX5Count < 0)
        {   // need to borrow next order of magitude and calculate remainder eg. 10-8=2
            if (aX10Count == 0)
            {
                aX10Count++;
                borrow = true;
            }
            aX1Count = aX10Count*10 + aX5Count*5 + aX1Count;
            aX5Count = 0;
        }
        printf("adjusted     x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aX10Count, aX5Count, aX1Count, aSub);

        // Convert back to subtractive form
        // eg. IV: X(0), V(0), I(4), sub(0) = IIII = IIIII - I = (V - I) = IV = X(0), V(1), I(1), sub(1)
        //     IX: X(0), V(1), I(4), sub(0) = V + (IIII) = V + (V - I) = (V + V) - I = (X - I) = IX
        //                                  = X(1), V(0), I(1), sub(1)
        // if aX1Count = 4, 5, 6, 9 we get IV, V, VI, IX or XL, L, LX, XC or CD, D, DC, CM
        switch (aX1Count) {
            case 4:
                aX1Count = 1;
                aX5Count = 1;
                aSub = true;
                break;

            case 5:
                aX1Count = 0;
                aX5Count = 1;
                break;

            case 6:
                aX1Count = 1;
                aX5Count = 1;
                break;

            case 9:
                aX1Count = 1;
                aX10Count = 1;
                aSub = true;
                break;

            default:
                break;
        }

        printf("final form   x10 = %d, x5 = %d, x1 = %d, sub = %d\n", aX10Count, aX5Count, aX1Count, aSub);

        // Return Output
        if (order == THOU && (aX10Count > 0 || aX5Count > 0))
        {
            // Only values up to MMMCMXCIX (3999) are supported
            fprintf(stderr, "Fatal Error: Only values up to MMMCMXCIX (3999) are supported\n");
            return false;
        }

        if (!aSub)
        {
            printf("output should be '");
            if (aX5Count == 1)
            {
                printf("%c", x5);
                strncat(cStr, &x5, 1);
            }

            for (i = 0; i < aX1Count; i++)
            {
                printf("%c", x1);
                strncat(cStr, &x1, 1);
            }
            printf("'\n");
        }
        else
        {
            printf("output should be '");
            if (aX1Count == 1)
            {
                printf("%c", x1);
                strncat(cStr, &x1, 1);
            }

            if (aX10Count == 1)
            {
                printf("%c", x10);
                strncat(cStr, &x10, 1);
            }

            if (aX5Count == 1)
            {
                printf("%c", x5);
                strncat(cStr, &x5, 1);
            }
            printf("'\n");
        }
    }
    printf("subOrder: got %s, returning borrow:%d\n", cStr, borrow);

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
    printf("--> romStrCmpSH: %s, %s %d\n", aH->orderPtr[order], bH->orderPtr[order], order);

    // Compare strings from the highest to the lowest order
    if (aH->orderPtr[order] != NULL && bH->orderPtr[order] == NULL)
    {
        printf("%s > %s\n", aH->mainStr, bH->mainStr);
        return 1;
    }
    else if (aH->orderPtr[order] == NULL && bH->orderPtr[order] != NULL)
    {
        printf("%s < %s\n", aH->mainStr, bH->mainStr);
        return -1;
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
    {
        int aNumRelVal = romNumRelVal(aH->orderPtr[order], aH->orderLen[order], order);
        int bNumRelVal = romNumRelVal(bH->orderPtr[order], bH->orderLen[order], order);

        if (aNumRelVal > bNumRelVal)
        {
            printf("%s > %s\n", aH->mainStr, bH->mainStr);
            return 1;
        }
        else if (aNumRelVal < bNumRelVal)
        {
            printf("%s < %s\n", aH->mainStr, bH->mainStr);
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
    printf("\n--> romStrCmp: %s, %s\n", as, bs);
    // Check input
    if (!isCleanValidString(as))
    {
        fprintf(stderr, "%s is not a proper Roman numeral.\n", as);
        return BAD_NUMERAL_A;
    }

    if (!isCleanValidString(bs))
    {
        fprintf(stderr, "%s is not a proper Roman numeral.\n", bs);
        return BAD_NUMERAL_B;
    }

    // String holders
    StrHolder aH;
    StrHolder bH;

    // Attach holders
    attachHolder(as, &aH);
    attachHolder(bs, &bH);

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
            printf("romNumRelVal(%s, %d, %s) = %d\n", str, order, numerals[order][i], i);
            return i;
        }
    }

    printf("romNumRelVal(%s, %d) returns -1\n", str, order);
    return -1;
}

