/* symtable linkedList implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

/* Node is a struct that can be linked together to form 
a list of Nodes and holds certain variables: key, value, next */
struct Node {
    /* char pointer to the key */
    const char *key;
    /* void pointer to the value */
    const void* value;
    /* pointer to the next Node in list */
    struct Node *next;
}; 

/* SymTable is a struct that points to the head/first Node in 
a list of Nodes and holds other data about the linked list (size) */
struct SymTable {
    /* pointer pointing to the first node of the list */
    struct Node *head;
    /* size holds the length of a list of Nodes in SymTable */
    size_t size;
}; 

SymTable_T SymTable_new(void){
   struct SymTable *oSymTable = malloc(sizeof(struct SymTable));
   if(oSymTable == NULL){
      return NULL;
   }
   oSymTable->head = NULL;
   oSymTable->size = 0;
   return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
   struct Node *free_node;
   struct Node *next_node;

   assert(oSymTable != NULL);

   free_node = oSymTable->head;
   while (free_node != NULL) {
      free((char *)free_node->key);
      next_node = free_node->next;
      free(free_node);
      free_node = next_node;
   }
   free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
   assert(oSymTable != NULL);
   return oSymTable->size;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue) {
   struct Node *nNode; 
   struct Node *currNode; 
   void *defCopy;

   assert(oSymTable != NULL);
   assert(pcKey != NULL); 

   nNode = malloc(sizeof(struct Node));
   currNode = oSymTable->head;
   defCopy = malloc(strlen(pcKey)+1);

   if (nNode == NULL) {
      return 0;
   }
   if (defCopy == NULL) {
      free(nNode);
      return 0;
   }

   strcpy(defCopy, pcKey);
   nNode->key = defCopy;
   nNode->value = pvValue;
   nNode->next = oSymTable->head;

   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         free(defCopy);
         free(nNode);
         return 0;
      }
      currNode = currNode->next;
   }
   oSymTable->head = nNode;
   oSymTable->size++;
   return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue){
   struct Node *currNode;
   void* oldValue;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   currNode = oSymTable->head;
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
   struct Node *currNode;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   currNode = oSymTable->head;
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         return 1;
      }
      currNode = currNode->next;
   }
   return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
   struct Node *currNode;
   
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   currNode = oSymTable->head;
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         return (void*)currNode->value;
      }
      currNode = currNode->next;
   }
   return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
   struct Node *currNode;
   struct Node *prev;
   void* value;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   currNode = oSymTable->head;
   prev = NULL;
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         value = (void*)currNode->value;
         oSymTable->size--;

         if (prev != NULL) {
            prev->next = currNode->next;
         } else {
            oSymTable->head = currNode->next;
         }
         free((void*)currNode->key);
         free((void*)currNode);

         return value;
     }
     prev = currNode;
     currNode = currNode->next;
    }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
                  void (*pfApply)(const char *pcKey,
                                  void *pvValue, void *pvExtra),
                  const void *pvExtra){
   struct Node *currNode;

   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   currNode = oSymTable->head;
   while (currNode != NULL) {
      (*pfApply)(currNode->key, (void*)currNode->value, (void*)pvExtra);
      currNode = currNode->next;
   }
}
