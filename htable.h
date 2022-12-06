#pragma once
typedef unsigned int uint;

typedef void* table;

table htablecreate(uint capacity);
void htabledestroy(table);
uint htableinsert(table p, char *key, void *ptr);
void htableprint(table t);
void* htablesearch(table t, char *key);
void htabledelete(table t, char *key);
