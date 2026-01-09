#include "list.h"
#include <stdio.h>
#include <stdlib.h>


List list_create(functionCopy copy, functionCompareEq compare, functionDelete del, functionPrint print)
{
    if (!copy || !compare || !del)
        return NULL;
    
    List list = malloc(sizeof(struct _list));
    if (!list) {
        fprintf(stderr, "NULL return by malloc in list_create\n");
        return NULL;
    }

    list->list = NULL;
    list->copy = copy;
    list->compare = compare;
    list->del = del;
    list->print = print;

    return list;
}

int list_is_empty(List list) {
    return list->list == NULL;
}

void list_add(List list, void* data) {
    if (!list) {
        fprintf(stderr, "argument list given is NULL\n");
        return;
    }
    Node new = malloc(sizeof(struct _Node));
    if (!new) {
        fprintf(stderr, "NULL return by malloc in list_add\n");
        return;
    }
    new->data = list->copy(data);
    new->next = list->list;
    list->list = new;
}


void* list_head(List list) {
    if (!list) {
        fprintf(stderr, "NULL return by malloc in list_create\n");
        return NULL;
    }

    if (!list->list)
        return NULL;

    void* head = list->list->data;
    list->list = list->list->next;

    return head;
}

void list_print(List list) {
    if (!list) {
        fprintf(stderr, "argument list given is NULL\n");
        return;
    }
    if (!list->print)
        return;
    for (Node temp = list->list; temp != NULL; temp = temp->next)
        list->print(temp->data);
}


void list_destroy(List list) {
    if (list) {
        Node node = list->list;
        while (node != NULL) {
            list->del(node->data);
            Node temp = node;
            node = node->next;
            free(temp);
        }
        free(list);
    }
}