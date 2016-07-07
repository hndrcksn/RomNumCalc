#include "RomNumCalcApp.h"
#include "../lib/RomNumMath.h"

int main(int argc, char *argv[])
{
    // Output buffer
    char output[64];

    printf("Roman Numeral Calculator!\n");

    if (argc != 4)
    {
        printf("The RNC requires three inputs:\n");
        printf(" -a AAA BBB\n");
        printf(" This adds AAA to BBB and prints out the sum\n");
        printf(" -s AAA BBB\n");
        printf(" This subtracts BBB from AAA and prints out the sum\n");
    }
    else
    {
        if (!strcmp(argv[1], "-a"))
        {
            printf("%s + %s = %s\n", argv[2], argv[3], addition(argv[2], argv[3], output));
        }
        else if (!strcmp(argv[1], "-s"))
        {
            printf("%s - %s = %s\n", argv[2], argv[3], subtraction(argv[2], argv[3], output));
        }
    }
    return 0;
}
