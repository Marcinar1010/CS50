#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string);
int count_words(string);
int count_sentences(string);

int main(void)
{
    // prompt for text
    string input = get_string("Text: ");
    //printf("%i letter(s).\n", count_letters(input));
    //printf("%i word(s).\n", count_words(input));
    //printf("%i sentence(s).\n", count_sentences(input));

    // calculate index
    float l = count_letters(input) * 1.0;
    float w = count_words(input) * 1.0;
    float s = count_sentences(input) * 1.0;
    float L = l / w * 100;
    float S = s / w * 100;
    int index = round((0.0588 * L) - (0.296 * S) - 15.8);
    // printf("index %i\n", index);

    // print the output
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
    return 0;
}

// counting letters
int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

// counting words
int count_words(string text)
{
    int words = 1;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 32)
        {
            words++;
        }
    }
    return words;
}

// counting sentences
int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
            sentences++;
        }
    }
    return sentences;
}
