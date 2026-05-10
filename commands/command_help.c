#include "./header_files/command.h"

int command_help(HashTable myHashTable, ListOfLists myLoL, char* delimiters){
    // help - Prints a list of all available commands

    printf(GRAY);
    printf("This program is used to update or access the state of participants in an election. \n");
    printf("Here is a list of all the available commands you could use:\n\n");

    printf(GREEN); printf("l "); printf(BLUE); printf("<PIN>                        "); printf(GRAY); printf("- Prints data of participant with specified PIN\n");
    printf(GREEN); printf("i "); printf(BLUE); printf("<PIN> <lname> <fname> <zip>  "); printf(GRAY); printf("- Inserts a new participant into the program\n");
    printf(GREEN); printf("if "); printf(BLUE); printf("<filepath>                  "); printf(GRAY); printf("- Inserts participants of <file> into the program\n");
    printf(GREEN); printf("m "); printf(BLUE); printf("<pin>                        "); printf(GRAY); printf("- Updates the state of the participant of specified PIN to has voted\n");
    printf(GREEN); printf("bv "); printf(BLUE); printf("<filepath>                  "); printf(GRAY); printf("- Updates the state of the participants of specified PINs written in <filepath> to has voted\n");
    printf(GREEN); printf("v "); printf(BLUE); printf("                             "); printf(GRAY); printf("- Returns number of participants that have voted so far\n");
    printf(GREEN); printf("perc "); printf(BLUE); printf("                          "); printf(GRAY); printf("- Returns percentage of participants that have voted so far\n");
    printf(GREEN); printf("z "); printf(BLUE); printf("<zipcode>                    "); printf(GRAY); printf("- Returns the PIN of participants who have voted of specified zip code\n");
    printf(GREEN); printf("o "); printf(BLUE); printf("                             "); printf(GRAY); printf("- Returns in order how many voters have voted of each zip code, order is from most voters to least voters\n");
    printf(GREEN); printf("save "); printf(BLUE); printf("<filepath>                "); printf(GRAY); printf("- Saves the current state of the program in <filepath>, file can then be used with -f flag to resume another time\n");
    printf(GREEN); printf("clear "); printf(BLUE); printf("                         "); printf(GRAY); printf("- Clears the terminal\n");
    printf(GREEN); printf("help "); printf(BLUE); printf("                          "); printf(GRAY); printf("- Prints a list of all available commands\n");
    printf(GREEN); printf("exit "); printf(BLUE); printf("                          "); printf(GRAY); printf("- Exits the program, returns how many bytes were freed in the process\n");

    printf(RESET_COLOR);
    return 1;
}
