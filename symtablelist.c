/* linkedlist symtable implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

struct Node {
    const char *key;
    const void* value;
    struct Node *next;
}; 

struct SymTable {
    struct Node *head;
    size_t size;
}; 

SymTable_T SymTable_new(void){
    struct SymTable *oSymTable = malloc(sizeof(struct SymTable));
    assert(oSymTable != NULL);
    oSymTable->head = NULL;
    oSymTable->size = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
    struct Node *free_node = oSymTable->head;
    struct Node *next_node = free_node->next;
    assert(oSymTable != NULL);
    while (free_node != NULL) {
        free((char *)free_node->key);
        free((void *)free_node->value);
        free(free_node);
        free_node = next_node;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
    struct Node *length_node = oSymTable->head;
    size_t count = 0;
    assert(oSymTable != NULL);
    while (length_node != NULL) {
        count++;
        length_node = length_node->next;
    }
    return count;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue){
    struct Node *nNode = malloc(sizeof(struct Node));
    struct Node *currNode = oSymTable->head;
    void *defCopy = malloc(sizeof(*pcKey));
    strcpy(defCopy,pcKey);
    assert(oSymTable != NULL && pcKey != NULL && pvValue != NULL);
    if(oSymTable->head == NULL){
        oSymTable->head = nNode;
        nNode->key = defCopy;
        nNode->value = pvValue;
        oSymTable->size++;
        return 1;
    }
    else {
        while (currNode->next != NULL) {
            if (strcmp(currNode->key, pcKey) == 0){
                return 0;
            }
            currNode = currNode->next;
        }
        currNode->next = nNode;
        nNode->key = defCopy;
        nNode->value = pvValue;
        oSymTable->size++;
        return 1;
    }
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey, 
                       const void *pvValue){
    struct Node *currNode = oSymTable->head;
    void* oldValue;
    assert(oSymTable != NULL && pcKey != NULL && pvValue != NULL);
    while (currNode != NULL) {
        if (strcmp(currNode->key, pcKey) == 0) {
            oldValue = (void*)currNode->value;
            currNode->value = pvValue;
            return oldValue;
        }
        currNode = currNode->next;
    }
    return NULL;           
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    struct Node *currNode = oSymTable->head;
    assert(oSymTable != NULL && pcKey != NULL);
    while (currNode != NULL) {
        if (strcmp(currNode->key, pcKey) == 0) {
            return 1;
        }
        currNode = currNode->next;
    } 
    return 0;           
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct Node *currNode = oSymTable->head;
    assert(oSymTable != NULL && pcKey != NULL);
    while (currNode != NULL) {
        if (strcmp(currNode->key, pcKey) == 0) {
            return (void*)currNode->value;
        }
        currNode = currNode->next;
    } 
    return NULL;     
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct Node *remNode = oSymTable->head;
    assert(oSymTable != NULL && pcKey != NULL);
    while (remNode != NULL) {
        if (strcmp(remNode->key, pcKey) == 0) {
            remNode->key = NULL;
            return (void*)remNode->value;
        }
        remNode = remNode->next;
    } 
    return NULL;     

}

void SymTable_map(SymTable_T oSymTable, 
                  void (*pfApply)(const char *pcKey, 
                  void *pvValue, void *pvExtra),
                  const void *pvExtra){
    struct Node *currNode = oSymTable->head;
    assert(oSymTable != NULL && pfApply != NULL);
    while (currNode != NULL) {
        (*pfApply)(currNode->key, (void*)currNode->value, (void*)pvExtra);
        currNode = currNode->next;
    }
}
