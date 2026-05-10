// This file was written by Alex Gkiafis, sdi2200284

#include <stdio.h>
#include "./header_files/ListOfLists.h"
#include <stdlib.h>

// I suck at naming but thats the best I could do. ListOfLists can be referred as LoL for shorter
// I decided to use list handlers so that I can keep additional information about each list,
// they going to take a little bit of extra space but I can keep info on the number of items,
// number of keys and an extra pointer to the tail of the list for fast insertion, I think its worth it.

typedef struct List_node{
    Item item;
    struct List_node* next;
} List_node;

// From header file: typedef struct list* List;
typedef struct list{
    int num_of_items;
    Key key;
    List_node* head;
    List_node* tail;
} List_Handler;

typedef struct LoL_node{
    List_Handler* list;
    Key key;
    struct LoL_node* next;
    struct LoL_node* prev;
} LoL_node;

// From header file: typedef struct listOfLists* ListOfLists;
typedef struct listOfLists{
    int num_of_items;
    int num_of_keys;
    LoL_node* head;
    LoL_node* tail;
} LoL_Handler;

// Now the fun part. The implementation of the functions:

// only createLoL is accessible to the user
ListOfLists createLoL(){
    LoL_Handler* newLoL = malloc(sizeof(LoL_Handler));
    if(newLoL == NULL) return NULL;
    newLoL->num_of_items = 0;
    newLoL->num_of_keys = 0;
    newLoL->head = NULL;
    newLoL->tail = NULL;
    return newLoL;
}

List createList(Key key){
    List newList = malloc(sizeof(List_Handler));
    if(newList == NULL) return NULL;
    newList->num_of_items = 0;
    newList->key = key;
    newList->head = NULL;
    newList->tail = NULL;
    return newList;
}

int insertInLoL(ListOfLists, Item, Key);
int insertInList(List, Item, Key);

// only insertItem is accessible to the user
int insertItem(LoL_Handler* handlerPtr, Item item, Key key){
    return insertInLoL(handlerPtr, item, key);
}

int insertInLoL(LoL_Handler* handlerPtr, Item item, Key key){
    LoL_node* lolNodePtr;
    LoL_node** ptrAddrs = &(handlerPtr->head);

    while((*ptrAddrs) != NULL && (*ptrAddrs)->key != key){
        ptrAddrs = &((*ptrAddrs)->next);
    }

    if((*ptrAddrs) == NULL){
        (*ptrAddrs) = malloc(sizeof(LoL_node));
        if(*ptrAddrs == NULL) return 0;
        lolNodePtr = *ptrAddrs;
        lolNodePtr->key = key;
        lolNodePtr->next = NULL;
        lolNodePtr->prev = handlerPtr->tail;
        handlerPtr->tail = lolNodePtr;
        lolNodePtr->list = createList(key);
        if(lolNodePtr->list == NULL){
            handlerPtr->tail = lolNodePtr->prev;
            free(lolNodePtr);
            *ptrAddrs = NULL;
            return 0;
        }
        handlerPtr->num_of_keys += 1;
    } else{
        lolNodePtr = *ptrAddrs;
    }

    int was_inserted = insertInList(lolNodePtr->list, item, key);

    if(was_inserted){
        handlerPtr->num_of_items += 1;
    } else{
        if(lolNodePtr->list->num_of_items == 0){
            handlerPtr->tail = lolNodePtr->prev;
            free(lolNodePtr->list);
            free(lolNodePtr);
            *ptrAddrs = NULL;
        }
        return 0;
    }

    // The insertion is finished, bellow I just do any necessary sorting
    LoL_node* rightNodePtr = lolNodePtr;
    LoL_node* leftNodePtr = lolNodePtr->prev;
    while(leftNodePtr != NULL && rightNodePtr->list->num_of_items > leftNodePtr->list->num_of_items){
        List_Handler* tempList = rightNodePtr->list;
        Key tempKey = rightNodePtr->key;
        rightNodePtr->list = leftNodePtr->list;
        rightNodePtr->key = leftNodePtr->key;
        leftNodePtr->list = tempList;
        leftNodePtr->key = tempKey;
        rightNodePtr = leftNodePtr;
        leftNodePtr = leftNodePtr->prev;
    }

    return 1; // signal that everything worked well
}

int insertInList(List_Handler* handlerPtr, Item item, Key key){
    List_node* listNodePtr;
    if(handlerPtr->num_of_items == 0){
        handlerPtr->head = malloc(sizeof(List_node));
        if(handlerPtr->head == NULL) return 0;
        handlerPtr->head->item = item;
        handlerPtr->head->next = NULL;
        handlerPtr->tail = handlerPtr->head;
    } else{
        handlerPtr->tail->next = malloc(sizeof(List_node));
        if(handlerPtr->tail->next == NULL) return 0;
        handlerPtr->tail->next->item = item;
        handlerPtr->tail->next->next = NULL;
        handlerPtr->tail = handlerPtr->tail->next;
    }
    handlerPtr->num_of_items += 1;

    return 1; // signal that everything worked well
}

int numOfKeys(LoL_Handler* handlerPtr){
    return handlerPtr->num_of_keys;
}

int numOfItems(List_Handler* handlerPtr){
    return handlerPtr->num_of_items;
}

int LoLnumOfItems(LoL_Handler* handlerPtr){
    return handlerPtr->num_of_items;
}

Key getKey(List_Handler* handlerPtr){
    return handlerPtr->key;
}

List readList(LoL_Handler* handlerPtr, Key key){
    LoL_node* lolNodePtr = handlerPtr->head;
    while(lolNodePtr != NULL){
        if(lolNodePtr->key == key){
            return lolNodePtr->list;
        }
        lolNodePtr = lolNodePtr->next;
    }
    return NULL;
}

List extractList(LoL_Handler* handlerPtr, Key key){
    LoL_node* lolNodePtr = handlerPtr->head;
    LoL_node* prevNodePtr = NULL;
    while(lolNodePtr != NULL){
        if(lolNodePtr->key == key){
            if(prevNodePtr == NULL){
                handlerPtr->head = lolNodePtr->next;
            } else{
                prevNodePtr->next = lolNodePtr->next;
            }

            if(lolNodePtr->next == NULL){
                handlerPtr->tail = prevNodePtr;
            } else{
                lolNodePtr->next->prev = prevNodePtr;
            }

            List_Handler* listHandlerPtr = lolNodePtr->list;
            free(lolNodePtr);
            handlerPtr->num_of_keys -= 1;
            handlerPtr->num_of_items -= listHandlerPtr->num_of_items;
            return listHandlerPtr;
        }
        prevNodePtr = lolNodePtr;
        lolNodePtr = lolNodePtr->next;
    }
    return NULL;
}

int readLists(LoL_Handler* handlerPtr, List* arr, int n){
    LoL_node* lolNodePtr = handlerPtr->head;
    int i = 0;
    while(lolNodePtr != NULL && i < n){
        arr[i++] = lolNodePtr->list;
        lolNodePtr = lolNodePtr->next;
    }
    return i;
}

int readItems(List_Handler* handlerPtr, Item* arr, int n){
    List_node* listNodePtr = handlerPtr->head;
    int i = 0;
    while(listNodePtr != NULL && i < n){
        arr[i++] = listNodePtr->item;
        listNodePtr = listNodePtr->next;
    }
    return i;
}

int destroyList(List_Handler** handlePtrAddr, destroyItemFunction destroyItem){
    int bytesFreed = 0;
    List_node* listNodePtr = (*handlePtrAddr)->head;
    List_node* temp;
    while(listNodePtr != NULL){
        bytesFreed += destroyItem(listNodePtr->item);
        temp = listNodePtr;
        listNodePtr = listNodePtr->next;
        bytesFreed += sizeof(List_node);
        free(temp);        
    }
    bytesFreed += sizeof(List_Handler);
    free(*handlePtrAddr);
    *handlePtrAddr = NULL;
    return bytesFreed;
}

int destroyLoL(LoL_Handler** handlePtrAddr, destroyItemFunction destroyItem){
    int bytesFreed = 0;
    LoL_node* lolNodePtr = (*handlePtrAddr)->head;
    LoL_node* temp;
    while(lolNodePtr != NULL){
        bytesFreed += destroyList(&(lolNodePtr->list), destroyItem);
        temp = lolNodePtr;
        lolNodePtr = lolNodePtr->next;
        bytesFreed += sizeof(LoL_node);
        free(temp);
    }
    bytesFreed += sizeof(LoL_Handler);
    free(*handlePtrAddr);
    *handlePtrAddr = NULL;
    return bytesFreed;
} 
