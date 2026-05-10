// This file was written by Alex Gkiafis, sdi2200284

// This program can be used to store the state of participants in an election
// It uses a hash table, so you can have fast access to any participant you want
// using his PIN. You also got fast access to info on who have voted of a certain
// postal codes, for which I am using a list of lists data structure. While making
// this tool, I have taken steps to protect the data in case of malloc fail, so
// if you are using this program, but suddenly run out of memory, the state
// of the participants will not be lost, you can save it in a file and exit
// the program, or you can wait till you have more memory available.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./header_files/commandHandler.h"
#include "./header_files/colors.h"               // Defines how text should be colored
#include "./modules/header_files/voter.h"        // Defines the Voter data structure
#include "./modules/header_files/ListOfLists.h"  // Defines the ListOfList data structure
#include "./modules/header_files/hash_table.h"   // Defines the hashtable data structure

char* readCommand(char* line, int size);         // Reads a command from the user

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// The main function ////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]){
    char line[500];
    int line_size = 499;
    FILE* temp_file;

    // Here I initialize with default value in case arguments were not passed
    int bucketentries = 2, initial_size = 2;
    char *registeredvoters = NULL;

    for (int i = 0; i < argc; i++) {
        if(strlen(argv[i]) == 2 && argv[i][0] == '-'){
            if(i+1 == argc) break;
            switch(argv[i][1]){
                case 'f':
                    registeredvoters = argv[i+1];
                    break;
                case 'b': 
                    bucketentries = atoi(argv[i+1]);
                    break;
                case 'i':
                    initial_size = atoi(argv[i+1]);
                    break;
            }
        }
    }

    if(bucketentries <= 0 || initial_size <= 0){
        printf(ERROR_COLOR);
        printf("Flags -b and -i can only use values greater than 0\n");
        return 1;
    }

    HashTable myHashTable = createHashTable(initial_size, bucketentries);
    if(myHashTable == NULL){
        printf(ERROR_COLOR);
        printf("Not enough memory for initializing the program\n");
        return 1;
    }

    ListOfLists myLoL = createLoL();
    if(myLoL == NULL){
        printf(ERROR_COLOR);
        printf("Not enough memory for initializing the program\n");
        return 1;
    }

    // If file for initializing the hashtable was passed as argument
    if(registeredvoters != NULL){
        strcpy(line, "if "); // Just runs an if <filename> command
        strcat(line, registeredvoters);
        executeCommand(line, myHashTable, myLoL);
    }

    // Starts listening to user commands
    while(readCommand(line, line_size)){
        int flag = executeCommand(line, myHashTable, myLoL);
        if(flag == -1) break; // -1 is special flag for stopping the program
    }

    return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Other functions //////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char* readCommand(char* line, int size){
    printf(PROMPT_COLOR);
    printf("Enter your command");
    printf(RESET_COLOR);
    printf(": ");
    return fgets(line, size, stdin);
}