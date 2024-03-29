/* symtable hash implementation */
#include "symtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>

/* bucketCounts array holds all the possible configurations for the
the size of bucket array we can either start with/ expand to */
static const size_t bucketCounts[] = {509, 1021, 2039, 4093, 8191, 
16381, 32749, 65521};

/* denotes the min value for the number of buckets and the max number 
buckets for a SymTable */
enum BucketEnds{bucketMin = 509, bucketMax = 65521};

/* Bindings can be formed to make a list of Bindings and hold
certain variables: key, value, next. */ 
struct Binding {
    /* char pointer to the key */
    const char *key;
    /* void pointer to the value */
    const void* value;
    /* pointer to the next Binding in list */
    struct Binding *next;
}; 

/* SymTable points first to the buckets by which the lists of
Bindings can be hashed too and specifically the head Binding of the
first list of Bindings. Symtable also holds variables regarding the
entire hash table such as: bucketSize and bindingsSize.*/
struct SymTable {
    /* double pointer pointing to the head node of the first
    linked list in the array of linked lists */
    struct Binding **head;
    /* holds how many buckets are being used in the SymTable*/
    size_t bucketSize;
    /* keeps track of how many Bindings total are in the SymTable */
    size_t bindingsSize;
}; 

/* this function takes in parameters const char pointer pcKey and 
size_t uBucketCount and returns a hash index as a type size_t */
static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   /* hash function */ 
   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

SymTable_T SymTable_new(void){
   struct SymTable *oSymTable = malloc(sizeof(struct SymTable));
   if(oSymTable == NULL){
      return NULL;
   }
   oSymTable->bucketSize = bucketMin;
   oSymTable->bindingsSize = 0;
   oSymTable->head = calloc(oSymTable->bucketSize, sizeof(oSymTable->head));
   return oSymTable;
}

void SymTable_free(SymTable_T oSymTable){
   struct Binding *free_node;
   struct Binding *next_node;
   size_t i;

   assert(oSymTable != NULL);

   for(i = 0; i < (size_t)oSymTable->bucketSize; i++){
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
   return oSymTable->bindingsSize;
}

/* SymTable_expand takes in a parameter of a SymTable oSymTable. The 
function first calculates the desired bucketSize to expand to. If the
bucketSize cannot be expanded, the function returns 0 and leaves
the SymTable unchanged. If oSymTable can be expanded, a new array is
instantiated and all the previous bindings are rehashed into the new 
set of buckets. However, if there is insufficient memory for a new 
array of buckets, the function returns 0. If there is memory, and the 
previous bindings are rehashed, the old array of buckets are freed,  
oSymTable points to the new set of buckets, and 1 is returned. */
static size_t SymTable_expand(SymTable_T oSymTable) {
    size_t i;
    size_t oldBucketCount;
    size_t newBucket;
    struct Binding **newHead;
    struct Binding *currNode;
    struct Binding *nextNode;
 
    assert(oSymTable != NULL);

    oldBucketCount = oSymTable->bucketSize;
    /*checks if SymTable can be expanded further */
    if(oldBucketCount == bucketMax){
        return 0;
    }
    /* determines the next size of buckets to expand to */
    for (i = 1; i < sizeof(bucketCounts); i++) {
        if (bucketCounts[i] > oSymTable->bucketSize) {
            oSymTable->bucketSize = bucketCounts[i];
            break;
        }
    }

    /* allocates new array of buckets */
    newHead = calloc(oSymTable->bucketSize, sizeof(struct Binding*));
    
    /*checks for insufficient memory allocation */
    if (newHead == NULL) {
        return 0;
    }

    /* rehash old bindings to new array of buckets */
    for (i = 0; i < oldBucketCount; i++) {
        currNode = oSymTable->head[i];
        while (currNode != NULL) {
            nextNode = currNode->next;
            newBucket = SymTable_hash(currNode->key,oSymTable->bucketSize);
            currNode->next = newHead[newBucket];
            newHead[newBucket] = currNode;
            currNode = nextNode;
        }
    }

    free(oSymTable->head);

    oSymTable->head = newHead;
    return 1;
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
                 const void *pvValue) {
   size_t bucket; 
   struct Binding *nNode;
   struct Binding *currNode;
   void *defCopy;

   assert(oSymTable != NULL);
   assert(pcKey != NULL); 

   bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   nNode = malloc(sizeof(struct Binding));
   currNode = oSymTable->head[bucket];
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
   nNode->next = oSymTable->head[bucket];

   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         free(defCopy);
         free(nNode);
         return 0;
      }
      currNode = currNode->next;
   }
   oSymTable->head[bucket] = nNode;
   oSymTable->bindingsSize++;
   if(oSymTable->bindingsSize > oSymTable->bucketSize)
   {
     SymTable_expand(oSymTable);
   }
   return 1;
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
                       const void *pvValue){
   struct Binding *currNode;
   void* oldValue;
   size_t bucket; 

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
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
   size_t bucket;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
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
   struct Binding *currNode; 
   struct Binding *prev;
   size_t bucket;
   void* value;

   assert(oSymTable != NULL);
   assert(pcKey != NULL);

   bucket = SymTable_hash(pcKey,oSymTable->bucketSize);
   currNode = oSymTable->head[bucket];
   prev = NULL;
   while (currNode != NULL) {
      if (strcmp(currNode->key, pcKey) == 0) {
         value = (void*)currNode->value;
         oSymTable->bindingsSize--;

         if (prev != NULL) {
            prev->next = currNode->next;
         } else {
            oSymTable->head[bucket] = currNode->next;
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
   size_t i;
 
   assert(oSymTable != NULL);
   assert(pfApply != NULL);

    for (i = 0; i < (size_t)oSymTable->bucketSize; i++){
      currNode = oSymTable->head[i];
      while (currNode != NULL) {
        (*pfApply)(currNode->key, (void*)currNode->value, (void*)pvExtra);
        currNode = currNode->next;
      }
   }
}


