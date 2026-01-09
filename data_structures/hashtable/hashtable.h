#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stddef.h>
#include "funcs.h"

// initial capacity of a hash table
#define INITIAL_CAPACITY 103

// factor after which a hash table is rehashed.
#define BALANCE_FACTOR  0.65

/**
 * describes if the element in the cell is deleted or not
 */
typedef enum {
    NO,
    DELETED
} del_status_t;

struct _Cell {
    void* data;
    del_status_t status;
};
typedef struct _Cell* Cell;

/**
 * Collisions are resolved with double hashing.
 */
struct _Hashtable {
    Cell* elems;
    size_t size;
    size_t capacity;
    functionCopy copy;
    functionCompareEq compare;
    funcionHash hash;
    funcionHash secondHash;
    functionPrint print;
    functionDelete del;
};

typedef struct _Hashtable* Hashtable;

/**
 * create a new empty hash table with the given functions. Return NULL on error.
 * Print function is optional; must be NULL if not used.
 */
Hashtable ht_create(functionCopy copy, functionCompareEq compare, funcionHash hash, funcionHash secondHash,
                    functionDelete del, functionPrint print);

/**
 * add data to the hashtable.
 */
void ht_add(Hashtable T, void* data);

/**
 * return 1 if data is in T, otherwise 0.
 */
int ht_search(Hashtable T, void* data);

/**
 * delete data from T. Return 1 if it was deleted, otherwise 0 (data wasn't in T).
 */
int ht_delete(Hashtable T, void* data);

/**
 * delete the hash table from memory.
 */
void ht_destroy(Hashtable T);

/**
 * print hash table.
 */
void ht_print(Hashtable T);


/**
 * hashtable t;
 * mainhash(x) = t->hash(x);
 *
 * step = R - (t->secondhash(x) % R), where R < table_size (which is prime)
 * therefore, gcm(step, table_size) = 1 and step > 0
 * 
 * prob_seq(i) = (mainhash(x) + i*step) % table_size
 * being table_size prime, pro_seq visits all cells.
 */

#endif /* __HASHTABLE_H__ */