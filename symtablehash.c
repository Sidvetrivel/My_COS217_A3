/* hash symtable implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

struct Binding {
    const char *key;
    const void* value;
    struct Binding *next;
}; 

struct Table {
    struct Binding **head;
    size_t bucketSize;
    size_t bindingsSize;
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
   struct Table *oSymTable = malloc(sizeof(struct Table));
   oSymTable->bucketSize = 509;
   oSymTable->bindingsSize = 0;
   if(oSymTable == NULL){
      return NULL;
   }
   oSymTable->head = calloc(oSymTable->bucketSize, sizeof(oSymTable->head));
   return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
   struct Binding *free_node;
   struct Binding *next_node;
   assert(oSymTable != NULL);
   for (size_t i = 0; i < (size_t)oSymTable->bucketSize; i++){
      free_node = oSymTable->head[i];
      while (free_node != NULL) {
        free((char *)free_node->key);
        next_node = free_node->next;
        free(free_node);
        free_node = next_node;
      }
   }
   free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
   assert(oSymTable != NULL);
   return oSymTable->bindingSize;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue) {
   assert((oSymTable != NULL) || (pcKey != NULL) || (pvValue != NULL));
   size_t bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   struct Binding *nNode = malloc(sizeof(struct Binding));
   struct Binding *currNode = oSymTable->head[bucket];
   void *defCopy = malloc(strlen(pcKey)+1);

   /* alloc failure?*/
   if (nNode == NULL) {
      return 0;
   }

   /*alloc failure?*/
   if (defCopy == NULL) {
      free(nNode);
      return 0;
   }

   strcpy(defCopy, pcKey);
   nNode->key = defCopy;
   nNode->value = pvValue;
   nNode->next = oSymTable->head[bucket];

   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         free(defCopy);
         free(nNode);
         return 0;
      }
      currNode = currNode->next;
   }
   oSymTable->head = nNode;
   oSymTable->bindingSize++;
   return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue){
   struct Binding *currNode;
   void* oldValue;
   size_t bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   assert(oSymTable != NULL);
   assert(pcKey != NULL);
   currNode = oSymTable->head[bucket];
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
   struct Binding *currNode;
   size_t bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   assert(oSymTable != NULL);
   assert(pcKey != NULL);
   currNode = oSymTable->head[bucket];
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         return 1;
      }
      currNode = currNode->next;
   }
   return 0;
}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
   struct Binding *currNode;
   size_t bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   assert(oSymTable != NULL);
   assert(pcKey != NULL);
   currNode = oSymTable->head[bucket];
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         return (void*)currNode->value;
      }
      currNode = currNode->next;
   }
   return NULL;
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
   struct Binding *currNode = NULL;
   struct Binding *prev = NULL;
   size_t bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   void* value;
   assert(oSymTable != NULL);
   assert(pcKey != NULL);
   currNode = oSymTable->head[bucket];
   
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         value = (void*)currNode->value;
         oSymTable->bindingSize--;

         if (prev != NULL) {
            prev->next = currNode->next;
         } else {
            /* removing the head node */
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
   struct Binding *currNode;
   assert((oSymTable != NULL) || (pfApply != NULL));
    for (size_t i = 0; i < (size_t)oSymTable->bucketSize; i++){
      currNode = oSymTable->head[i];
      while (currNode != NULL) {
        (*pfApply)(currNode->key, (void*)currNode->value, (void*)pvExtra);
        currNode = currNode->next;
      }
   }
}
