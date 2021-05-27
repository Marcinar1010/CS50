#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


int main(int argc, string argv[])
{
    char KEY_l[26];
    char KEY_u[26];
    if (argc == 2)
    {
        int n = strlen(argv[1]);
        // validate the key
        // check if all characters are letters
        for (int i = 0; i < n; i++)
        {
            if ((argv[1][i] >= 'a' && argv[1][i] <= 'z') || (argv[1][i] >= 'A' && argv[1][i] <= 'Z'))
            {
                KEY_l[i] = tolower(argv[1][i]);
                KEY_u[i] = toupper(argv[1][i]);
            }
            else
            {
                printf("Key must contain 26 characters.\n");
                return 1;
            }
        }
        // check if characters repeated
        for (int l = 0; l < n - 1; l++)
        {
            for (int k = l + 1; k < n; k++)
            {
                if (KEY_l[l] == KEY_l[k])
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;
                }
            }
        }
        // check if there is 26 characters
        if (n == 26)
        {
            //prompting user for input
            string input = get_string("plaintext: ");
            int m = strlen(input);
            char out[m + 1];
            // crypting
            for (int j = 0; j < m; j++)
            {
                if (isupper(input [j]))
                {
                    out[j] = KEY_u[ input[j] - 65];
                }
                else if (islower(input [j]))
                {
                    out[j] = KEY_l[ input[j] - 97];
                }
                else
                {
                    out[j] = input[j];
                }
            }
            out[m] = '\0';
            // printing cyphertext
            printf("ciphertext: %s\n", out);
            return 0;
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
}