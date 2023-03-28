/* hashmap symtable implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

enum {BUCKET_COUNT = 509};

struct Binding {
    const char *key;
    int value;
    struct Binding *next;
}; 

struct Table {
    struct Binding *buckets[BUCKET_COUNT];
    size_t size;
}; 

static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

SymTable_T SymTable_new(void){
    struct SymTable *oSymTable = malloc(sizeof(struct Table));
    if(oSymTable == NULL){
        return NULL;
    }
    for (int i = 0; i < BUCKET_COUNT; i++) {
        oSymTable->buckets[i] = NULL;
    }

    return oSymTable; 
}    

void SymTable_free(SymTable_T oSymTable){
    assert(oSymTable != NULL);
    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        struct Binding *current = oSymTable->buckets[i];
        while (current != NULL) {
            struct Binding *next = current->next;
            free(current);
            current = next;
        }
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
   assert(oSymTable != NULL);
   return oSymTable->size;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue){
    assert(oSymTable != NULL && pcKey != NULL && pvValue != NULL);
    size_t bucketIndex = SymTable_hash(pcKey, BUCKET_COUNT);
    struct Binding *current = oSymTable->buckets[bucketIndex];
    while (current != NULL) {
        if (strcmp(current->key, pcKey) == 0) {
            current->value = (int)pvValue;
            return 0;
        }
        current = current->next;
    }

    struct Binding *newBinding = malloc(sizeof(struct Binding));
    if (newBinding == NULL) {
        return 1;
    }

    newBinding->key = pcKey;
    newBinding->value = (int)pvValue;
    newBinding->next = oSymTable->buckets[bucketIndex];
    oSymTable->buckets[bucketIndex] = newBinding;
    return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
                       const void *pvValue){
    assert(oSymTable != NULL && pcKey != NULL && pvValue != NULL); 
    size_t bucketIndex = SymTable_hash(pcKey, BUCKET_COUNT);
    struct Binding *current = oSymTable->buckets[bucketIndex];
    void* oldValue;
    while (current != NULL) {
        if (strcmp(current->key, pcKey) == 0) {
            oldValue = (void*)current->value;
            current->value = (int)pvValue;
            return oldValue;
        }
        current = current->next;
    }
    return NULL;

}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    assert(oSymTable != NULL && pcKey != NULL); 
    size_t bucketIndex = SymTable_hash(pcKey, BUCKET_COUNT);
    struct Binding *current = oSymTable->buckets[bucketIndex];  
    while (current != NULL) {
        if (strcmp(current->key, pcKey) == 0) {
            return 1;
        }
        current = current->next;
    } 
    return 0;      
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    assert(oSymTable != NULL && pcKey != NULL);
    size_t bucketIndex = SymTable_hash(pcKey, BUCKET_COUNT);
    struct Binding *current = oSymTable->buckets[bucketIndex];  
    while (current != NULL) {
        if (strcmp(current->key, pcKey) == 0) {
            return (void*)current->value;
        }
        current = current->next;
    } 
    return NULL;      
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    assert(oSymTable != NULL);
    assert(pcKey != NULL);  
    size_t bucketIndex = SymTable_hash(pcKey, BUCKET_COUNT);
    struct Binding *current = oSymTable->buckets[bucketIndex];  
    struct Binding *previous = NULL;
    void *value = (void*)current->value;
    while (current != NULL) {
        if (strcmp(current->key, pcKey) == 0) {
            if (previous == NULL) {
                oSymTable->buckets[bucketIndex] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return value; 
        }
        previous = current;
        current = current->next;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable, 
                  void (*pfApply)(const char *pcKey, 
                  void *pvValue, void *pvExtra),
                  const void *pvExtra){ 
    assert(oSymTable != NULL && pfApply != NULL);
    for (size_t i = 0; i < BUCKET_COUNT; i++) {
        struct Binding *current = oSymTable->buckets[i];
        while (current != NULL) {
            (*pfApply)(current->key, (void*)current->value, (void*)pvExtra);
            current = current->next;
        }
    }
    
}
