// This file was written by Alex Gkiafis, sdi2200284

#include "./header_files/voter.h"
#include <string.h>
#include <stdlib.h>

// From header file: typedef struct voter* = Voter
struct voter{
    int PIN;
    char* surname;
    char* firstname;
    int TK;
    char has_voted;
};

Voter create_voter(int PIN, char* surname, char* firstname, int TK){
    Voter new_voter = malloc(sizeof(struct voter));
    if(new_voter == NULL) return NULL;

    new_voter->surname = NULL; 
    new_voter->firstname = NULL;

    new_voter->surname = malloc((strlen(surname)+1) * sizeof(char));
    if(new_voter->surname == NULL){
        free(new_voter);
        return NULL;
    }

    strcpy(new_voter->surname, surname);

    new_voter->firstname = malloc((strlen(firstname)+1) * sizeof(char));
    if(new_voter->firstname == NULL){
        free(new_voter);
        free(new_voter->surname);
        return NULL;
    }

    strcpy(new_voter->firstname, firstname);

    new_voter->PIN = PIN;
    new_voter->TK = TK;
    new_voter->has_voted = 'N';

    return new_voter;
}

int getPin(Voter voter){
    return voter->PIN;
}

char* getSurName(Voter voter){
    return voter->surname;
}

char* getFirstName(Voter voter){
    return voter->firstname;
}

int getTK(Voter voter){
    return voter->TK;
}

char hasVoted(Voter voter){
    return voter->has_voted;
}

void setToHasVoted(Voter voter){
    voter->has_voted = 'Y';
}

void storeVoterInFile(Voter voter, FILE* fileptr){
    fprintf(fileptr, "%d %s %s %d %c\n", voter->PIN, voter->surname, voter->firstname, voter->TK, voter->has_voted);
}

int PINtoInt(int PIN){
    return PIN;
} 

int destroy_voter(Voter voter){
    int bytesFreed = strlen(voter->surname) + strlen(voter->firstname) + sizeof(struct voter) + 2;
    free(voter->surname);
    free(voter->firstname);
    free(voter);
    return bytesFreed;
}
