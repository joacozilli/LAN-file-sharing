#ifndef __STR_H__
#define __STR_H__

/**
 * return a copy in memory of the string.
 */
void* str_copy(void* s);

/**
 * return 0 if s1, s2 are equal, < 0 if s1 is less than s2
 * and > 0 f s1 is greater than s2.
 */
int str_compare(void* s1, void* s2);

/**
 * delete string from memory.
 */
void str_delete(void* s);

/**
 * print string with new line at the end.
 */
void str_print(void* s);

/**
 * djb2 hash function.
 */
unsigned long str_hash1(void* str);


/**
 * sdbm hash function.
 */
unsigned long str_hash2(void* str);
#endif /* __STR_H__ */