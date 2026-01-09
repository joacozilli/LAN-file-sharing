#include "array.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


Array array_create(unsigned int capacity, functionCopy copy, functionCompareEq compare, functionDelete del, functionPrint print) {
    if (capacity <= 0 || !copy || !compare || !del)
        return NULL;
    Array arr = malloc(sizeof(struct _Array));
    if (!arr) {
        fprintf(stderr, "NULL return by malloc in array_create\n");
        return NULL;
    }

    arr->size = 0;
    arr->capacity = capacity;
    arr->copy = copy;
    arr->compare = compare;
    arr->del = del;
    arr->print = print;
    arr->elems = malloc(sizeof(void*) * capacity);
    for (unsigned int i = 0; i < capacity; i++)
        arr->elems[i] = NULL;
    
    return arr;
}


void array_add(Array arr, void* data) {
    if (!arr) {
        fprintf(stderr, "argument arr given is NULL in array_add\n");
        return;
    }
    if (!data) {
        fprintf(stderr, "argument data given is NULL in array_add\n");
        return;
    }
    arr->size++;
    if (arr->size > arr->capacity) {
        int inc = arr->capacity*2 < MAX_INCREMENT ? arr->capacity*2 : MAX_INCREMENT;
        int newCapacity = arr->capacity + inc;
        arr->elems = realloc(arr->elems, sizeof(void*) * newCapacity);
        arr->capacity = newCapacity;
    }
    arr->elems[arr->size-1] = arr->copy(data);
}

unsigned int array_size(Array arr) {
    if (!arr) {
        fprintf(stderr, "argument arr given is NULL in array_size\n");
        return -1;
    }
    return arr->size;
}

void array_print(Array arr) {
    if (!arr) {
        fprintf(stderr, "argument arr given is NULL in array_print\n");
        return;
    }
    for (unsigned int i = 0; i < arr->size; i++)
        arr->print(arr->elems[i]);
}

void array_destroy(Array arr) {
    if (!arr) {
        fprintf(stderr, "argument arr given is NULL in array_destroy\n");
        return;
    }

    for (unsigned int i = 0; i < arr->size; i++) 
        arr->del(arr->elems[i]);
    
    free(arr->elems);
    free(arr);
}