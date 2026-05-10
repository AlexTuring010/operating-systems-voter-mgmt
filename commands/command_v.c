#include "./header_files/command.h"

int command_v(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // v - Returns number of participants that have voted so far

    printf("Voted So Far %d\n", LoLnumOfItems(myLoL));
    return 1;
}