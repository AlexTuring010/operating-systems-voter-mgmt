#include "./header_files/command.h"

int command_perc(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // perc - Returns percentage of participants that have voted so far
    
    float ratio = LoLnumOfItems(myLoL) / (float) numOfItemsHT(myHashTable);
    printf("%f\n", ratio);
    return 1;
}