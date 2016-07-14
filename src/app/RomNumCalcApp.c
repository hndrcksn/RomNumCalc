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
        printf(" -c AAA BBB\n");
        printf(" Compares AAA to BBB and prints out the relative difference or equality\n");
    }
    else
    {
        if (!strcmp(argv[1], "-a"))
        {
            if (addition(argv[2], argv[3], output) != NULL)
            {
                printf("%s + %s = %s\n", argv[2], argv[3], strlen(output)!=0?output:"nihil");
                return 0;
            }
            else
            {
                fprintf(stderr, "Error! No Data output!\n");
                return -1;
            }
        }
        else if (!strcmp(argv[1], "-s"))
        {
            if (subtraction(argv[2], argv[3], output) != NULL)
            {
                printf("%s - %s = %s\n", argv[2], argv[3], strlen(output)!=0?output:"nihil");
                return 0;
            }
            else
            {
                fprintf(stderr, "Error! No Data output!\n");
                return -2;
            }
        }
        else if (!strcmp(argv[1], "-c"))
        {
            int compare = romStrCmp(argv[2], argv[3]);
            if (compare == 0)
            {
                printf("%s == %s\n", argv[2], argv[3]);
                return 0;
            }
            else if (compare < 0 && compare > -900)
            {
                printf("%s < %s\n", argv[2], argv[3]);
                return 0;
            }
            else if (compare > 0)
            {
                printf("%s > %s\n", argv[2], argv[3]);
                return 0;
            }
            else
            {
                fprintf(stderr, "Error! No comparison!\n");
                return -3;
            }
        }
    }
    return 0;
}
