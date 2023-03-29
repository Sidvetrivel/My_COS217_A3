/*symtable header file*/
#include <stddef.h>
#ifndef SYMTABLELIST_INCLUDED
#define SYMTABLELIST_INCLUDED

/* struct SymTable initialization*/
struct SymTable;

/* pointer SymTable_T to object SymTable initialization */
typedef struct SymTable *SymTable_T;

/* SymTable_new returns a new SymTable and returns a pointer to the 
new SymTable object if there is sufficient memory and null if there
is insufficient memory */
SymTable_T SymTable_new(void);

/* SymTable_free takes a SymTable object oSymTable and frees all 
the memory that the object occupies */ 
void SymTable_free(SymTable_T oSymTable);

/* SymTable_getLength takes in a SymTable object oSymTable
and returns the number of bindings in the SymTable object */
size_t SymTable_getLength(SymTable_T oSymTable);

/* SymTable_put takes in SymTable object oSymTable, a const char 
pointer pcKey, and const void pointer pvValue. If there is insufficient
memory for a put, the function leaves oSymTable unchanged and 
returns 0. If oSymTable doesn't contain pcKey, a new binding is added 
with the key pcKey and value pvValue. Then it returns 1 but 
if there is an instance of pcKey in oSymTable, the function returns 
0 and doesn't change the SymTable. */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue);

/* SymTable_replace takes in SymTable object oSymTable, a const char 
pointer pcKey, and const void pointer pvValue. If oSymtable contains a 
binding with key pcKey, it replaces the value with pvValue and returns
the old value as a void pointer. If a binding with key pcKey is not in
oSymTable, the function leaves oSymTable unchanged and returns NULL.*/
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
                       const void *pvValue);

/* SymTable_contains takes in SymTable object oSymTable and a const 
char pointer pcKey. The function returns 1 if oSymTable containts a
binding with a key pcKey and 0 if a binding with the key pcKey is
not found.*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* SymTable_get takes in SymTable object oSymTable and a const 
char pointer pcKey. The function returns the value of a particular 
binding in oSymTable with the key pcKey. If the binding is not found,
the function returns NULL.*/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* SymTable_remove takes in SymTable object oSymTable and a const 
char pointer pcKey. The function removes the binding from oSymTable 
that has the key pcKey and returns the value of the binding. If the 
binding to be removed is not found, the funciton returns NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* SymTable_map takes in a SymTable object oSymTable, a const char 
pointer pcKey, 2 void pointers pvValue and pvExtra, and a cons void
pointer pvExtra. The function then applies the function *pfApple to 
every binding in oSymTable while passing pvExtra as a parameter to 
*pfApply.*/
void SymTable_map(SymTable_T oSymTable, 
                  void (*pfApply)(const char *pcKey, 
                  void *pvValue, void *pvExtra),
                  const void *pvExtra);

#endif




