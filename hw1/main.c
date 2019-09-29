#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"

int main() 
{
    hashmap_t map[HASH_SIZE];
    load_dictionary("wordlist.txt", map);
    char* misspelled[MAX_MISSPELLED];
    char text[] = "test1.txt";
    FILE* fp = fopen(text, "r");
    check_words(fp, map, misspelled);

    return 0;
}

