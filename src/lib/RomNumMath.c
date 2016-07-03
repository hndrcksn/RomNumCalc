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
    // String is clean if it only contains valid Roman numeral characters
    char *validRomanChars = "IVXLCMD";
    printf("Checking if '%s' is clean...\n", rN->nString);
    for (int i = 0; i < strlen(rN->nString); i++)
    {
        printf("i = %d, c = '%c'\n", i, (rN->nString)[i]);
        if (strchr(validRomanChars, (rN->nString)[i]) == NULL)
        {
            // Invalid char found
            printf("Invalid char = '%c'\n", (rN->nString)[i]);
            return false;
        }
    }
    // All chars are valid
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
    printf("%d chars in sequence\n", seqCount);
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
        if (parseToInt(rN->nString) == rN->value)
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

int parseToInt(const char *s)
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
