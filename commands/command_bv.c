#include "./header_files/command.h"

int command_bv(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // bv <fileofkeys> - Updates the state of the participants of specified PINs written in <filepath> to has voted
    char line[500];
    int line_size = 500;

    char* fileName = strtok(NULL, delimiters);
    FILE* filePtr = fopen(fileName, "r");
    
    if(filePtr == NULL){
        printf(ERROR_COLOR);
        printf("%s could not be opened\n", fileName);
        printf(RESET_COLOR);
        return 0;
    }

    int lines_read = 0;
    while(fgets(line, line_size, filePtr)) {
        char* pinString = strtok(line, delimiters);
        
        if(pinString == NULL){
            continue;
        }

        lines_read++;

        if(testNumberParemeter(pinString) == 0){
            printf(ERROR_COLOR);
            printf("Malformed Input\n");
            printf(RESET_COLOR);
            continue;
        } 
        int pin = atoi(pinString);

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
            printf("%d Marked Voted\n", pin);
            setToHasVoted(voter);
            insertItem(myLoL, voter, getTK(voter));
        }
    }

    fclose(filePtr);

    if(lines_read == 0){
        printf(WARNING_COLOR);
        printf("The file specified was empty\n");
        printf(RESET_COLOR);
    }
}