#include "./header_files/command.h"

int command_if(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // if <filepath> - Inserts participants of <file> into the program
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

    char surName[256], firstName[256], hasvoted;
    int pin, postalCode, lines_read = 0;

    while(fgets(line, line_size, filePtr)) {
        hasvoted = 'N';
        int numOfParameters = sscanf(line, "%d %255s %255s %d %c", &pin, surName, firstName, &postalCode, &hasvoted);
        lines_read += 1;

        if(numOfParameters < 4){
            printf(ERROR_COLOR);
            printf("Line %d missing required parameters\n", lines_read);
            printf(RESET_COLOR);
            continue;
        }

        Voter voter = readHashTable(myHashTable, pin, PINtoInt);
        if(voter != NULL){
            printf(ERROR_COLOR);
            printf("PIN %d has already been used for another participant\n", pin);
            printf(RESET_COLOR);
            continue;
        }

        voter = create_voter(pin, surName, firstName, postalCode);
        if(voter == NULL){
            printf(ERROR_COLOR);
            printf("Not enough memory to store participant with PIN %d\n", pin);
            printf(RESET_COLOR);
            continue;
        }

        int was_inserted = insertHashTable(myHashTable, voter, pin, PINtoInt);
        if(was_inserted == 0){
            printf(ERROR_COLOR);
            printf("Not enough memory to store participant with PIN %d\n", pin);
            printf(RESET_COLOR);
            destroy_voter(voter);
            continue;
        }

        if(numOfParameters == 5 && hasvoted == 'Y'){
            setToHasVoted(voter);
            was_inserted = insertItem(myLoL, voter, getTK(voter));
            if(was_inserted == 0){
                printf(ERROR_COLOR);
                printf("Not enough memory to store participant with PIN %d\n", pin);
                printf(RESET_COLOR);
                extractHashTable(myHashTable, pin, PINtoInt);
                destroy_voter(voter);
                continue;
            }
        }
    }

    fclose(filePtr);

    if(lines_read == 0){
        printf(WARNING_COLOR);
        printf("File was empty, no participants were inserted");
        printf(RESET_COLOR);
        return 0;
    } else{
        printf("Insertion has been completed\n");
    }
    return 1;
}