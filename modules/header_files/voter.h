// Got to use the voter struct in multiple files so I made a header file for it to include anywhere I need
// The public information for each voter: PIN, surname, firstname and TK

// I've made it so that the module handles cases where malloc fails, it will
// reverse last actions, prevent leaks and let the user know it failed, so that
// the user can then decide how to move forward as he pleases.

#ifndef VOTERS_H  /* Using a header guard to prevent the header file from being included   */
#define VOTERS_H  /* more than once or cause an infinite chain of includes                 */
#include <stdio.h>


typedef struct voter* Voter;

Voter create_voter(int PIN, char* surname, char* firstname, int TK);

int getPin(Voter);                        // returns the pin

char* getSurName(Voter);                  // returns sur name

char* getFirstName(Voter);                // returns first name

int getTK(Voter);                         // returns the TK

char hasVoted(Voter);                     // returns 'Y' if yes and 'N' if no
 
void setToHasVoted(Voter);                // use when a voter votes to set him as hasVoted = 'Y'

void storeVoterInFile(Voter, FILE*);      // Stores voter in a file

int PINtoInt(int PIN);                    // Turns pin to an integer, for our case does nothing since pin is already an integer

int destroy_voter(Voter);                 // destroys the voter

#endif

// This module was made by Alex Gkiafis, sdi2200284