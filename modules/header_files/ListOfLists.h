// ListOfLists is a module for storing data based on a key.

// I've made it so that the module handles cases where malloc fails, it will
// reverse last actions, prevent leaks and let the user know it failed, so that
// the user can then decide how to move forward as he pleases.

// Here is a visual example of this data structure. Every time an item is added
// with a key that has not been recorded a new element is created in the list.
// Each element contains another list of all the items that share the same key.
//
//       [Alex]
//         |
//       [John]      [Peter]      [Dennis]
//         |            |            |
//   [123][ ][]-> [421][ ][]-> [001][ ][] -> Null
//
// So in this example, Alex and John share the same key, which is 123. The user can easily define what
// he wants the types of the key and item to be. So this module could easily be reused in later projects.
// Also, the keys are sorted so that keys with more items are first. This way you can have fast access
// to which key holds the most items, for example, which areas have the most voters?

#ifndef LISTOFLISTS_H                                /* This is a header guard to prevent the file from being included  */
#define LISTOFLISTS_H                                /* more than once which could cause an infinite chain of includes  */

#include "./voter.h"                                 // includes voter.h so I got access to Voter data type and it's functions

// Define what Key and Item are:
typedef int Key;                                     // As of my knowledge postal codes are integers here in Greece
typedef Voter Item;                                  // And for my Item we going to use the cool Voter type I just made

typedef struct listOfLists* ListOfLists;             // ListOfLists is the main data structure you will be usinng
typedef struct list* List;                           // List is another data structure, it lives inside ListOfLists

// I will refer to ListOfLists as LoL for shorter

// Here are some nice functions for using this data structure:

ListOfLists createLoL();                             // Important for initializing the data structure before using

int insertItem(ListOfLists, Item, Key);              // Returns 0 if insertion fails, returns 1 if insertion succeeds

int numOfKeys(ListOfLists);                          // Gets the number of different keys in the LoL

int numOfItems(List);                                // Gets the number of items in a list

int LoLnumOfItems(ListOfLists);                      // Gets the number of items in the LoL

Key getKey(List);                                    // Gets the key of a list

List readList(ListOfLists, Key);                     // It reads the list of specified key, returns NULL if no key found

List extractList(ListOfLists, Key);                  // It extracts the list of specified key, returns NULL if no key found

int readLists(ListOfLists, List* arr, int n);        // Adds first n lists to an array, returns how many lists were added

int readItems(List, Item* arr, int n);               // Adds first n items to an array, returns how many items were added

// Remember, lists are sorted based on how many items they got, list with the most items will be at the start of the array
// Items on the other hand, they are sorted based on when they were inserted, item inserted first will be at the start of the array

// Warning: only destroy lists that have been extracted from the LoL. Lists that have only been read using
// the readList or readLists are still linked inside the LoL, they are not extracted so do not destroy them.
// Same goees for items that have been read using readItems function, do not free those items, they are still in their list.

typedef int (*destroyItemFunction)(Item);           // The user should pass a function as a parameter to use for destroying his items

int destroyList(List*, destroyItemFunction);         // Always use those two functions whenever you are done using a list or a LoL
int destroyLoL(ListOfLists*, destroyItemFunction);   // They will destroy the data structures, and free all memory in the process

// You may have noticed there is no function for removing an item from the LoL, that's because for the current project 
// someone who has voted cannot be changed so that he has not voted. But a delete function could be made in the future.

#endif

// This module was made by Alex Gkiafis, sdi2200284