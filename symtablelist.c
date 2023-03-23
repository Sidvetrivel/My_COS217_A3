/* linkedlist symtable implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    const char *key;
    void *value;
    struct Node *next;
};
struct SymTable_T {
    struct Node *head;
    size_t size;
};

struct SymTable_T SymTable_new(void){
    struct SymTable_T oSymTable; 
    oSymTable.head = malloc(sizeof(struct SymTable_T));
    assert(oSymTable.head != NULL);
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){

}

size_t SymTable_getLength(SymTable_T oSymTable){

}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue){
        

}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
                       const void *pvValue){
                    
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){

}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){

}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){

}

void SymTable_map(SymTable_T oSymTable, 
                  void (*pfApply)(const char *pcKey, 
                  void *pvValue, void *pvExtra),
                  const void *pvExtra){


}