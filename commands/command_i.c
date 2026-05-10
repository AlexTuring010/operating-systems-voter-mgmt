#include "./header_files/command.h"

int command_i(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // i <pin> <lname> <fname> <zip> - Inserts a new participant into the program

    char* pinString = strtok(NULL, delimiters);
    if(testNumberParemeter(pinString) == 0){
        printf(ERROR_COLOR); 
        printf("Malformed Input\n");
        printf(RESET_COLOR);
        return 0;
    }

    char* surname = strtok(NULL, delimiters);
    char* firstname = strtok(NULL, delimiters);
        
    if(surname == NULL || firstname == NULL){
        printf(ERROR_COLOR); 
        printf("Malformed Input\n"); 
        printf(RESET_COLOR);
        return 0;
    }

    char* tkString =  strtok(NULL, delimiters);
    if(testNumberParemeter(tkString) == 0){
        printf(ERROR_COLOR); 
        printf("Malformed Input\n"); 
        printf(RESET_COLOR);
        return 0;
    }

    int pin = atoi(pinString);
    int tk = atoi(tkString);

    Voter voter = readHashTable(myHashTable, pin, PINtoInt);
    if(voter != nullItem){
        printf(ERROR_COLOR); 
        printf("%d already exist\n", pin); 
        printf(RESET_COLOR);
        return 0;
    }
    
    voter = create_voter(pin, surname, firstname, tk);
    if(voter == NULL){
        printf(ERROR_COLOR); 
        printf("Not enough memory for running this command\n"); 
        printf(RESET_COLOR);
        return 0;
    }

    int was_inserted = insertHashTable(myHashTable, voter, pin, PINtoInt);
    if(!was_inserted){
        printf(ERROR_COLOR);
        printf("Not enough memory for running this command\n"); 
        printf(RESET_COLOR);
        return 0;
    }

    printf("Inserted %d %s %s %d N\n", pin, surname, firstname, tk);
}