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


// node *newNode(node *ptr)
// {
//     node *new_node = NULL;
//     if (ptr->next == NULL)
//     {
//         ptr->next = malloc(sizeof(node));
//         new_node = ptr->next;
//     }
//     else
//     {
//         new_node = newNode(ptr->next);
//     }
//     return new_node;
// }

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

    if(!input)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    char buffer[LENGTH + 1];

// char sample[3] = {'s','a','m'};
// printf("%s (%lu)\n",sample,strlen(sample));

    while (fgets(buffer, LENGTH + 2, input))
    {
        unsigned int hash_value = hash(buffer);

        node *tmp = NULL;

        buffer[strcspn(buffer, "\n")] = 0;
// printf("fgets buffer: %s (%lu)\n", buffer, strlen(buffer));

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

// printf("copied: %s (%lu)\n\n", tmp->word, strlen(tmp->word));

        dic_size++;
    }


//...............................
 /*
    rewind(input);
    int largestsum = 0, smallestsum = 5000;
    while (fgets(buffer, LENGTH, input))
    {
        if (hash(buffer)>largestsum)
        {
            largestsum = hash(buffer);
        }
        else if (hash(buffer)<smallestsum)
        {
            smallestsum = hash(buffer);
        }
    }

    rewind(input);

    const int m = largestsum; //maxsum
    int count[m];
    for (int i = 0 ; i < m; i++)
    {
        count[i] = 0;
    }
    while (fgets(buffer, LENGTH, input))
    {
        //  hash(buffer);
        //  printf("%i\n",hash(buffer));
        count[hash(buffer)]++;
        if (hash(buffer)>largestsum)
            largestsum = hash(buffer);
    }

    const int largerthan = 30;
    long int max_appearance=0, zero_cnt=0, largerthan_cnt=0;
    // int max_appearance_num;
    int avg_appearance;

    for (int i = 0 ; i < m; i++)
    {
        if (count[i] > max_appearance)
        {
            max_appearance = count[i];
            // max_appearance_num = i;
        }

        if (count[i] ==0)
        {
            zero_cnt++;
        }

        if (count[i] > largerthan)
        {
            largerthan_cnt++;
        }
    }

    long int buf=0;
    int o = m;
    for (int i =0;i<m;i++)
    {
        buf += count[i];
        if (count[i]==0)
            o--;
    }
    avg_appearance = buf/o;

    printf("max appearance: %ld\nzero appearance: %ld\nlarger than %d: %ld\n",max_appearance,zero_cnt,largerthan,largerthan_cnt);
    printf("largest sum: %d, smallest sum: %d\n",largestsum,smallestsum);
    printf("average appearance: %d, total buckets: %d, used buckets: %ld, percent of buckets wasted: %d\n",avg_appearance, largestsum, largestsum-zero_cnt,(int)(zero_cnt*100.0/largestsum));
    printf("\n");
*/
//...............................

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
    query[str_len - 1]='\0';

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
