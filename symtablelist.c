/* linkedlist symtable implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    const char *key;
    int value;
    struct Node *next;
};

struct SymTable_T {
    struct Node *head;
    size_t size;
}; 

SymTable_T SymTable_new(void){
    SymTable_T *oSymTable = malloc(sizeof(struct SymTable_T));
    assert(oSymTable != NULL);
    oSymTable->head = NULL;
    oSymTable->size = 0;
    return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
    assert(oSymTable != NULL);
    struct Node *free_node = oSymTable->head;
    while (free_node != NULL) {
        struct Node *next_node = free_node->next;
        free((char *)free_node->key);
        free(free_node->value);
        free(free_node);
        free_node = next_node;
    }
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
    assert(oSymTable != NULL);
    size_t length;
    struct Node *length_node = oSymTable->head;
    while (length_node != NULL) {
        struct Node *next_node = length_node->next;
        length++;
        length_node = next_node;
    }
    oSymTable->size = length;
    return oSymTable->size;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
                 const void *pvValue){
    assert(oSymTable != NULL && pcKey != NULL && pvValue != NULL);
    struct Node *nNode = malloc(sizeof(struct Node));
    struct Node *currNode = oSymTable->head;

    if (oSymTable->head == NULL) {
        oSymTable->head = nNode;
        return 1;
    }

    while (currNode != NULL) {
        if (strcmp(currNode->key, pcKey) == 0) {
            // Found a node with the same key, update its value and return
            currNode->value = pvValue;
        }
        currNode = currNode->next;
    }
    currNode = oSymTable->head;
    while (currNode->next != NULL) {
        currNode = currNode->next;
    }
    currNode->next = nNode;
    return 0;

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