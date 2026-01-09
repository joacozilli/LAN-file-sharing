#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>




Hashtable ht_create(functionCopy copy, functionCompareEq compare, funcionHash hash, funcionHash secondHash,
                    functionDelete del, functionPrint print)
{
    if (!copy || !compare || !hash || !secondHash || !del)
        return NULL;
    
    Hashtable t = malloc(sizeof(struct _Hashtable));
    if (!t) {
        fprintf(stderr, "NULL return by malloc in ht_create\n");
        return NULL;
    }
    t->capacity = INITIAL_CAPACITY;
    t->copy = copy;
    t->compare = compare;
    t->hash = hash;
    t->secondHash = secondHash;
    t->del = del;
    t->print = print;
    t->size = 0;

    t->elems = malloc(sizeof(Cell) * t->capacity);
    for (unsigned long i = 0; i < t->capacity; i++) {
        t->elems[i] = malloc(sizeof(struct _Cell));
        t->elems[i]->data = NULL;
        t->elems[i]->status = NO;
    }
    return t;
}


static int is_prime(int a) {
    if (a <= 1)
        return 0;
    for (int i = 2; i <= a / 2; i++)
        if (a % i == 0)
            return 0;
    return 1;
}
/**
 * increase the capacity of T and rehash all elements.
 */
static void rehash(Hashtable T) {
    size_t newCapacity = T->capacity*2;
    while (!is_prime(newCapacity))
        newCapacity++;

    Cell* arr = malloc(sizeof(Cell) * newCapacity);
    for (unsigned long i = 0; i < newCapacity; i++) {
        arr[i] = malloc(sizeof(struct _Cell));
        arr[i]->data = NULL;
        arr[i]->status = NO;
    }
    T->size = 0;
    for (unsigned long i = 0; i < T->capacity; i++) {

        if (T->elems[i]->data != NULL && T->elems[i]->status != DELETED) {
            void* data = T->elems[i]->data;
            unsigned long hashValue = T->hash(data);
            int idx = hashValue % newCapacity;
            unsigned long offset = (newCapacity-1) - (T->secondHash(data) % (newCapacity-1));

            for (int j = 1; arr[idx]->data != NULL; j++)
                idx = (hashValue + j*offset) % newCapacity;
            
            arr[idx]->data = T->copy(data);
            T->size++;
        }
        T->del(T->elems[i]->data);
        free(T->elems[i]);
    }
    free(T->elems);
    T->elems = arr;
    T->capacity = newCapacity;
}

/**
 * find a cell for data on the hash table. If data already exists, return the index of the
 * cell in which it is and put alreadyExists to 1, otherwise return the index of an 
 * available cell to add data and put alreadyExists to 0.
 */
static int find_cell(Hashtable T, void* data, int* alreadyExists) {

    unsigned long hashValue = T->hash(data);
    int idx = hashValue % T->capacity;
    if (T->elems[idx]->data == NULL) {
        *alreadyExists = 0;
        return idx;
    }
    if (!T->compare(data, T->elems[idx]->data)) {
        *alreadyExists = T->elems[idx]->status == NO;
        return idx;
     }

    /* start probing sequence. Must search until NULL found to verify if data already exists. */

    unsigned long offset = (T->capacity-1) - (T->secondHash(data) % (T->capacity-1));
    *alreadyExists = 0;
    int foundIdx = -1;
    idx = (hashValue + offset) % T->capacity;

    /**
     * if many deletions are done, could happen that there is no NULL cells but many DELETED cells,
     * therefore the need of third condition. Find_cell is inefficient in such case.
     */
    for (unsigned long i = 2; !(*alreadyExists) && T->elems[idx]->data != NULL && i <= T->capacity; i++) {

        if (T->elems[idx]->status == DELETED && foundIdx == -1)
            foundIdx = idx;

        else if (T->elems[idx]->status == NO && !T->compare(data, T->elems[idx]->data)) {
            *alreadyExists = 1;
            foundIdx = idx;
        }
        idx = (hashValue + i*offset) % T->capacity;
    }
    return foundIdx != -1 ? foundIdx : idx;
}


int ht_search(Hashtable T, void* data) {
    if(!T) {
        fprintf(stderr, "argument T given is NULL");
        return -1;
    }
    if (!data) {
        fprintf(stderr, "argument data given is NULL");
        return -1;
    }
    int found;
    find_cell(T, data, &found);
    return found;
}

void ht_add(Hashtable T, void* data) {
    if(!T) {
        fprintf(stderr, "argument T given is NULL");
        return;
    }
    if (!data) {
        fprintf(stderr, "argument data given is NULL");
        return;
    }
    double factor = (double) (T->size + 1) / (double) T->capacity;
    if (factor >= BALANCE_FACTOR)
        rehash(T);
    
    int found;
    int idx = find_cell(T, data, &found);
    if (found)
        return;
    assert(T->elems[idx] != NULL);
    if (T->elems[idx]->data != NULL)
        T->del(T->elems[idx]->data);
    T->elems[idx]->data = T->copy(data);
    T->elems[idx]->status = NO;
    T->size++;
}


int ht_delete(Hashtable T, void* data) {
    if(!T) {
        fprintf(stderr, "argument T given is NULL");
        return -1;
    }
    if (!data) {
        fprintf(stderr, "argument data given is NULL");
        return -1;
    }
    int found;
    int idx = find_cell(T, data, &found);
    if (!found)
        return 0;
    T->elems[idx]->status = DELETED;
    T->size--;
    return 1; 
}

void ht_destroy(Hashtable T) {
    if(!T)
        return;
    for (size_t i = 0; i < T->capacity; i++) {
        if (T->elems[i]->data != NULL)
            T->del(T->elems[i]->data);
        free(T->elems[i]);
    }
    free(T->elems);
    free(T);
}

void ht_print(Hashtable T) {
    if (!T) {
        fprintf(stderr, "argument T given is NULL");
        return;
    }
    if (!T->print)
        return;
    for (size_t i = 0; i < T->capacity; i++) {
        if (T->elems[i]->data != NULL && T->elems[i]->status == NO)
            T->print(T->elems[i]->data);
        else
            printf("NULL\n");
    }
}