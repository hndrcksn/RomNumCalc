#include "RomNumCalcApp.h"
#include "../lib/RomNumMath.h"

int main(int argc, char *argv[])
{
    // Output buffer
    char output[32];
    memset(output, '\0', 32);

    printf("Roman Numeral Calculator!\n");

    if (argc != 4)
    {
        printf("The Roman Numeral Calculator requires three inputs:\n");
        printf(" -a AAA BBB\n");
        printf(" Adds AAA to BBB and prints out the sum\n");
        printf(" -s AAA BBB\n");
        printf(" Subtracts BBB from AAA and prints out the difference\n");
    }
    else
    {
        if (!strcmp(argv[1], "-a"))
        {
            if (addition(argv[2], argv[3], output) != NULL)
            {
                printf("%s + %s = %s\n", argv[2], argv[3], output);
                return 0;
            }
            else
            {
                printf("Error!\n");
                return -1;
            }
        }
        else if (!strcmp(argv[1], "-s"))
        {
            if (subtraction(argv[2], argv[3], output) != NULL)
            {
                printf("%s - %s = %s\n", argv[2], argv[3], output);
                return 0;
            }
            else
            {
                printf("Error!\n");
                return -1;
            }
        }
    }
    return 0;
}
