#include "./header_files/command.h"

int command_o(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // o - Returns in order how many voters have voted of each zip code, order is from most voters to least voters

    if(numOfKeys(myLoL) == 0){
        printf(WARNING_COLOR);
        printf("No one has voted yet\n");
        printf(RESET_COLOR);
        return 0;
    }

    List* lists = malloc(sizeof(List) * numOfKeys(myLoL));
    if(lists == NULL){
        printf(ERROR_COLOR);
        printf("Not enough memory for running this command\n");
        printf(RESET_COLOR);
    }

    readLists(myLoL, lists, numOfKeys(myLoL));
        
    for(int i = 0; i < numOfKeys(myLoL); i++){
        printf("%d  %d\n", getKey(lists[i]), numOfItems(lists[i]));
    }
    
    free(lists);
}
