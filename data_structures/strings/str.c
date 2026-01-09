#include "str.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void* str_copy(void* s) {
    char* copy = malloc(sizeof(char) * (strlen((char*) s)+1));
    strcpy(copy, (char*) s);
    return copy;
}

int str_compare(void* s1, void* s2) {
    return strcmp((char*) s1, (char*) s2);
}

void str_delete(void* s) {
    free(s);
}

void str_print(void* s) {
    printf("%s\n", (char*) s);
}


unsigned long str_hash1(void* str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *(char*) str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

unsigned long str_hash2(void* str) {
    unsigned long hash = 0;
    int c;

    while ((c = *(char*) str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}