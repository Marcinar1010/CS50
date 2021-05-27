#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //variables
    long card_nr;
    int value, sum1 = 0, sum2 = 0, total, i = 0, j = 0, first_digit, second_digit, m, n, o;

    do
    {
        card_nr = get_long("Number: ");
    }
    while (card_nr < 0);

    while (card_nr > 1)
    {
        j++;
        i++;
        value = card_nr % 10;
        if (i == 1)
        {
            sum1 = sum1 + value;
        }
        if (i == 2)
        {
            if (value * 2 > 9)
            {
                m = (value * 2) % 10;
                n = (value * 2) / 10;
                o = m + n;
            }
            else
            {
                o = value * 2;
            }
            sum2 = sum2 + o;
            i = 0;
        }
        if (card_nr < 100)
        {
            if (card_nr < 10)
            {
                first_digit = value;
            }
            else
            {
                second_digit = value;
            }
        }
        card_nr = card_nr / 10;
    }
    //checking coretness
    total = sum1 + sum2;
    //printf("card %li\n", card_nr);
    //printf("first %i\n", first_digit);
    //printf("second %i\n", second_digit);
    //printf("sum1 %i\n", sum1);
    //printf("sum2 %i\n", sum2);
    //printf("m %i\n", m);
    //printf("n %i\n", n);
    //printf("0 %i\n", o);
    //printf("total %i\n", total);
    //printf("j %i\n", j);
    // check if AMEX, MC, VISA, otherwise invalid
    if (j == 15 && first_digit == 3 && (second_digit == 4 || second_digit == 7) && total % 10 == 0)
    {
        printf("AMEX\n");
    }
    else if (j == 16 && first_digit == 5 && (second_digit == 1 || second_digit == 2 || second_digit == 3 || second_digit == 4
    || second_digit == 5) && total % 10 == 0)
    {
        printf("MASTERCARD\n");
    }
    else if ((j == 13 || j == 16) && first_digit == 4 && total % 10 == 0)
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}