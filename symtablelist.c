/* linkedlist symtable implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

struct Node {
    const char *key;
    int value;
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
        free((int*)free_node->value);
        free(free_node);
        free_node = next_node;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
    size_t length;
    struct Node *length_node = oSymTable->head;
    struct Node *next_node = length_node->next;
    assert(oSymTable != NULL);
    while (length_node != NULL) {
        length++;
        length_node = next_node;
    }
    oSymTable->size = length;
    return oSymTable->size;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue){
    struct Node *nNode = malloc(sizeof(struct Node));
    struct Node *currNode = oSymTable->head;
    assert(oSymTable != NULL && pcKey != NULL && pvValue != NULL);
    while (currNode->next != NULL) {
        currNode = currNode->next;
    }
    currNode->next = nNode;
    return 0;

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
            return currNode->value;
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
            return remNode->value;
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
        (*pfApply)(currNode->key, currNode->value, pvExtra);
        currNode = currNode->next;
    }
}
