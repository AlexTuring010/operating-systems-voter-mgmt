#include "./header_files/command.h"

int command_save(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // save <filepath> - Saves the current state of the program in <filepath>, file can then be used with -f flag to resume another time

    char* filePath = strtok(NULL, delimiters);

    FILE* temp_file = fopen(filePath, "w");

    if(temp_file == NULL){
        printf(ERROR_COLOR);
        printf("%s could not be opened\n", filePath);
        printf(RESET_COLOR);
        return 0;
    }

    storeInFile(myHashTable, temp_file, storeVoterInFile);

    printf("The state of the participants has been saved in %s\n", filePath);
    fclose(temp_file);
    return 1;
}