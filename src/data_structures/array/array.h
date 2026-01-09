#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "funcs.h"

/**
 * maximum increment possible when realloc'ing array.
 */
#define MAX_INCREMENT 1000

struct _Array {
    void** elems;
    unsigned size;
    unsigned capacity;
    functionCopy copy;
    functionCompareEq compare;
    functionDelete del;
    functionPrint print;
};

typedef struct _Array* Array;


/**
 * create a new empty array with the given capacity and functions. Return NULL on error.
 *  Print function is optional; must be NULL if not used.
 */
Array array_create(unsigned int capacity, functionCopy copy, functionCompareEq compare, functionDelete del, functionPrint print);

/**
 * add the data at the end of the array.
 */
void array_add(Array arr, void* data);

/**
 * return the array size.
 */
unsigned int array_size(Array arr);

/**
 * print array.
 */
void array_print(Array arr);

/**
 * delete the given array from memory.
 */
void array_destroy(Array arr);

#endif /* __ARRAY_H__ */