#ifndef __CONCURRENT_LIST_H__
#define __CONCURRENT_LIST_H__

#include "list.h"
#include <pthread.h>

struct _ConcurrentList {
    List list;
    pthread_mutex_t mutex;
};

typedef struct _ConcurrentList* ConcurrentList;

/**
 * create new empty concurrent list with the given functions. Return NULL on error.
 * Print function is optional; must be NULL if not used.
 */
ConcurrentList concurrent_list_create(functionCopy copy, functionCompareEq compare, functionDelete del, functionPrint print);

/**
 * return 1 if conclist is empty, otherwise 0.
 */
int concurrent_list_is_empty(ConcurrentList conclist);

/**
 * add data to the head of conclist.
 */
void concurrent_list_add(ConcurrentList conclist, void* data);

/**
 * remove and return head of conclist. Return NULL if conclist is empty.
 */
void* concurrent_list_head(ConcurrentList conclist);

/**
 * Print conclist.
 */
void concurrent_list_print(ConcurrentList conclist);

/**
 * delete conclist from memory.
 */
void concurrent_list_destroy(ConcurrentList conclist);

#endif /* __CONCURRENT_LIST_H__ */