/* symtable linkedList implementation */
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

/* Symtable struct which holds the pointer to the head node of the new
linked-list as well as a variable size_t which denotes how many Nodes 
are currently in the SymTable */
struct SymTable {
    struct Node *head;
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

   /*assert*/ 
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

   /*assert*/ 
   assert(oSymTable != NULL);
   assert(pcKey != NULL); 

   nNode = malloc(sizeof(struct Node));
   currNode = oSymTable->head;
   defCopy = malloc(strlen(pcKey)+1);

   /* alloc failure? (nNode)*/
   if (nNode == NULL) {
      return 0;
   }
   /*alloc failure? (defCopy)*/
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

   /*assert*/ 
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
   
   /*assert*/ 
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
   
   /*assert*/ 
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

   /*assert*/ 
   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   currNode = oSymTable->head;
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

   /*assert*/ 
   assert(oSymTable != NULL);
   assert(pfApply != NULL);

   currNode = oSymTable->head;
   while (currNode != NULL) {
      (*pfApply)(currNode->key, (void*)currNode->value, (void*)pvExtra);
      currNode = currNode->next;
   }
}
