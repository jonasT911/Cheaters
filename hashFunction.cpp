//
// Created by jonas on 5/5/2020.
//

#include "hashFunction.h"

int letterNumber(char letter) {
    int number = letter;

    if (number >= 97) {
        number -= 32;
    }
    if (number >= 65 && number <= 90) {
        number -= 65;
    } else {
        if (number >= 48 && number <= 57) {
            number -= 20;
        } else {
            number = 0;
        }
    }

    return number;
}

int hashString(vector<string> chunk) {

    int shifter = 1;
    int result = 0;
    int letter = 0;

    for (int i = 0; i < chunk.size(); i++) {
        for (int j = 0; j < chunk[i].length(); j++) {
            letter = letterNumber(chunk[i].at(j));
            result = result + letter * shifter;
            shifter += 2;
        }

        shifter *= i * 3;
    }

    result = result % TABLE_LENGTH;

    return result;
}


void putLinkedList(hashNode *node, int sourceFile,int &ifc) {

    if (node->sourceFile != sourceFile) {
        if (node->next == nullptr) {
            hashNode *insert = new hashNode;

            node->next = insert;
            insert->next = nullptr;
            insert->sourceFile = sourceFile;
        } else {
            putLinkedList(node->next, sourceFile,ifc);
        }

    }else{
       ifc++;
      // cout<<"KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK"<<endl;
    }

}

void matchingChunks(int *result[], hashNode *node) {

    hashNode *lead = node;
    hashNode *trail = node;
    while (lead->next) {
        while (trail != lead) {

            if (trail->sourceFile < lead->sourceFile) {//smallest value goes in front
                result[trail->sourceFile][lead->sourceFile]++;
            } else {
                result[lead->sourceFile][trail->sourceFile]++;
            }

            trail = trail->next;
        }
        trail = node;
        lead = lead->next;
    }
}

void orderedInsert(vector<matchList> &list,matchList newNode){

    int iterator=0;
    if(list.empty()){
        list.push_back(newNode);
    }else{
        while(iterator<list.size() && (newNode.collisions<list[iterator].collisions)){

            iterator++;
        }
        list.insert(list.begin()+iterator,newNode);
    }
}
