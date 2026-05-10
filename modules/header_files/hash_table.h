// A module of a hash table using linear probing

// I've made it so that the module handles cases where malloc fails, it will
// reverse last actions, prevent leaks and let the user know it failed, so that
// the user can then decide how to move forward as he pleases.

#ifndef HASHTABLE_H                                                  /* Header guard to prevent the header file from being included in   */
#define HASHTABLE_H                                                  /* multiple places and possible causing an infinte loop of includes */
#include <stdio.h>
#include "voter.h"                                                   // voter.h includes the Voter data typ

typedef Voter Item;                                                  // Using the Voter as the items of the hash table
typedef int Key;                                                     // PIN is the key and is an intger for our program

#define nullItem NULL                                                // Set a value to be returned if no item is found when searching

typedef struct hash_table* HashTable;                                // This hash table uses linear hashing, has average search time complexity of O(1)

HashTable createHashTable(int initial_buckets, int bucketentries);   // Creates and initializes the hash table, use before anything else

typedef int (*keyToIntFun)(Key);                                     // Inserts an item into the has table based on a key. Psas a function for turning your
int insertHashTable(HashTable, Item, Key, keyToIntFun);              // key into an int value. If your key is already int just pass a function that returns the key.

Item readHashTable(HashTable, Key, keyToIntFun);                     // Reads an item from the hash table, will return nullItem if no item of that key was found

Item extractHashTable(HashTable, Key, keyToIntFun);                  // Extracts an item from the hash table

int numOfItemsHT(HashTable);                                         // Returns number of items in hash table

void readItemsHT(HashTable, Item* arr);                              // Adds all items in your array, WARNING: make sure your array has allocated enough space
   
typedef void (*storeItemInFile)(Item, FILE*);                        // Write items in a file, the format should be specified by a function passed as parameter
void storeInFile(HashTable, FILE*, storeItemInFile);                 // Unlike readImtesHT, this function does not need you to allocate memory for an array

typedef int (*destroyItemFun)(Item);                                 // Destroys the hash table, pass function for destroying your item.
int destroyHashTable(HashTable*, destroyItemFun);                    // If you dont want the item to be destroyed, just pass a function that does nothing

#endif

// This module was made by Alex Gkiafis, sdi2200284