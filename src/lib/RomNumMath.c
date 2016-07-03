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
    printf("Checking '%s'...\n", rN->nString);
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

bool sequenceInRomNumeralIsValid(RomNumeral *rN)
{
    return sequenceInStringIsValid(rN->nString);
}

bool sequenceInStringIsValid(const char *s)
{
    // Check for NULL
    if (s == NULL)
    {
        return false;
    }

    // Show string where sequence starts and get first character
    printf("Checking '%s'...\n", s);
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
            return false;
            break;
    }

    for (int i = 0; i < strlen(s) && s[i] != '\0' ; i++)
    {
        printf("i = %d, c = '%c'\n", i, s[i]);
        if (s[i] == firstChar && i >= maxChar)
        {
            // Too many chars found in a row
            printf("Exiting after number of chars hit %d\n", i+1);
            return false;
        }
    }
    // Sequence is valid
    return true;
}

bool subtractiveISequenceInRomNumeralIsValid(RomNumeral *rN)
{
    return subtractiveISequenceInStringIsValid(rN->nString);
}

bool subtractiveISequenceInStringIsValid(const char *s)
{
    // Check for NULL
    if (s == NULL)
    {
        return false;
    }

    // Show string where sequence starts and get first character
    printf("Checking '%s'...\n", s);
    char firstChar = s[0];
    if (firstChar != 'I')
    {
        // Only checking for I's here
        return false;
    }
    else if (strlen(s) >= 2)
    {
        // Check next char
        switch (s[1]) {
            case 'V':
            case 'X':
                return true;
                break;
            default:
                printf("... not a valid subtractive sequence!\n");
                return false;
                break;
        }
    }
}
