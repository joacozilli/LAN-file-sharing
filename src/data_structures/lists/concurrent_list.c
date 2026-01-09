#include "concurrent_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


ConcurrentList concurrent_list_create(functionCopy copy, functionCompareEq compare, functionDelete del, functionPrint print)
{
    if (!copy || !compare || !del)
        return NULL;

    ConcurrentList conclist = malloc(sizeof(struct _ConcurrentList));
    if (!conclist) {
        fprintf(stderr, "NULL return by malloc in concurrent_list_create\n");
        return NULL;
    }

    conclist->list = list_create(copy, compare, del, print);
    if (!conclist->list) {
        fprintf(stderr, "NULL return by list_create in concurrent_list_create\n");
    }

    if (pthread_mutex_init(&conclist->mutex, NULL) != 0) {
        fprintf(stderr, "pthread_mutex_init in concurrent_list_create: %s\n", strerror(errno));
    }

    return conclist;
}

int concurrent_list_is_empty(ConcurrentList conclist) {
    pthread_mutex_lock(&conclist->mutex);
    int res = list_is_empty(conclist->list);
    pthread_mutex_unlock(&conclist->mutex);
    return res;
}

void concurrent_list_add(ConcurrentList conclist, void* data) {
    pthread_mutex_lock(&conclist->mutex);
    list_add(conclist->list, data);
    pthread_mutex_unlock(&conclist->mutex);
}

void* concurrent_list_head(ConcurrentList conclist) {
    pthread_mutex_lock(&conclist->mutex);
    void* res = list_head(conclist->list);
    pthread_mutex_unlock(&conclist->mutex);
    return res;  
}

void concurrent_list_print(ConcurrentList conclist) {
    pthread_mutex_lock(&conclist->mutex);
    list_print(conclist->list);
    pthread_mutex_unlock(&conclist->mutex);  
}

void concurrent_list_destroy(ConcurrentList conclist) {
    pthread_mutex_lock(&conclist->mutex);
    list_destroy(conclist->list);
    pthread_mutex_unlock(&conclist->mutex);
    pthread_mutex_destroy(&conclist->mutex);
    free(conclist);
}