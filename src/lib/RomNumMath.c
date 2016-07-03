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
    char *validRomanChars = "IVXLCMD";
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

