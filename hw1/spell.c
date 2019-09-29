#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"


int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) 
{
    int misscount = 0;
    char punctuations[] = " ,.!?:;/\n\r\t";
    char buffer;
    char wordbuffer[LENGTH + 1];
    int wp = 0;
    while ((buffer = fgetc(fp)) >= 0) 
    {
        if (buffer < 'Z' && buffer >= 'A') 
        {
            buffer += 'a' - 'A';
        }
        if (strchr(punctuations, buffer) != NULL) 
        {
            if (wp == 0) 
            {
                continue;
            }
            else 
            {
                wordbuffer[wp++] = '\0';
                if (!check_word(wordbuffer, hashtable)) 
                {   
                    printf("misspelled: %s\n",wordbuffer);
                    if (misscount < MAX_MISSPELLED) 
                    {
                        char misspelledword[LENGTH + 1];
                        strcpy(misspelledword, wordbuffer);
                        misspelled[misscount++] = misspelledword;
                    }
                }
                wp = 0;
            }
        }
        else 
        {
            if (wp == 45) 
            {
                continue;
            }
            wordbuffer[wp++] = buffer;
        }
    }
    int i = 0;
    node* n;
    node* p;
    for (i = 0; i < HASH_SIZE; i++) 
    {
        n = hashtable[i];
        
        while (n != NULL) 
        {
            p = n -> next;
            free(n);
            n = p;
        }        
    }
    return misscount;
}

bool check_word(const char* word, hashmap_t hashtable[])
{
    int hashval = hash_function(word);
    node* n = hashtable[hashval];
    while (n != NULL) 
    {
        if (strcmp(n->word, word) == 0) 
        {
            return true;
        }
        n = n->next;
    }
    return false;
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) 
{
    FILE* fd = fopen(dictionary_file, "r");
    if (fd == NULL) 
    {
        return false;
    }
    char buffer;
    char wordbuffer[LENGTH + 1];
    int wp = 0;
    node* n;
    int i = 0;
    for (i = 0; i < HASH_SIZE; i++) 
    {
        hashtable[i] = NULL;
    }
    while ((buffer = fgetc(fd)) >= 0) 
    {
        if (buffer != '\n')
        {   
            if (wp == 45) 
            {
                continue;
            }
            wordbuffer[wp++] = buffer;
        }
        else 
        {
            wordbuffer[wp++] = '\0';
            n = malloc(sizeof(node));
            n -> next = NULL;
            strcpy(n->word, wordbuffer);
            int hashval = hash_function(n->word);
            n->next = hashtable[hashval];
            hashtable[hashval] = n;
            wp = 0;
        }
    }
    return true;
}
