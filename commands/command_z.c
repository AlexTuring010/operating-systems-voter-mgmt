#include "./header_files/command.h"

int command_z(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // z <zipcode> - Returns the PIN of participants who have voted of specified zip code
            
    char* tkString = strtok(NULL, delimiters);
    if(testNumberParemeter(tkString) == 0){
        printf(ERROR_COLOR); 
        printf("Malformed Input\n");
        printf(RESET_COLOR);
        return 0;
    }
    int tk = atoi(tkString);

    List myList = readList(myLoL, tk);
    if(myList == NULL){
        printf(WARNING_COLOR); 
        printf("There are no voters in given zipcode\n");
        printf(RESET_COLOR);
        return 0;
        return 0;
    }

    Voter* voters = malloc(sizeof(Voter) * numOfItems(myList));
    if(voters == NULL){
        printf(ERROR_COLOR);
        printf("Not enough memory for running this command\n");
        printf(RESET_COLOR);
        return 0;
    }

    readItems(myList, voters, numOfItems(myList));

    printf("%d voted in %d\n", numOfItems(myList), tk);
    for(int i = 0; i < numOfItems(myList); i++){
        printf("%d \n", getPin(voters[i]));
    }

    free(voters);
}