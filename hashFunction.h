//
// Created by jonas on 5/5/2020.
//

#ifndef CHEATERS_HASHFUNCTION_H
#define CHEATERS_HASHFUNCTION_H

#include <iostream>
#include <string>
#include <vector>


using namespace std;

int const EMPTY=-1;
int const TABLE_LENGTH=104729;

struct hashNode {
    int sourceFile;
    hashNode *next;
};

int hashString(vector<string> chunk);
void putLinkedList(hashNode *node, int sourceFile);
void matchingChunks(int *result[],hashNode *node);

#endif //CHEATERS_HASHFUNCTION_H
