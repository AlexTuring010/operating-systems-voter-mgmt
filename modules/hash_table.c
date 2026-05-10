// This file was written by Alex Gkiafis, sdi2200284

#include "./header_files/hash_table.h"
#include <stdio.h>
#include <stdlib.h>

// For the implementation of this hash table I use a Linear Hasing Table with primary and overflow buckets
// It's size will increase dynamically, through a process explained in this pdf: https://www.alexdelis.eu/LinearHashing-CD21.pdf
// Looks interesting, and it gives an avarage search time complexity of O(1), so its great for searching stuff
// I have also made it so it handles malloc fails are reverses all changes it made before the fail happened.

typedef struct bucket{
    Item* items;
    Key* keys;
    int numOfItems;
    struct bucket* next;
} Bucket;

typedef struct bucketsHandler{
    int num_of_buckets;
    int numOfItems;
    Bucket* head;
    Bucket* tail;
} BucketsHandler;

// From header file: typedef struct hash_table* HashTable;
typedef struct hash_table{
    int bucketentries;              // How many keys can a bucket store?
    int pointer;                    // Points to the bucket that will split next
    int round;                      // At which round of hashing are we currently at
    int initial_buckets;            // How many primary buckets did we start with
    int prime_buckets;              // How many primary buckets does the hash table have now
    int numOfItems;                 // Number of items that are stored in the hash table
    int primary_buckets_capacity;   // Capacity for items that can be stored in primary buckets
    float load_ratio;               // load_ratio = numOfItems / primary_buckets_capacity
    BucketsHandler** buckets;       // An array of buckets handlers
} TableHandler;

// To keep track of changes durign ths split I decided to create a quick linked list
// kind of structure, that stores the changes I make so I can then back track my steps

typedef struct changesNode{         
    Key key;
    BucketsHandler* from;
    BucketsHandler* to;
    struct changesNode* next;
} changesNode;

typedef struct changesLogs{
    int changes;
    changesNode* start;
    changesNode* end;
} changesLogs;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Changes logs functions ///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

changesLogs* createChangesLogs(){
    changesLogs* newChLogs = malloc(sizeof(changesLogs));
    if(newChLogs == NULL) return NULL;
    newChLogs->changes = 0;
    newChLogs->start = NULL;
    newChLogs->end = NULL;
    return newChLogs;
}

changesNode* createChangesNode(Key key, BucketsHandler* from, BucketsHandler* to){
    changesNode* nodePtr = malloc(sizeof(changesNode));
    if(nodePtr == NULL) return NULL;
    nodePtr->key = key;
    nodePtr->from = from;
    nodePtr->to = to;
    nodePtr->next = NULL;
    return nodePtr;
}

int insertChangesLogs(changesLogs* chLogs, Key key, BucketsHandler* from, BucketsHandler* to){
    // Use to insert a change record to the logs

    changesNode* newChNodePtr = createChangesNode(key, from, to);
    if(newChNodePtr == NULL) return 0;

    if(chLogs->changes == 0){
        chLogs->start = newChNodePtr;
        chLogs->end = chLogs->start;
    } else{
        chLogs->end->next = newChNodePtr;
        chLogs->end = chLogs->end->next;
    }

    chLogs->changes += 1;
    return 1;
}

changesNode* extractLast(changesLogs* chLogs){
    // Use to extract last change record from the logs

    changesNode* chNode = chLogs->start;
    changesNode* prevNode = NULL;
    while(chNode != chLogs->end){
        prevNode = chNode;
        chNode = chNode->next;
    }
    chLogs->end = prevNode;
    if(prevNode == NULL){
        chLogs->start = NULL;
    } else{
        prevNode->next = NULL;
    }
    chLogs->changes -= 1;
    return chNode;
}

void destroyChangeLogs(changesLogs* chLogs){
    changesNode *temp, *chNodePtr = chLogs->start;
    while(chNodePtr != NULL){
        temp = chNodePtr->next;
        free(chNodePtr);
        chNodePtr = temp;
    }
    free(chLogs);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// General use functions  ///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int power(int num, int power){
    int result = 1;
    for(int i = 1; i <= power; i++){
        result *= num;
    }
    return result;
}

int hashFunction(int round, int initial_buckets, int keyValue){
    int result = keyValue % (power(2, round) * initial_buckets);
    return result;
}

int blankFunction(Item item){
    // some destroy functions require a function for destroying items
    // as parameter, so I use this one as parameter to them to not destroy items
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Functions for destroying  ////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// From header file: typedef void (*destroyItemFun)(Item); 

int destroyBucketsHandler(BucketsHandler** handlerPtrAddr, int bucketentries, destroyItemFun destroyItem){
    int bytesFreed = 0;
    Bucket* bucketPtr = (*handlerPtrAddr)->head;
    while(bucketPtr != NULL){
        for(int i = 0; i < bucketPtr->numOfItems; i++){
            bytesFreed = destroyItem(bucketPtr->items[i]);
        }
        bytesFreed += sizeof(Item) * bucketentries;
        bytesFreed += sizeof(Key) * bucketentries;
        free(bucketPtr->items);
        free(bucketPtr->keys);
        Bucket* temp = bucketPtr;
        bucketPtr = bucketPtr->next;
        bytesFreed += sizeof(Bucket);
        free(temp);
    }
    bytesFreed += sizeof(BucketsHandler);
    free(*handlerPtrAddr);
    *handlerPtrAddr = NULL;
    return bytesFreed;
}

int destroyHashTable(TableHandler** handlerPtrAddr, destroyItemFun destroyItem){
    int bytesFreed = 0;
    BucketsHandler** bucketHandlersArr = (*handlerPtrAddr)->buckets;
    for(int i = 0; i < (*handlerPtrAddr)->prime_buckets; i++){
        bytesFreed += destroyBucketsHandler(&((*handlerPtrAddr)->buckets[i]), (*handlerPtrAddr)->bucketentries, destroyItem);
    }
    bytesFreed += sizeof(BucketsHandler*) * (*handlerPtrAddr)->prime_buckets;
    bytesFreed += sizeof(TableHandler);
    free(bucketHandlersArr);
    free(*handlerPtrAddr);
    *handlerPtrAddr = NULL;
    return bytesFreed;
}

void destroyLastPrimeBucket(TableHandler* handlerPtr, destroyItemFun destroyItem){
    // Decreases the array by one index, destroying last prime bucket in the process
    if(handlerPtr->prime_buckets == 0) return;
    destroyBucketsHandler(&handlerPtr->buckets[handlerPtr->prime_buckets - 1], handlerPtr->bucketentries, destroyItem);
    handlerPtr->buckets = realloc(handlerPtr->buckets, handlerPtr->prime_buckets * sizeof(BucketsHandler*));
    handlerPtr->prime_buckets -= 1;
    handlerPtr->primary_buckets_capacity -= handlerPtr->bucketentries;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Create functions  ////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Bucket* createBucket(int bucketentries){
    Bucket* newBucket = malloc(sizeof(Bucket));
    if(newBucket == NULL){
        return NULL;
    }
    newBucket->numOfItems = 0;
    newBucket->next = NULL;
    newBucket->items = malloc(sizeof(Item) * bucketentries);
    if(newBucket->items == NULL){
        free(newBucket); return NULL;
    }
    newBucket->keys = malloc(sizeof(Key) * bucketentries);
    if(newBucket->keys == NULL){
        free(newBucket->items); free(newBucket); return NULL;
    }
    return newBucket;
}

BucketsHandler* createBucketsHandler(int bucketentries){
    BucketsHandler* newHandlerPtr = malloc(sizeof(BucketsHandler));
    if(newHandlerPtr == NULL) return NULL;
    newHandlerPtr->num_of_buckets = 1;
    newHandlerPtr->numOfItems = 0;
    newHandlerPtr->head = createBucket(bucketentries);
    if(newHandlerPtr->head == NULL){
        free(newHandlerPtr); return NULL;
    }
    newHandlerPtr->tail = newHandlerPtr->head;
    return newHandlerPtr;
}

HashTable createHashTable(int initial_buckets, int bucketentries){
    TableHandler* handlerPtr = malloc(sizeof(TableHandler));
    if(handlerPtr == NULL) return NULL;
    handlerPtr->bucketentries = bucketentries;
    handlerPtr->initial_buckets = initial_buckets;
    handlerPtr->prime_buckets = initial_buckets;
    handlerPtr->round = 0;
    handlerPtr->pointer = 0;
    handlerPtr->numOfItems = 0;
    handlerPtr->primary_buckets_capacity = initial_buckets * bucketentries;
    handlerPtr->load_ratio = 0;
    handlerPtr->buckets = malloc(sizeof(BucketsHandler*) * initial_buckets);
    if(handlerPtr->buckets == NULL){
        free(handlerPtr); return NULL;
    }
    for(int i = 0; i < initial_buckets; i++){
        handlerPtr->buckets[i] = createBucketsHandler(bucketentries);
        if(handlerPtr->buckets[i] == NULL){
            for(int j = 0; j < i; j++){
                destroyBucketsHandler(&(handlerPtr->buckets[j]), bucketentries, blankFunction);
            }
            free(handlerPtr->buckets); free(handlerPtr); return NULL;
        }
    }
    return handlerPtr;
}

int createNewPrimeBucket(TableHandler* handlerPtr){
    // Increases the buckets array by one, creating a new prime bucket for the table
    BucketsHandler** tempArr = realloc(handlerPtr->buckets, (handlerPtr->prime_buckets + 1) * sizeof(BucketsHandler*));

    if(tempArr == NULL) return 0;
    handlerPtr->buckets = tempArr;

    BucketsHandler* tempBucketHandler = createBucketsHandler(handlerPtr->bucketentries);

    if(tempBucketHandler == NULL){
        handlerPtr->buckets = realloc(handlerPtr->buckets, handlerPtr->prime_buckets * sizeof(BucketsHandler*));
        return 0;
    }
    handlerPtr->buckets[handlerPtr->prime_buckets] = tempBucketHandler;

    handlerPtr->prime_buckets += 1;
    handlerPtr->primary_buckets_capacity += handlerPtr->bucketentries;
    return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Read and extract functions  //////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Item readBuckets(BucketsHandler* handlerPtr, Key key){
    Bucket* bucketPtr = handlerPtr->head;
    while(bucketPtr != NULL){
        for(int i = 0; i < bucketPtr->numOfItems; i++){
            if(bucketPtr->keys[i] == key){
                return bucketPtr->items[i];
            }
        }
        bucketPtr = bucketPtr->next;
    }
    return nullItem;
}

Item readHashTable(TableHandler* handlerPtr, Key key, keyToIntFun keyToInt){
    int keyValue = keyToInt(key);
    int index = hashFunction(handlerPtr->round, handlerPtr->initial_buckets, keyValue);

    if(index < handlerPtr->pointer){
        index = hashFunction(handlerPtr->round + 1, handlerPtr->initial_buckets, keyValue);
    }

    return readBuckets(handlerPtr->buckets[index], key);
}

Item extractBuckets(BucketsHandler* handlerPtr, Key key){
    Bucket *bucketPtr = handlerPtr->head, *tail = handlerPtr->tail, *prevPtr = NULL;
    Item item;
    int was_found = 0;

    while(bucketPtr != NULL){
        for(int i = 0; i < bucketPtr->numOfItems; i++){
            if(bucketPtr->keys[i] == key){
                // To fill the gap that is created I get an item from the tail
                item = bucketPtr->items[i];
                bucketPtr->items[i] = tail->items[tail->numOfItems - 1];
                bucketPtr->keys[i] = tail->keys[tail->numOfItems - 1];
                handlerPtr->tail->numOfItems -= 1;
                was_found = 1;
                break;
            }
        }
        if(was_found) break;
        prevPtr = bucketPtr;
        bucketPtr = bucketPtr->next;
    }

    if(!was_found){
        return nullItem;
    }

    if(handlerPtr->tail->numOfItems == 0){
        // If tail got emptied I destroy it
        while(bucketPtr != handlerPtr->tail){
            prevPtr = bucketPtr;
            bucketPtr = bucketPtr->next;
        }

        if(prevPtr != NULL){
            free(bucketPtr->items);
            free(bucketPtr->keys);
            free(bucketPtr);
            prevPtr->next = NULL;
            handlerPtr->tail = prevPtr;
            handlerPtr->num_of_buckets -= 1;
        }
    }

    handlerPtr->numOfItems -= 1;
    return item;
}

Item extractHashTable(TableHandler* handlerPtr, Key key, keyToIntFun keyToInt){
    int keyValue = keyToInt(key);
    int index = hashFunction(handlerPtr->round, handlerPtr->initial_buckets, keyValue);

    if(index < handlerPtr->pointer){
        index = hashFunction(handlerPtr->round + 1, handlerPtr->initial_buckets, keyValue);
    }

    Item item = extractBuckets(handlerPtr->buckets[index], key);

    if(item != nullItem){
        handlerPtr->numOfItems -= 1;
    }

    return item;
}

void readItemsHT(TableHandler* handlerPtr, Item* arr){
    int i = 0;
    for(int j = 0; j < handlerPtr->prime_buckets; j++){
        BucketsHandler* bucketsHandlerPtr = handlerPtr->buckets[j];
        Bucket* bucketPtr = bucketsHandlerPtr->head;
        while(bucketPtr != NULL){
            for(int k = 0; k < bucketPtr->numOfItems; k++){
                arr[i] = bucketPtr->items[k];
                i += 1;
            }
            bucketPtr = bucketPtr->next;
        }
    }
}

// Needed in case the user does not have memory space to read items
// through an array, but he wants to store the state of the voters
// before he exits the program, without having to allocate memory
void storeInFile(TableHandler* handlerPtr, FILE* fileptr, storeItemInFile storeItem){
    for(int j = 0; j < handlerPtr->prime_buckets; j++){
        BucketsHandler* bucketsHandlerPtr = handlerPtr->buckets[j];
        Bucket* bucketPtr = bucketsHandlerPtr->head;
        while(bucketPtr != NULL){
            for(int k = 0; k < bucketPtr->numOfItems; k++){
                storeItem(bucketPtr->items[k], fileptr);
            }
            bucketPtr = bucketPtr->next;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Insertion functions  /////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int insertBuckets(BucketsHandler* handlerPtr, Item item, Key key, int bucketentries);
int splitHashTable(TableHandler* handlerPtr, keyToIntFun keyToInt);

int insertHashTable(TableHandler* handlerPtr, Item item, Key key, keyToIntFun keyToInt){
    int bucketentries = handlerPtr->bucketentries;
    int keyValue = keyToInt(key);
    int index = hashFunction(handlerPtr->round, handlerPtr->initial_buckets, keyValue);
    
    if(index < handlerPtr->pointer){
        index = hashFunction(handlerPtr->round + 1, handlerPtr->initial_buckets, keyValue);
    }

    int was_inserted = insertBuckets(handlerPtr->buckets[index], item, key, bucketentries);
    if(!was_inserted){ // If true aborts mission
        return 0;
    }

    handlerPtr->load_ratio = handlerPtr->numOfItems / (float) handlerPtr->primary_buckets_capacity;

    if(handlerPtr->load_ratio > 0.75){
        int split_success = splitHashTable(handlerPtr, keyToInt);

        if(split_success == 0){ // If true aborts mission
            extractBuckets(handlerPtr->buckets[index], key); 
            return 0;
        }

        if(handlerPtr->prime_buckets == power(2, handlerPtr->round + 1) * handlerPtr->initial_buckets){
            handlerPtr->round += 1;
            handlerPtr->pointer = 0;
        } else{
            handlerPtr->pointer += 1;
        }
    }

    handlerPtr->numOfItems += 1;
    return 1; // Signals that everything worked well
}


int insertBuckets(BucketsHandler* handlerPtr, Item item, Key key, int bucketentries){
    if(handlerPtr->tail->numOfItems == bucketentries){
        Bucket* newBucket = createBucket(bucketentries);
        handlerPtr->tail->next = newBucket;
        if(newBucket == NULL) return 0;        
        newBucket->numOfItems = 1;
        newBucket->items[0] = item;
        newBucket->keys[0] = key;
        handlerPtr->tail = newBucket;
        handlerPtr->num_of_buckets += 1;
    } else{
        handlerPtr->tail->items[handlerPtr->tail->numOfItems] = item;
        handlerPtr->tail->keys[handlerPtr->tail->numOfItems] = key;
        handlerPtr->tail->numOfItems += 1;
    }
    handlerPtr->numOfItems += 1;
    return 1; // Signals that everything worked well
}

int rehashBucketItems(TableHandler*, Bucket*, changesLogs*, keyToIntFun);
void freeEmptyOvfBuckets(BucketsHandler*);
void reverseSplit(BucketsHandler*, changesLogs*, int, int);

int splitHashTable(TableHandler* handlerPtr, keyToIntFun keyToInt){
    if(createNewPrimeBucket(handlerPtr) == 0) return 0;
    BucketsHandler* bucketsHandlerPtr = handlerPtr->buckets[handlerPtr->pointer];

    changesLogs* chLogs = createChangesLogs();
    if(chLogs == NULL){
        destroyLastPrimeBucket(handlerPtr, blankFunction);
        return 0;
    }

    Bucket* bucketPtr = bucketsHandlerPtr->head;
    int was_hashed = 1, ogNumOfItems = 0;

    bucketsHandlerPtr->tail = bucketPtr;
    while(bucketPtr != NULL){
        ogNumOfItems = bucketPtr->numOfItems; // will bee needed in case I need to undo split
        was_hashed = rehashBucketItems(handlerPtr, bucketPtr, chLogs, keyToInt);
        if(was_hashed == 0) break;
        bucketPtr = bucketPtr->next;
    }

    if(was_hashed){
        freeEmptyOvfBuckets(bucketsHandlerPtr);
        destroyChangeLogs(chLogs);
    } else{
        reverseSplit(bucketsHandlerPtr, chLogs, ogNumOfItems, handlerPtr->bucketentries);
        destroyChangeLogs(chLogs);
        destroyLastPrimeBucket(handlerPtr, blankFunction);
        return 0;
    }

    return 1; // Signals that everything worked well
}

int rehashBucketItems(TableHandler* handlerPtr, Bucket* bucketPtr, changesLogs* chLogs, keyToIntFun keyToInt){
    // Responsible for rehashing all items in one bucket, the chLogs is used to record the changes that get made and for 
    // cases where items get rehashed to same index I update the tail of the handler and numOfItems of bucket appropriately
    BucketsHandler* bucketsHandlerPtr = handlerPtr->buckets[handlerPtr->pointer];

    int flag, bucketentries = handlerPtr->bucketentries, n = bucketPtr->numOfItems;
    bucketPtr->numOfItems = 0;

    for(int i = 0; i < n; i++){
        Item item = bucketPtr->items[i];
        Key key = bucketPtr->keys[i];
        int keyValue = keyToInt(key);
        int index = hashFunction(handlerPtr->round + 1, handlerPtr->initial_buckets, keyValue);

        flag = insertChangesLogs(chLogs, key, bucketsHandlerPtr, handlerPtr->buckets[index]);
        if(flag == 0) return 0;

        int flag = insertBuckets(handlerPtr->buckets[index], item, key, bucketentries);
        
        if(flag == 0){
            changesNode* chNode = extractLast(chLogs);
            free(chNode);
            return 0;
        }

        if(bucketsHandlerPtr->tail->numOfItems == bucketentries){       
            if(bucketsHandlerPtr->tail->next != NULL){
                bucketsHandlerPtr->tail = bucketsHandlerPtr->tail->next;
            }
        }

        bucketsHandlerPtr->numOfItems -= 1;
    }

    return 1;
}

void freeEmptyOvfBuckets(BucketsHandler* bucketsHandlerPtr){
    // The split has been completed successfuly, now I can free any overflow buckets I don't need

    Bucket* bucketPtr = bucketsHandlerPtr->tail->next;
    bucketsHandlerPtr->tail->next = NULL;
    Bucket* tempPtr;
    while(bucketPtr != NULL){
        bucketsHandlerPtr->num_of_buckets -= 1;
        free(bucketPtr->items);
        free(bucketPtr->keys);
        tempPtr = bucketPtr->next;
        free(bucketPtr);

        bucketPtr = tempPtr;
    }

}

void reverseSplit(BucketsHandler* bucketsHandlerPtr, changesLogs* chLogs, int ogNumOfItems, int bucketentries){
    // The split has failed, using the changesLogs list I can backtrack my steps to undo any changes
    // that had already been completed, items are brought back to their initial index

    changesNode* chNodePtr = extractLast(chLogs);
    while(chNodePtr != NULL){
        if(chNodePtr->from != chNodePtr->to){
            Item item = extractBuckets(chNodePtr->to, chNodePtr->key);
            // Note that I intentionally didn't free any of the buckets before the split is completed so now I
            // can insert items back to their index and I am sure they are going to fit with no need of mallocs
            insertBuckets(bucketsHandlerPtr, item, chNodePtr->key, bucketentries);
            if(bucketsHandlerPtr->tail->numOfItems == bucketentries){
                if(bucketsHandlerPtr->tail->next != NULL){
                    bucketsHandlerPtr->tail = bucketsHandlerPtr->tail->next;
                }
            }
        }
        chNodePtr = extractLast(chLogs);
    }
    bucketsHandlerPtr->tail->numOfItems = ogNumOfItems;
    while(bucketsHandlerPtr->tail->next != NULL){
        bucketsHandlerPtr->tail = bucketsHandlerPtr->tail->next;
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Othe functions  //////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int numOfItemsHT(TableHandler* handlerPtr){
    return handlerPtr->numOfItems;
}

// To be continued ... ?