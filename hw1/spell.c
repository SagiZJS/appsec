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
    int i = 0;
    bool capital = true;
    bool punctuation = false;
    while ((buffer = fgetc(fp)) != EOF) 
    {
        if (strchr(punctuations, buffer) != NULL) 
        {   
            punctuation = false;
            if (buffer == '.' ||buffer == '?'||buffer == '!') 
            {
                if (buffer == '.')
                {
                    punctuation = true;
                    continue;
                }
                capital = true;
            }
            if (wp == 0) 
            {
                continue;
            }
            else 
            {
                wordbuffer[wp++] = '\0';
                if (capital) 
                {
                    if (wordbuffer[0] >= 'A' &&  wordbuffer[0] <= 'Z')
                    {
                        for (i = 1; i< strlen(wordbuffer); i++) 
                        {
                            if (wordbuffer[i] >= 'A' &&  wordbuffer[i] <= 'Z') 
                            {
                                capital = false;
                                break;
                            }
                        }
                        if (capital) 
                        {
                            wordbuffer[0] += 'a' - 'A';
                        }

                    }
                }
                if (!check_word(wordbuffer, hashtable)) 
                {   
                    if (misscount < MAX_MISSPELLED) 
                    {
                       // printf("misspell:%s\n", wordbuffer);
  //                      char misspelledword[LENGTH + 1];
                        misspelled[misscount]=(char *) malloc(wp * sizeof(char));
                        strcpy(misspelled[misscount++], wordbuffer);
//                        misspelled[misscount++] = misspelledword;
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
            if (punctuation) 
            {
                wordbuffer[wp++]='.';
                punctuation = false;
            }
            if (wp == 45) 
            {
                continue;
            }
            wordbuffer[wp++] = buffer;
        }
    }
    return misscount;
}

void free_map(hashmap_t hashtable[]) 
{
    int i=0;
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
}

bool check_word(const char* word, hashmap_t hashtable[])
{   

    int i = 0;
    for (i = 0; i < strlen(word); i++) 
    {
        if (word[i] < 0) 
        {
            return false;
        }
    }
    bool flag = true;
    for (i=0;i<strlen(word);i++) 
    {
        if (word[i]<'0' || word[i]>'9') 
        {
            flag = false;
            break;
        }
    }
    if (flag) 
    {
        return flag;
    }
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
    while ((buffer = fgetc(fd)) != EOF) 
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
