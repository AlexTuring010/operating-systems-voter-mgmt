#include "./header_files/command.h"

int dontDestroyItem(Item item){
    return 0;
}

int command_exit(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // exit - Exits the program, returns how many bytes were freed in the process\n

    int bytesFreed = 0;
    bytesFreed += destroyHashTable(&myHashTable, destroy_voter);
    bytesFreed += destroyLoL(&myLoL, dontDestroyItem);
    printf("%d of Bytes Released\n\n", bytesFreed);
    return -1;
}