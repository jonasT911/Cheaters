#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "hashFunction.h"


using namespace std;


/*function... might want it in some class?*/
int getdir(string dir, vector<string> &files) {
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

void printChunk(vector<string> chunk) {
    cout << "Chunk is =";
    for (int j = 0; j < chunk.size(); j++) {
        cout << chunk[j] << " ";
    }
    cout << endl;
}


int main(int argc, char *argv[]) {

    int ifc = 0;
    int tot = 0;


    hashNode hashTable[TABLE_LENGTH];

    for (int i = 0; i < TABLE_LENGTH; i++) {
        hashTable[i].sourceFile = EMPTY;
        hashTable[i].next = NULL;
    }

    vector<string> files = vector<string>();


    ifstream inFile;

    vector<string> chunk = vector<string>();

    int numChunk = stoi(argv[2]);// 6;
    int numMatches = stoi(argv[3]);//200;
    string dir = argv[1];//string("med_doc_set");
    //files will be taken from the command line
    //numcheck will be taken from the command line

    getdir(dir, files);

    string fileName;
    string s;
    int hashValue = 0;


    for (int i = 2; i < files.size(); i++) {
        //   cout << i << files[i] << endl;

        fileName = dir + "/" + files[i];

        // cout << "opening " << fileName << endl;
        //Run file parsing
        inFile.open(fileName);

        string word;

        for (int j = 0; j < numChunk; j++) {
            inFile >> word;
            chunk.push_back(word);
        }


        // if (i == 2) {//limit for test purposes
        while (inFile) {
            //printChunk(chunk);//debug

            //hash chunk
            hashValue = hashString(chunk);
            tot++;
            //put hash in table
            //  cout<<hashValue<<" "<<hashTable[hashValue].sourceFile<<endl;//debug



            if (hashTable[hashValue].sourceFile == EMPTY) {
                hashTable[hashValue].sourceFile = i;
            } else {
                if (hashTable[hashValue].sourceFile != i) {
                    if (i == 22) {
                        // cout<<" collision with "<<files[hashTable[hashValue].sourceFile];
                        //debug  printChunk(chunk);
                    }
                    putLinkedList(&hashTable[hashValue], i, ifc);

                } else {
                    // cout << "in file collision------------------------------------------" << hashValue << endl;//debug
                    ifc++;
                    //printChunk(chunk);//debug

                }
            }

            chunk.erase(chunk.begin()); //Cycle chunk
            inFile >> word;
            chunk.push_back(word);


        }
        //}


        while (chunk.size()) {
            chunk.pop_back();//clears chunk
        }
        inFile.close();

        //debug
        // cout << "There were " << ifc << " in file collisions" << endl;
        //  cout << "out of " << tot << " hash calls" << endl;

        ifc = 0;
        tot = 0;
    }


    int collisions[files.size()][files.size()];

    //clear array
    for (int i = 2; i < files.size(); i++) {
        for (int j = 2; j <= i; j++) {
            collisions[j][i] = 0;
        }
    }


    //run down list and calculate collisions between files
    for (int i = 0; i < TABLE_LENGTH; i++) {

        //If there is more than one file at a node
        if (hashTable[i].next != nullptr) {

            hashNode *lead = &hashTable[i];
            hashNode *trail = &hashTable[i];
            hashNode *deleter = &hashTable[i];
            //THing
            while (lead != nullptr) {
                while (trail != lead) {

                    //debug cout<<trail->sourceFile <<","<<lead->sourceFile<<endl;

                    if (trail->sourceFile < lead->sourceFile) {//smallest value goes in front
                        collisions[trail->sourceFile][lead->sourceFile]++;
                    } else {
                        collisions[lead->sourceFile][trail->sourceFile]++;
                    }

                    if (lead->next == nullptr) {
                        deleter = trail;
                        trail = trail->next;
                        delete deleter;
                    } else {
                        trail = trail->next;
                    }

                }
                trail = &hashTable[i];
                lead = lead->next;
            }

        }
    }



    //create vector of files with collisions over X
    vector<matchList> plagiarized;
    matchList node;
    for (int i = 2; i < files.size(); i++) {
        for (int j = 2; j <= i; j++) {
            if (collisions[j][i] > numMatches) {

                node.collisions = collisions[j][i];
                node.report = to_string(collisions[j][i]) + ": " + files[j] + ", " + files[i];
                //plagiarized.push_back(report);
                orderedInsert(plagiarized, node);
            }

        }
    }
    //print files with over set collisions in descending order
    // cout << "Printing matches" << endl;
    for (int i = 0; i < plagiarized.size(); i++) {
        cout << plagiarized[i].report << endl;
    }


    return 0;
}