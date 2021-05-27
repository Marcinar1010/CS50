#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

typedef uint8_t BYTE;

//prototypes
bool check_jpg(BYTE buffer[512]);
void save_block(BYTE buffer[512], FILE *output);

int main(int argc, char *argv[])
{
    // run program only when given 1 input argument
    if (argc == 2)
    {
        char *filename = argv[1];
        FILE *input = fopen(filename, "r");
        FILE *output = NULL;
        if (input == NULL)
        {
            printf("File couldn't be opened\n");
            return 1;
        }
        BYTE *buffer = malloc(512 * sizeof(BYTE));
        if (buffer == NULL)
        {
            printf("Not enough memory to store the buffer\n");
            return 1;
        }
        int nr_of_jpgs = 0;
        char *current_name = malloc(8);
        if (current_name == NULL)
        {
            printf("Not enough memory to store the file name\n");
            return 1;
        }

        // read the 512 bytes data blocks from the input file
        while (fread(buffer, sizeof(BYTE), 512, input))
        {
            if (check_jpg(buffer))
            {
                if (nr_of_jpgs == 0)
                {
                    sprintf(current_name, "%03i.jpg", nr_of_jpgs);
                    output = fopen(current_name, "w");
                }
                else
                {
                    fclose(output);
                    sprintf(current_name, "%03i.jpg", nr_of_jpgs);
                    output = fopen(current_name, "w");
                }
                save_block(buffer, output);
                nr_of_jpgs++;
            }
            else
            {
                if (nr_of_jpgs > 0)
                {
                    save_block(buffer, output);
                }
            }
        }
        fclose(output);
        fclose(input);
        free(current_name);
        free(buffer);
    }
    else
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
}

bool check_jpg(BYTE buffer[512])
{
    return buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
}

void save_block(BYTE buffer[512], FILE *output)
{
    fwrite(buffer, sizeof(BYTE), 512, output);
}