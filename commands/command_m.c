#include "./header_files/command.h"

int command_m(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // m <pin> - Updates the state of the participant of specified PIN to has voted

    char* parameter = strtok(NULL, delimiters);
    if(testNumberParemeter(parameter) == 0){
        printf(ERROR_COLOR);
        printf("Malformed Input\n");
        printf(RESET_COLOR);
        return 0;
    }
    int pin = atoi(parameter);

    Voter voter = readHashTable(myHashTable, pin, PINtoInt);
    if(voter == nullItem){
        printf(ERROR_COLOR);
        printf("%d does not exist\n", pin);
        printf(RESET_COLOR);
    } else if(hasVoted(voter) == 'Y'){
        printf(WARNING_COLOR);
        printf("%d was already marked voted\n", pin);
        printf(RESET_COLOR);
    } else{
        setToHasVoted(voter);
        insertItem(myLoL, voter, getTK(voter));
    }

    printf("%d Marked Voted\n", pin);
}