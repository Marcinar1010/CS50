// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 676;
unsigned int nr_words_dictionary = 0;

// Hash table, initializing hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // current linked list
    // unsigned int position = hash(word);
    node *head = table[hash(word)];
    // search the word int the particular linked list
    for (node *tmp = head; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    // hash function, returns values between 0 - 675

    unsigned int position = 0;
    if (word[1] == '\0')
    {
        position = ((tolower(word[0]) - 97) * 26);
    }
    else
    {
        position = (((tolower(word[0]) - 97) * 26) + (tolower(word[1]) - 97));
    }
    return position;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    // TODO
    // fill up hash table with NULL pointers
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // open a dictionary file
    FILE *file = fopen(dictionary, "r");
    // check if opened
    if (file == NULL)
    {
        printf("Couldn't open the dictionary file \n");
        nr_words_dictionary = 0;
        return false;
    }
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Allocate memory for new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Not enough space for dictionary\n");
            free(n);
            nr_words_dictionary = 0;
            return false;
        }
        // copy the word to a new node
        strcpy(n->word, word);
        nr_words_dictionary++;
        // current position in a hash table
        unsigned int position = hash(word);

        // check if null pointer in hashtable, table empty -->
        if (table[position] == NULL)
        {
            // put the adress of the first node to the hash table
            table[position] = n;
            // set the pointer in the node to NULL
            n->next = NULL;

        }
        else
        {
            // add a new node at the beginning of the linked list
            n->next = table[position];
            table[position] = n;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return nr_words_dictionary;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    // iterate over all linked lists and delete every linked list
    // pointer temp
    node *temp = NULL;
    node *cursor = NULL;
    for (int i = 0; i < N; i++)
    {
        cursor = table[i];
        while (cursor)
        {
            temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
