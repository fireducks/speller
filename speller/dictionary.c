// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 15400;
unsigned int dic_size = 0;

// Hash table
node *table[N];

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;

    for (int i = 0; i < strlen(word); i++)
    {
        if (isalpha(word[i]))
        {
            sum += (word[i] - 95) * (LENGTH - i);
        }
    }
    return abs(sum);
}


node *newNode(node *ptr)
{
    if (ptr->next == NULL)
    {
        ptr->next = malloc(sizeof(node));
        return ptr->next;
    }
    else
    {
        return newNode(ptr->next);
    }
}


// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r");

    if (!input)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char buffer[LENGTH + 1];

    while (fgets(buffer, LENGTH + 2, input))
    {
        unsigned int hash_value = hash(buffer);

        node *tmp = NULL;

        buffer[strcspn(buffer, "\n")] = 0;

        if (table[hash_value] != NULL)
        {
            tmp = newNode(table[hash_value]);
        }
        else
        {
            table[hash_value] = malloc(sizeof(node));
            tmp = table[hash_value];
        }

        if (!tmp)
        {
            fclose(input);
            return false;
        }

        strcpy(tmp->word, buffer);
        tmp->next = NULL;
        dic_size++;
    }

    fclose(input);

    return true;
}

bool list_strcmp(node *ptr, const char *query)
{
    if (strcmp(ptr->word, query) == 0)
    {
        return true;
    }
    else if (ptr->next)
    {
        return list_strcmp(ptr->next, query);
    }
    else
    {
        return false;
    }
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int str_len = strlen(word) + 1;
    char query[str_len];

    for (int i = 0; i < str_len - 1; i++)
    {
        query[i] = tolower(word[i]);
    }
    query[str_len - 1] = '\0';

    unsigned int hash_value = hash(query);

    node *ptr = table[hash_value];

    if (!ptr)
    {
        return false;
    }
    else if (list_strcmp(ptr, query))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return dic_size;
}

// Unloads dictionary from memory, returning true if successful, else false
void recursive_unload(node *ptr)
{
    if (ptr != NULL)
    {
        recursive_unload(ptr->next);
        free(ptr);
    }
    return;
}

bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        recursive_unload(table[i]);
    }

    return true;
}
