#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // variables
    int height, i, j, k, m;
    do
    {
        height = get_int("Height: \n");
    }
    while ((height < 1) || (height > 8));


    // printing bricks
    // controlling row number by i
    for (i = 1; i < height + 1; i++)
    {
        // empty spaces in a row
        for (j = 0; j < height - i; j++)
        {
            printf(" ");
        }
        // left bricks in a row
        for (k = 0; k < i; k++)
        {
            printf("#");
        }
        // gap
        printf("  ");
        // right bricks
        for (m = 0; m < i; m++)
        {
            printf("#");
        }
        // new row
        printf("\n");
    }
}