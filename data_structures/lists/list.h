#ifndef __LIST_H__
#define __LIST_H__

#include "funcs.h"

struct _Node {
    void* data;
    struct _Node* next;
};
typedef struct _Node* Node;

struct _list {
    Node list;

    functionCopy copy;
    functionCompareEq compare;
    functionDelete del;
    functionPrint print;
};
typedef struct _list* List;

/**
 * create new empty list with the given functions. Return NULL on error.
 * Print function is optional; must be NULL if not used.
 */
List list_create(functionCopy copy, functionCompareEq compare, functionDelete del, functionPrint print);

/**
 * return 1 if list is empty, otherwise 0.
 */
int list_is_empty(List list);

/**
 * add data to the head of list.
 */
void list_add(List list, void* data);

/**
 * remove and return head of list. Return NULL if list is empty.
 */
void* list_head(List list);

/**
 * Print list.
 */
void list_print(List list);

/**
 * delete list from memory.
 */
void list_destroy(List list);


#endif /* __LIST_H__ */