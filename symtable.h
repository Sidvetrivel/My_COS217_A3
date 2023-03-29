#include <stddef.h>
#ifndef SYMTABLELIST_INCLUDED
#define SYMTABLELIST_INCLUDED

struct SymTable;
typedef struct SymTable *SymTable_T;

/*instantiates a new SymTable object and returns a pointer Symtable_T
to the new SymTable object */
SymTable_T SymTable_new(void);

void SymTable_free(SymTable_T oSymTable);

size_t SymTable_getLength(SymTable_T oSymTable);

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue);

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
                       const void *pvValue);

int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

void SymTable_map(SymTable_T oSymTable, 
                  void (*pfApply)(const char *pcKey, 
                  void *pvValue, void *pvExtra),
                  const void *pvExtra);

/* when called, the expand function will create a new array of
buckets with double the number of buckets, rehash all the previous 
bindings to the new buckets, free the old array of buckets, and point 
the head pointer of the SymTable to the new array of buckets.*/
int SymTable_expand(SymTable_T oSymTable);

#endif




