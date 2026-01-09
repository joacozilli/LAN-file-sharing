#include <stdio.h>
#include "..data_structures/hashtable/hashtable.h"
#include "../src/data_structures/array/array.h"
#include "../src/data_structures/lists/concurrent_list.h"
#include "../src/data_structures/strings/str.h"
#include "../include/network.h"
#include <string.h>
#include <assert.h>

int main() {

    FILE* shakespeare = fopen("shakespeare.txt", "r");
    assert(shakespeare != NULL);

    Hashtable t = ht_create(str_copy, str_compare, str_hash1, str_hash2, str_delete, str_print);

    char buffer[512];
    while (fgets(buffer, 512, shakespeare)) {
        buffer[strlen(buffer)-1] = '\0';
        ht_add(t, buffer);
    }

    //ht_print(t);

    ht_destroy(t);
    fclose(shakespeare);

    return 0;
}