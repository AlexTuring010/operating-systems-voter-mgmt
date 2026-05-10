// This is the header file of my command handler, responsible to handle commands

#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include "./colors.h"
#include "./../modules/header_files/voter.h"
#include "./../modules/header_files/hash_table.h"
#include "./../modules/header_files/ListOfLists.h"

int executeCommand(char* line, HashTable, ListOfLists);

#endif
