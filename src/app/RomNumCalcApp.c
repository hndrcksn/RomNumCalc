#include "RomNumCalcApp.h"
#include "../lib/RomNumMath.h"

int main(int argc, char *argv[])
{
    // Prepare output buffer
    char output[32];
    memset(output, '\0', 32);

    // Set expected operation argument
    int operation_arg = 1;

    // Self aggrandizement
    printf("Roman Numeral Calculator!\n");

    if ((argc == 5 && strcmp(argv[operation_arg], "-d")) ||
        (argc == 4 && !strcmp(argv[operation_arg], "-d")) ||
        (argc != 4 && argc != 5))
    {   // Help message
        printf("The Roman Numeral Calculator generally requires three inputs\n");
        printf("(an operational argument followed by two Roman numerals):\n\n");
        printf("  -a AAA BBB\n");
        printf("  Adds AAA to BBB and prints out the sum\n");
        printf("  -s AAA BBB\n");
        printf("  Subtracts BBB from AAA and prints out the difference\n");
        printf("  -c AAA BBB\n");
        printf("  Compares AAA to BBB and prints out the relative difference or equality\n\n");
        printf("A '-d' may be added as an argument before the operational arguments -a, -s or -c\n\n");
        printf("  -d -a AAA BBB\n");
        printf("  Enables debugging mode and prints addition procedural data to stdout\n");
    }
    else
    {
        if (!strcmp(argv[operation_arg], "-d"))
        {   // Handle debugging
            global_debugging = true;
            operation_arg = 2;
        }

        if (!strcmp(argv[operation_arg], "-a"))
        {   // Handle addition
            if (addition(argv[operation_arg+1], argv[operation_arg+2], output) != NULL)
            {
                printf("%s + %s = %s\n", argv[operation_arg+1], argv[operation_arg+2], strlen(output)!=0?output:"nihil");
                return 0;
            }
            else
            {
                printf("ERROR! No data output!\n");
                return -1;
            }
        }
        else if (!strcmp(argv[operation_arg], "-s"))
        {   // Handle subtraction
            if (subtraction(argv[operation_arg+1], argv[operation_arg+2], output) != NULL)
            {
                printf("%s - %s = %s\n", argv[operation_arg+1], argv[operation_arg+2], strlen(output)!=0?output:"nihil");
                return 0;
            }
            else
            {
                printf("ERROR! No Data output!\n");
                return -2;
            }
        }
        else if (!strcmp(argv[operation_arg], "-c"))
        {   // Handle comparison
            int comparison = romStrCmp(argv[operation_arg+1], argv[operation_arg+2]);
            if (comparison == 0)
            {
                printf("%s == %s\n", argv[operation_arg+1], argv[operation_arg+2]);
                return 0;
            }
            else if (comparison < 0 && comparison > -900)
            {
                printf("%s < %s\n", argv[operation_arg+1], argv[operation_arg+2]);
                return 0;
            }
            else if (comparison > 0)
            {
                printf("%s > %s\n", argv[operation_arg+1], argv[operation_arg+2]);
                return 0;
            }
            else
            {
                printf("ERROR! No comparison!\n");
                return -3;
            }
        }
    }
    return 0;
}
