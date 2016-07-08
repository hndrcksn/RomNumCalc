#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "RomNumMath.h"

struct RomNumeral
{
    int value;
    char *nString;
};

RomNumeral *numeralCreate(int numValue, char *numString)
{
    RomNumeral *rN = malloc(sizeof(RomNumeral));

    if (rN != NULL)
    {
        rN->value = numValue;
        rN->nString = numString;
        return rN;
    }
    else
    {
        return NULL;
    }
}

struct StrHolder
{
    // String lengths
    int onesLen;
    int tensLen;
    int hunsLen;
    int thouLen;

    // Substring pointers
    char *iPtr;
    char *vPtr;
    char *xPtr;
    char *lPtr;
    char *cPtr;
    char *dPtr;
    char *mPtr;

    // Order pointers
    char *onesPtr;
    char *tensPtr;
    char *hunsPtr;
    char *thouPtr;

    // Main string pointer
    char *mainStr;
};

void numeralReplace(RomNumeral *rN, int newValue, char *newStrPtr)
{
    rN->value = newValue;
    rN->nString = newStrPtr;
}

int numeralValue(RomNumeral *rN)
{
    return rN->value;
}

char *numeralString(RomNumeral *rN)
{
    return rN->nString;
}

void numeralDestroy(RomNumeral *rN)
{
    free(rN);
    return;
}

bool char0Is_I(RomNumeral *rN)
{
    return !strncmp(&(rN->nString)[0], "I", 1);
}

bool char1Is_V(RomNumeral *rN)
{
    return !strncmp(&(rN->nString)[1], "V", 1);
}

bool numeralStringIsClean(RomNumeral *rN)
{
    return stringIsClean(rN->nString);
}

bool stringIsClean(const char *s)
{
    // String is clean if it only contains valid Roman numeral characters
    char *validRomanChars = "IVXLCMD";
    printf("Checking if '%s' is clean...\n", s);
    for (int i = 0; i < strlen(s); i++)
    {
        printf("i = %d, c = '%c'\n", i, s[i]);
        if (strchr(validRomanChars, s[i]) == NULL)
        {
            // Invalid char found
            printf("Invalid char = '%c'\n", s[i]);
            return false;
        }
    }
    // All chars are valid
    printf("'%s' is clean!\n", s);
    return true;
}

int sequenceInRomNumeralIsValid(RomNumeral *rN)
{
    return sequenceInStringIsValid(rN->nString);
}

int sequenceInStringIsValid(const char *s)
{
    // Seeking uniform sequence of chars (III, XXX, etc.)
    // Check for NULL
    if (s == NULL)
    {
        return 0;
    }

    // Show string where sequence starts and get first character
    printf("Checking if '%s' is valid...\n", s);
    char firstChar = s[0];
    int maxChar = 0;

    // Assign maximum number of chars per sequence
    switch (firstChar) {
        case 'V' :
        case 'L' :
        case 'D' :
            maxChar = 1;
            break;

        case 'I' :
        case 'X' :
        case 'C' :
            maxChar = 3;
            break;

        case 'M' :
            maxChar = 3;
            break;

        default:
            // Oops invalid character found!
            printf("Invalid character (%d:%c) found!\n", firstChar, firstChar);
            return 0;
            break;
    }

    int seqCount = 0;
    for (int i = 0; i < strlen(s) && s[i] != '\0' ; i++)
    {
        printf("i = %d, c = '%c'\n", i, s[i]);
        seqCount++;
        if (s[i] == firstChar && i >= maxChar)
        {
            // Too many chars found in a row
            printf("Exiting after number of chars hit %d\n", i+1);
            return 0;
        }
        else if (s[i] != firstChar)
        {
            // Sequence has ended early
            printf("Sequence ended %c != %c\n", s[i], firstChar);
            return 0;
        }
    }

    // Sequence is valid
    printf("%d chars in valid sequence\n", seqCount);
    return seqCount;
}

int subtractiveSequenceInRomNumeralIsValid(RomNumeral *rN)
{
    return subtractiveSequenceInStringIsValid(rN->nString);
}

int subtractiveSequenceInStringIsValid(const char *s)
{
    // Check for empty string or single char (neither can be subtractive)
    if (s == NULL)
    {
        return 0;
    }
    else if (strlen(s) == 1)
    {
        return 0;
    }

    // Show string where sequence starts and get first character
    printf("Checking if '%s' is subtractive...\n", s);
    char firstChar = s[0];

    if (strlen(s) >= 2)
    {
        // For  'I'
        switch (firstChar) {
            case 'I':
                if (s[1] == 'V')
                {
                    return 4;
                }
                else if (s[1] == 'X')
                {
                    return 9;
                }
                else
                {
                    return 0;
                }
                break;

            case 'X':
                if (s[1] == 'L')
                {
                    return 40;
                }
                else if (s[1] == 'C')
                {
                    return 90;
                }
                else
                {
                    return 0;
                }
                break;

            case 'C':
                if (s[1] == 'D')
                {
                    return 400;
                }
                else if (s[1] == 'M')
                {
                    return 900;
                }
                else
                {
                    return 0;
                }
                break;

            default:
                printf("... not a valid subtractive sequence!\n");
                return 0;
                break;
        }
    }
}

bool isValid(RomNumeral *rN)
{
    if (numeralStringIsClean(rN))
    {
        // No foreign characters detected
        if (romNumToInt(rN->nString) == rN->value)
        {
            // Entered test value matches parsed value
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool isValidString(const char *s)
{
    // Not used yet
    return false;
}

int romNumToInt(const char *s)
{
    int retVal = 0;

    // Return 0 for NULL strings
    if (s == NULL)
    {
        return retVal;
    }

    // Setup variables for parsing
    // Setup magnitude counters
    int ones      = 0;
    int tens      = 0;
    int hundreds  = 0;
    int thousands = 0;

    // Setup magnitude markers
    bool inOnes      = false;
    bool inTens      = false;
    bool inHundreds  = false;
    bool inThousands = false;

    char prevChar = '\0';
    char currChar = '\0';
    char nextChar = '\0';

    int length = strlen(s);
    int charCount = 0;

    while (charCount < length)
    {
        int seqCount = 0;
        currChar = prevChar;
        currChar = s[charCount];
        if (currChar == 'I')
        {
            inOnes = true;

            if ((seqCount = subtractiveSequenceInStringIsValid(&s[charCount])) != 0)
            {
                ones += seqCount;
                charCount += 2;
            }
            else if ((seqCount = sequenceInStringIsValid(&s[charCount])) != 0)
            {
                ones += seqCount;
                charCount += seqCount;
            }
            else
            {
                return retVal;
            }
        }
        else if (currChar == 'V')
        {
            inOnes = true;
            ones += 5;
            charCount++;
        }
        else if (currChar == 'X')
        {
            inTens = true;
            if ((seqCount = subtractiveSequenceInStringIsValid(&s[charCount])) != 0)
            {
                tens += seqCount;
                charCount += 2;
            }
            else if ((seqCount = sequenceInStringIsValid(&s[charCount])) != 0)
            {
                tens += seqCount * 10;
                charCount += seqCount;
            }
            else
            {
                // Lone character
                tens += 10;
                charCount++;
            }
        }
        else if (currChar == 'L')
        {
            inTens = true;
            tens += 50;
            charCount++;
        }
        else if (currChar == 'C')
        {
            inHundreds = true;
            if ((seqCount = subtractiveSequenceInStringIsValid(&s[charCount])) != 0)
            {
                hundreds += seqCount;
                charCount += 2;
            }
            else if ((seqCount = sequenceInStringIsValid(&s[charCount])) != 0)
            {
                hundreds += seqCount * 100;
                charCount += seqCount;
            }
            else
            {
                // Lone character
                hundreds += 100;
                charCount++;
            }
        }
        else if (currChar == 'D')
        {
            inHundreds = true;
            hundreds += 500;
            charCount++;
        }
        else if (currChar == 'M')
        {
            inThousands = true;
            if ((seqCount = sequenceInStringIsValid(&s[charCount])) != 0)
            {
                thousands += seqCount * 1000;
                charCount += seqCount;
            }
            else
            {
                // Lone character
                thousands += 1000;
                charCount++;
            }
        }
        else
        {
            // Continue to avoid infinite loop
            charCount++;
        }
    }

    retVal = thousands + hundreds + tens + ones;
    printf("%s = %d\n", s, retVal);
    return retVal;
}

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
/*
RomNumeral romNumAddition(RomNumeral *rN1, RomNumeral *rN2)
{
    RomNumeral *rN3 = numeralCreate (

}

RomNumeral romNumSubtraction(RomNumeral *rN1, RomNumeral *rN2)
{

}

RomNumeral romNumConcatenation(RomNumeral *rN1, RomNumeral *rN2)
{

}
*/

/* New with attachment */
bool valString (const char *s)
{
    // Check for NULL string
    if (s == NULL)
    {
        return false;
    }

    int length = strlen(s);
    printf("'%s' at 0x%p, Length = %d\n", s, s, length);

    StrHolder vH;
    attachHolder(s, &vH);

    // Quick check - if any order lengths are negative then
    // they are out of order and hence invalid
    if (vH.onesLen < 0 ||
        vH.tensLen < 0 ||
        vH.hunsLen < 0 ||
        vH.thouLen < 0)
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

    if (vH.onesPtr != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.onesPtr, "I",    vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "II",   vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "III",  vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "IV",   vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "V",    vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "VI",   vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "VII",  vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "VIII", vH.onesLen) == 0) ||
            (strncmp(vH.onesPtr, "IX",   vH.onesLen) == 0))
        {
            onesValid = true;
            ordersValid++;
        }
    }
    if (vH.tensPtr != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.tensPtr, "X",    vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "XX",   vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "XXX",  vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "XL",   vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "L",    vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "LX",   vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "LXX",  vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "LXXX", vH.tensLen) == 0) ||
            (strncmp(vH.tensPtr, "XC",   vH.tensLen) == 0))
        {
            tensValid = true;
            ordersValid++;
        }
    }
    if (vH.hunsPtr != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.hunsPtr, "C",    vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "CC",   vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "CCC",  vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "CD",   vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "D",    vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "DC",   vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "DCC",  vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "DCCC", vH.hunsLen) == 0) ||
            (strncmp(vH.hunsPtr, "CM",   vH.hunsLen) == 0))
        {
            hunsValid = true;
            ordersValid++;
        }
    }
    if (vH.thouPtr != NULL)
    {
        // Validate
        ordersPresent++;
        if ((strncmp(vH.thouPtr, "M",   vH.thouLen) == 0) ||
            (strncmp(vH.thouPtr, "MM",  vH.thouLen) == 0) ||
            (strncmp(vH.thouPtr, "MMM", vH.thouLen) == 0))
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

char *addition (const char *as, const char *bs, char *cs)
{
    char onesStr[255];
    char tensStr[255];
    char hunsStr[255];
    char thouStr[255];
    // Clear output buffer
    memset(cs, '\0', 1);
    memset(onesStr, '\0', 1);
    memset(tensStr, '\0', 1);
    memset(hunsStr, '\0', 1);
    memset(thouStr, '\0', 1);
printf("\n\n\nbuffer clear!\n");
    // String holders
    StrHolder aH;
    StrHolder bH;
    StrHolder cH;

    // Attach holders
    attachHolder(as, &aH);
    attachHolder(bs, &bH);
    attachHolder(cs, &cH);

    // Add two Romman numeral strings
    if (stringIsClean(aH.mainStr) && stringIsClean(bH.mainStr))
    {
        if (valString(aH.mainStr) && valString(bH.mainStr))
        {
            printf("%s + %s = ", aH.mainStr, bH.mainStr);

            bool carry = false;

            carry = addOrder(&aH, &bH, onesStr, 'I', false);
            printf("Ones (%s)", onesStr);
            printf("%s\n", carry?" with carry":"");

            carry = addOrder(&aH, &bH, tensStr, 'X', carry);
            printf("Tens (%s)", tensStr);
            printf("%s\n", carry?" with carry":"");

            carry = addOrder(&aH, &bH, hunsStr, 'C', carry);
            printf("Hundreds (%s)", hunsStr);
            printf("%s\n", carry?" with carry":"");

            carry = addOrder(&aH, &bH, thouStr, 'M', carry);
            printf("Thousands (%s)", thouStr);
            printf("%s\n", carry?" with carry":"");

            // Check output
            printf("OUTPUT>> %s|%s|%s|%s\n ", thouStr, hunsStr, tensStr, onesStr);
            strncat(cH.mainStr, thouStr, strlen(thouStr));
            strncat(cH.mainStr, hunsStr, strlen(hunsStr));
            strncat(cH.mainStr, tensStr, strlen(tensStr));
            strncat(cH.mainStr, onesStr, strlen(onesStr));

            if (stringIsClean(cH.mainStr) && valString(cH.mainStr))
            {
                printf("CLEAN and VALID +++ %s strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
            }
            else
            {
                printf("NOT CLEAN/VALID --- '%s' strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
            }
            return cs;
        }
        else
        {
            printf("%s is not a proper Roman numeral.\n", bH.mainStr);
            return NULL;
        }
    }
    else
    {
        printf("%s is not a proper Roman numeral.\n", aH.mainStr);
        return NULL;
    }
}

char *subtraction (const char *as, const char *bs, char *cs)
{
    char onesStr[255];
    char tensStr[255];
    char hunsStr[255];
    char thouStr[255];
    // Clear output buffer
    memset(cs, '\0', 1);
    memset(onesStr, '\0', 1);
    memset(tensStr, '\0', 1);
    memset(hunsStr, '\0', 1);
    memset(thouStr, '\0', 1);
printf("\n\n\n sub buffer clear!\n");
    // String holders
    StrHolder aH;
    StrHolder bH;
    StrHolder cH;

    // Attach holders
    attachHolder(as, &aH);
    attachHolder(bs, &bH);
    attachHolder(cs, &cH);

    // Subtract two Romman numeral strings
    if (stringIsClean(aH.mainStr) && stringIsClean(bH.mainStr))
    {
        if (valString(aH.mainStr) && valString(bH.mainStr))
        {
            printf("%s - %s = ", aH.mainStr, bH.mainStr);

            bool borrow = false;

            borrow = subOrder(&aH, &bH, onesStr, 'I', false);
            printf("Ones (%s)", onesStr);
            printf("%s\n", borrow?" with borrow":"");

            borrow = subOrder(&aH, &bH, tensStr, 'X', borrow);
            printf("Tens (%s)", tensStr);
            printf("%s\n", borrow?" with borrow":"");

            borrow = subOrder(&aH, &bH, hunsStr, 'C', borrow);
            printf("Hundreds (%s)", hunsStr);
            printf("%s\n", borrow?" with borrow":"");

            borrow = subOrder(&aH, &bH, thouStr, 'M', borrow);
            printf("Thousands (%s)", thouStr);
            printf("%s\n", borrow?" with borrow":"");

            // Check output
            printf("OUTPUT>> %s|%s|%s|%s\n ", thouStr, hunsStr, tensStr, onesStr);
            strncat(cH.mainStr, thouStr, strlen(thouStr));
            strncat(cH.mainStr, hunsStr, strlen(hunsStr));
            strncat(cH.mainStr, tensStr, strlen(tensStr));
            strncat(cH.mainStr, onesStr, strlen(onesStr));

            if (stringIsClean(cH.mainStr) && valString(cH.mainStr))
            {
                printf("CLEAN and VALID +++ %s strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
            }
            else
            {
                printf("NOT CLEAN/VALID --- '%s' strlen(%zu)\n", cH.mainStr, strlen(cH.mainStr));
            }
            return cs;
        }
        else
        {
            printf("%s is not a proper Roman numeral.\n", bH.mainStr);
            return NULL;
        }
    }
    else
    {
        printf("%s is not a proper Roman numeral.\n", aH.mainStr);
        return NULL;
    }
}

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
    sh->onesLen = 0;
    sh->tensLen = 0;
    sh->hunsLen = 0;
    sh->thouLen = 0;

    // Substring pointers
    sh->iPtr = NULL;
    sh->vPtr = NULL;
    sh->xPtr = NULL;
    sh->lPtr = NULL;
    sh->cPtr = NULL;
    sh->dPtr = NULL;
    sh->mPtr = NULL;

    // Order pointers
    sh->onesPtr = NULL;
    sh->tensPtr = NULL;
    sh->hunsPtr = NULL;
    sh->thouPtr = NULL;

    // Specify pointers to existing numerals
    // ONES
    if ((sh->iPtr = strchr(sh->mainStr, 'I')) != NULL)
    {
        printf("I at 0x%p\n", sh->iPtr);
        // We have ones
        sh->onesPtr = sh->iPtr;
    }
    if ((sh->vPtr = strchr(sh->mainStr, 'V')) != NULL)
    {
        printf("V at 0x%p\n", sh->vPtr);
        if (sh->onesPtr != NULL)
        {
            if (sh->vPtr < sh->onesPtr)
            {
                // We have ones
                sh->onesPtr = sh->vPtr;
            }
        }
        else
        {
            // We have ones
            sh->onesPtr = sh->vPtr;
        }
    }

    // TENS
    if ((sh->xPtr = strchr(sh->mainStr, 'X')) != NULL)
    {
        printf("X at 0x%p\n", sh->xPtr);
        // Check if there is a subtractive
        if (sh->onesPtr != NULL)
        {
            if (sh->onesPtr > sh->xPtr)
            {
                // We have tens
                sh->tensPtr = sh->xPtr;
            }
            else
            {
                printf("subtractive\n");
            }
        }
        else
        {
            // We have tens
            sh->tensPtr = sh->xPtr;
        }
    }
    if ((sh->lPtr = strchr(sh->mainStr, 'L')) != NULL)
    {
        printf("L at 0x%p\n", sh->lPtr);
        if (sh->tensPtr != NULL)
        {
            if (sh->lPtr < sh->tensPtr)
            {
                // We have tens
                sh->tensPtr = sh->lPtr;
            }
        }
        else
        {
            // We have tens
            sh->tensPtr = sh->lPtr;
        }
    }

    // HUNDREDS
    if ((sh->cPtr = strchr(sh->mainStr, 'C')) != NULL)
    {
        printf("C at 0x%p\n", sh->cPtr);
        // Check if there is a subtractive
        if (sh->tensPtr != NULL)
        {
            if (sh->tensPtr > sh->cPtr)
            {
                // We have hundreds
                sh->hunsPtr = sh->cPtr;
            }
            else
            {
                printf("subtractive\n");
            }
        }
        else
        {
            // We have hundreds
            sh->hunsPtr = sh->cPtr;
        }
    }
    if ((sh->dPtr = strchr(sh->mainStr, 'D')) != NULL)
    {
        printf("D at 0x%p\n", sh->dPtr);
        if (sh->hunsPtr != NULL)
        {
            if (sh->dPtr < sh->hunsPtr)
            {
                // We have hundreds
                sh->hunsPtr = sh->dPtr;
            }
        }
        else
        {
            // We have hundreds
            sh->hunsPtr = sh->dPtr;
        }
    }

    // THOUSANDS
    if ((sh->mPtr = strchr(sh->mainStr, 'M')) != NULL)
    {
        printf("M at 0x%p\n", sh->mPtr);
        // Check if there is a subtractive
        if (sh->hunsPtr != NULL)
        {
            if (sh->hunsPtr > sh->mPtr)
            {
                // We have thousands
                sh->thouPtr = sh->mPtr;
            }
            else
            {
                printf("subtractive\n");
            }
        }
        else
        {
            // We have thousands
            sh->thouPtr = sh->mPtr;
        }
    }

    if (sh->onesPtr != NULL)
    {
        // onesPtr points to first char in ones part of the string
        sh->onesLen = strlen(sh->onesPtr);
        printf("onesPtr at 0x%p, length = %d\n", sh->onesPtr, sh->onesLen);
    }
    if (sh->tensPtr != NULL)
    {
        // tensPtr points to first char in tens part of the string
        if (sh->onesPtr != NULL)
        {
            sh->tensLen = sh->onesPtr - sh->tensPtr;
        }
        else
        {
            sh->tensLen = strlen(sh->tensPtr);
        }
        printf("tensPtr at 0x%p, length = %d\n", sh->tensPtr, sh->tensLen);
    }
    if (sh->hunsPtr != NULL)
    {
        // hunsPtr points to first char in hundreds part of the string
        if (sh->tensPtr != NULL)
        {
            sh->hunsLen = sh->tensPtr - sh->hunsPtr;
        }
        else if (sh->onesPtr != NULL)
        {
            sh->hunsLen = sh->onesPtr - sh->hunsPtr;
        }
        else
        {
            sh->hunsLen = strlen(sh->hunsPtr);
        }
        printf("hunsPtr at 0x%p, length = %d\n", sh->hunsPtr, sh->hunsLen);
    }
    if (sh->thouPtr != NULL)
    {
        // thouPtr points to first char in thousands part of the string
        if (sh->hunsPtr != NULL)
        {
            sh->thouLen = sh->hunsPtr - sh->thouPtr;
        }
        else if (sh->tensPtr != NULL)
        {
            sh->thouLen = sh->tensPtr - sh->thouPtr;
        }
        else if (sh->onesPtr != NULL)
        {
            sh->thouLen = sh->onesPtr - sh->thouPtr;
        }
        else
        {
            sh->thouLen = strlen(sh->thouPtr);
        }
        printf("thouPtr at 0x%p, length = %d\n", sh->thouPtr, sh->thouLen);
    }
}

//int subtractiveSequenceInStringIsValid(const char *s)
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
    printf("Checking if '%s' is subtractive...\n", s);
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

bool addOrder(StrHolder *aH, StrHolder *bH, char *cStr, char order, bool carriedOver)
{
printf("---------------- > addOrder %c\n", order);
    char *aOrderPtr;
    char *bOrderPtr;
    int aOrderLen;
    int bOrderLen;
    int x10Count = 0;
    char x1;
    char x5;
    char x10;
    bool carry = false;

    switch (order) {
        case 'I':
            aOrderPtr = aH->onesPtr;
            bOrderPtr = bH->onesPtr;
            aOrderLen = aH->onesLen;
            bOrderLen = bH->onesLen;
            x1  = order;
            x5  = 'V';
            x10 = 'X';
            break;

        case 'X':
            aOrderPtr = aH->tensPtr;
            bOrderPtr = bH->tensPtr;
            aOrderLen = aH->tensLen;
            bOrderLen = bH->tensLen;
            x1  = order;
            x5  = 'L';
            x10 = 'C';
            break;

        case 'C':
            aOrderPtr = aH->hunsPtr;
            bOrderPtr = bH->hunsPtr;
            aOrderLen = aH->hunsLen;
            bOrderLen = bH->hunsLen;
            x1  = order;
            x5  = 'D';
            x10 = 'M';
            break;

        case 'M':
            aOrderPtr = aH->thouPtr;
            bOrderPtr = bH->thouPtr;
            aOrderLen = aH->thouLen;
            bOrderLen = bH->thouLen;
            x1  = order;
            break;

        default:
            printf("incorrect order identifier '%c' exiting...", order);
            return false;
            break;
    }

//            if ( neither string has ones, skip )
//            else if ( one string has ones, but not the other, take the one that has )
//            else if ( both strings have ones, add/concatenate/merge them )
//              if ( a string starts with a I, see if it's subractive and save the result for later )
//                if ( neither is subtractive, line up strings together, largest letters first )
//                  ( any Xs or VVs get passed up the food chain the remaining V&Is get arranged in order largest first and the
//                       subtractives get taken away. Each subtractive is a -2 from the final tally
//                       save results to buffer for merging later with output buffer )

    if (aOrderPtr != NULL && bOrderPtr == NULL && !carriedOver)
    {
        strncat(cStr, aOrderPtr, aOrderLen);
    }
    else if (bOrderPtr != NULL && aOrderPtr == NULL && !carriedOver)
    {
        strncat(cStr, bOrderPtr, bOrderLen);
    }
    else /* if (bOrderPtr != NULL && aOrderPtr != NULL) */
    {
        int i = 0;
        int x1Count = carriedOver?1:0;
        int x5Count = 0;
        int subCount = 0;
printf("\n\nx10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

        if (isStringSubtractive(aOrderPtr))
        {
            subCount++;
        }
        if (isStringSubtractive(bOrderPtr))
        {
            subCount++;
        }

        // Tally characters
        for (i = 0; i < aOrderLen; i++)
        {
            if (aOrderPtr[i] == x10)
            {
                x10Count++;
            }
            else if (aOrderPtr[i] == x5)
            {
                x5Count++;
            }
            else if (aOrderPtr[i] == x1)
            {
                x1Count++;
            }
        }
        for (i = 0; i < bOrderLen; i++)
        {
            if (bOrderPtr[i] == x10)
            {
                x10Count++;
            }
            else if (bOrderPtr[i] == x5)
            {
                x5Count++;
            }
            else if (bOrderPtr[i] == x1)
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

        if (order == 'M' && (x10Count > 0 || x5Count > 0))
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
//                printf("%c", x10);
//                strncat(cStr, &x10, 1);
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
//                printf("%c", x10);
//                strncat(cStr, &x10, 1);
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
    printf("addOrder: got %s\n", cStr);

    return carry;
}

bool subOrder(StrHolder *aH, StrHolder *bH, char *cStr, char order, bool borrowedFrom)
{
printf("---------------- > subOrder %c\n", order);
    char *aOrderPtr;
    char *bOrderPtr;
    int aOrderLen;
    int bOrderLen;
    int x10Count = 0;
    char x1;
    char x5;
    char x10;
    bool borrow = false;

    switch (order) {
        case 'I':
            aOrderPtr = aH->onesPtr;
            bOrderPtr = bH->onesPtr;
            aOrderLen = aH->onesLen;
            bOrderLen = bH->onesLen;
            x1  = order;
            x5  = 'V';
            x10 = 'X';
            break;

        case 'X':
            aOrderPtr = aH->tensPtr;
            bOrderPtr = bH->tensPtr;
            aOrderLen = aH->tensLen;
            bOrderLen = bH->tensLen;
            x1  = order;
            x5  = 'L';
            x10 = 'C';
            break;

        case 'C':
            aOrderPtr = aH->hunsPtr;
            bOrderPtr = bH->hunsPtr;
            aOrderLen = aH->hunsLen;
            bOrderLen = bH->hunsLen;
            x1  = order;
            x5  = 'D';
            x10 = 'M';
            break;

        case 'M':
            aOrderPtr = aH->thouPtr;
            bOrderPtr = bH->thouPtr;
            aOrderLen = aH->thouLen;
            bOrderLen = bH->thouLen;
            x1  = order;
            break;

        default:
            printf("incorrect order identifier '%c' exiting...", order);
            return false;
            break;
    }

//            if ( neither string has ones, skip )
//            else if ( one string has ones, but not the other, take the one that has )
//            else if ( both strings have ones, add/concatenate/merge them )
//              if ( a string starts with a I, see if it's subractive and save the result for later )
//                if ( neither is subtractive, line up strings together, largest letters first )
//                  ( any Xs or VVs get passed up the food chain the remaining V&Is get arranged in order largest first and the
//                       subtractives get taken away. Each subtractive is a -2 from the final tally
//                       save results to buffer for merging later with output buffer )

//{
    int i = 0;
    int x1Count = borrowedFrom?-1:0;
    int x5Count = 0;
    int subCount = 0;
printf("\n\nbase tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

    if (isStringSubtractive(aOrderPtr))
    {
        subCount++;
    }
    if (isStringSubtractive(bOrderPtr))
    {
        subCount++;
    }

    // Tally characters (A is added)
    for (i = 0; i < aOrderLen; i++)
    {
        if (aOrderPtr[i] == x10)
        {
            x10Count++;
        }
        else if (aOrderPtr[i] == x5)
        {
            x5Count++;
        }
        else if (aOrderPtr[i] == x1)
        {
            x1Count++;
        }
    }
printf("post A tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

    // Tally characters (B is subtracted)
    for (i = 0; i < bOrderLen; i++)
    {
        if (bOrderPtr[i] == x10)
        {
            x10Count--;
        }
        else if (bOrderPtr[i] == x5)
        {
            x5Count--;
        }
        else if (bOrderPtr[i] == x1)
        {
            x1Count--;
        }
    }
printf("post B tally x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);
// Handle subtractives. When removing a subtractive it has to take another non-subtractive
// with it. eg IV - I  = (V - I) - I = IIII - I = III
//          eg XIV - V = X + (IV - V) = X + (-I) = IX (with a borrow from tens)
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

// if x1Count is 1 and subCount is 1 then we have a negative one situation and must borrow from
// higher up. Here we borrow in good faith that somewhere in the upper orders the difference will be
// reconciled. If not I guess it becomes a negative number?
    if ((x1Count == 1 && subCount == 1)  ||
        (x1Count == -1 && subCount == 0) ||
        (x5Count == -1 && x1Count < 0 && subCount == 0))
    {
        x10Count++;
        borrow = true;
    }


// Adjust tally
    if (x5Count == -1 && x10Count == 1)
    {
        x10Count--;
        x5Count = 1;
    }
    if (x1Count < 0)
    {
        x5Count--;
        x1Count = 5 + x1Count;
    }

    subCount = 0;
/*
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
*/
printf("post looping   x10 = %d, x5 = %d, x1 = %d, sub = %d\n", x10Count, x5Count, x1Count, subCount);

    if (order == 'M' && (x10Count > 0 || x5Count > 0))
    {
        // Only values up to MMMCMXCIX (3999) are supported
        fprintf(stderr, "Fatal Error: Only values up to MMMCMXCIX (3999) are supported\n");
        return false;
    }

    if (x1Count == -1 && x10Count == 1)
    { // added for x1Count == -1 in subOrder
        printf("output should be '");
        if (x1Count == -1)
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
    else if (subCount != 1)
    {
        printf("output should be '");
        if (x10Count == 1)
        {
//                printf("%c", x10);
//                strncat(cStr, &x10, 1);
            borrow = true;
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
//                printf("%c", x10);
//                strncat(cStr, &x10, 1);
            borrow = true;
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
//}
    printf("subOrder: got %s\n", cStr);

    return borrow;
}

