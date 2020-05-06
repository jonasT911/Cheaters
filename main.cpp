#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


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


int main() {

    vector<string> files = vector<string>();

    ifstream inFile;

    vector<string> chunk = vector<string>();

    int numChunk = 6;
    string dir = string("sm_doc_set");
    //files will be taken from the command line
    //numcheck will be taken from the command line

    getdir(dir, files);

    string fileName;
    string s;


    for (unsigned int i = 2; i < files.size(); i++) {
        cout << i << files[i] << endl;

        fileName = dir + "/" + files[i];

        cout << "opening " << fileName << endl;
        //Run file parsing
        inFile.open(fileName);

        string word;

        for (int j = 0; j < numChunk; j++) {
            inFile >> word;
            chunk.push_back(word);
        }


        if (i == 2) {
            while (inFile) {
                // printChunk(chunk);
                chunk.erase(chunk.begin());
                inFile >> word;
                chunk.push_back(word);


            }
        }


        while (chunk.size()) {
            chunk.pop_back();
        }
        inFile.close();
    }


    return 0;
}