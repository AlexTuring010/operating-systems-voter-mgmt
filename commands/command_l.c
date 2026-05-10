#include "./header_files/command.h"

int command_l(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // l <pin> - Prints data of participant with specified PIN

    char* parameter = strtok(NULL, delimiters);
    if(testNumberParemeter(parameter) == 0){
        printf(ERROR_COLOR);
        printf("Malformed Pin\n");
        printf(RESET_COLOR);
        return 0;
    }
    
    int pin = atoi(parameter);
    Voter voter = readHashTable(myHashTable, pin, PINtoInt);

    if(voter == nullItem){
        printf(ERROR_COLOR);
        printf("Participant %d not in cohort\n", pin);
        printf(RESET_COLOR);
    } else{
        printf("%d %s %s %d\n", getPin(voter), getSurName(voter), getFirstName(voter), getTK(voter));
    }

}
