#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int main()
{
    char ch;
    int count_digit = 0;
    int count_char = 0;
    FILE *fp = fopen("random.txt", "r");
    while ((ch = fgetc(fp)) != EOF)
    {
        if (isdigit(ch))
        {
            count_digit++;
        }
        if (isalpha(ch))
        {
            count_char++;
        }
    }
    printf("There are %d numbers and %d characters. \n", count_digit, count_char);
    fclose(fp);
}