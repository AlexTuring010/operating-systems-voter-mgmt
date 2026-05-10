#include "./header_files/commandHandler.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// For initializing the commands handler ////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// If I ever add more commands I should modify the code here so that they are added to the array
// If the amount of commands increases significally I should consider using a hash table instead of array

int command_l(HashTable, ListOfLists, char* delimiter);
int command_i(HashTable, ListOfLists, char* delimiter);
int command_m(HashTable, ListOfLists, char* delimiter);
int command_bv(HashTable, ListOfLists, char* delimiter);
int command_v(HashTable, ListOfLists, char* delimiter);
int command_perc(HashTable, ListOfLists, char* delimiter);
int command_z(HashTable, ListOfLists, char* delimiter);
int command_o(HashTable, ListOfLists, char* delimiter);
int command_save(HashTable, ListOfLists, char* delimiter);
int command_exit(HashTable, ListOfLists, char* delimiter);
int command_clear(HashTable, ListOfLists, char* delimiter);
int command_help(HashTable, ListOfLists, char* delimiter);
int command_if(HashTable, ListOfLists, char* delimite);

typedef int (*commandFunction)(HashTable, ListOfLists, char* delimiter);

struct Command {
    const char *name;
    commandFunction cmdFunction;
};

#define NUM_OF_COMMANDS 13

struct Command commands[NUM_OF_COMMANDS] = {
    {"l", command_l},
    {"i", command_i},
    {"m", command_m},
    {"bv", command_bv},
    {"v", command_v},
    {"perc", command_perc},
    {"z", command_z},
    {"o", command_o},
    {"save", command_save},
    {"exit", command_exit},
    {"clear", command_clear},
    {"help", command_help},
    {"if", command_if}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Code for handling commands ///////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// It takes a line from the terminal and finds what function it should run

int executeCommand(char* line, HashTable myHashTable, ListOfLists myLoL){
    char* delimiters = " \t\n\r";
    char* command = strtok(line, delimiters);
        
    if(command == NULL) return 0;

    int flag, command_found = 0;

    for(int i = 0; i < NUM_OF_COMMANDS; i++){
        if(strcmp(commands[i].name, command) == 0){
            command_found = 1;
            flag = commands[i].cmdFunction(myHashTable, myLoL, delimiters);
        }
    }

    if(!command_found){
        printf(ERROR_COLOR);
        printf("Command not found\n");
        printf(RESET_COLOR);
        printf("\n");
        return 0;
    } else{
        printf("\n");
        return flag;
    }
}