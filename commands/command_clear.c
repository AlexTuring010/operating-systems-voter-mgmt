#include "./header_files/command.h"

int command_clear(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // clear -  Clears the terminal

    system("clear");
    return 1;
}