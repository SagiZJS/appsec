#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"

int main(int argc, char** argv) 
{   
    if (argc != 3) 
    {
        printf("usage: ./spell_check wordlist.txt text,txt");
        return 0;
    }
    hashmap_t map[HASH_SIZE];
    char* wordlist = argv[1];
    load_dictionary(wordlist, map);
    char* misspelled[MAX_MISSPELLED];
    char* text = argv[2];
    FILE* fp = fopen(text, "r");
    check_words(fp, map, misspelled);
    return 0;
}

