#ifndef __FUNCS_H__
#define __FUNCS_H__

/**
 * Must return a copy of data.
 */
typedef void* (*functionCopy)(void* data);

/**
 * Must return 0 if data1 and data2 are equal, otherwise non-zero value.
 */
typedef int (*functionCompareEq)(void* data1, void* data2);

/**
 * Must return 0 if data1 == data2, less than 0 if data1 < data2 or greater
 * than 0 if data1 > data2
 */
typedef int (*functionCompareOrd)(void* data1, void* data2);

/**
 * Must delete data from memory.
 */
typedef void (*functionDelete)(void* data);

/**
 * Must print on screen the data.
 */
typedef void (*functionPrint)(void* data);

/**
 * Must return a hash value for data.
 */
typedef unsigned long (*funcionHash)(void* data);

#endif /* __FUNCS_H__ */