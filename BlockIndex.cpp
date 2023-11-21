/*
BlockIndex.cpp
*/
#include "BlockIndex.h"

using namespace std;

int BlockIndex::FindHighest() {
    int tempZip = 0;
    int tempRBN = 0;

    if (index.size() == 0) {
        return 0;
    }

    int i = 0;
    while(i < index.size()){
        if (index[i].zipCode > tempZip) {
            tempZip = index[i].zipCode;
            tempRBN = index[i].RBN;
        }
        i++;
    }
   
    return tempRBN;
}

int BlockIndex::Search(int zip) {

    if (index.size() == 0)
        return 0;

    int tempRBN = 0;
    bool found = false;
    int tempZip;

    int i = 0;
    while (i < index.size()) {
    if (found) {
        if (index[i].zipCode >= zip && index[i].zipCode < tempZip) {
            tempRBN = index[i].RBN;
            tempZip = index[i].zipCode;
        }
    } else if (index[i].zipCode >= zip) {
        tempRBN = index[i].RBN;
        tempZip = index[i].zipCode;
        found = true;
    }
    i++;
  }
  return tempRBN;
}

void BlockIndex::Add(Block& b, int r) {
    BlockIndexVariables temp;
    temp.zipCode = b.getMaximumZip();
    temp.RBN = r;
    temp.active = true;
    Del(r);
    index.push_back(temp);
}

void BlockIndex::Del(int r) {
    if (index.size() == 0) {
        return;
    }
    for (int i = 0; i < index.size(); i++) {
        if (index[i].RBN == r) {
            index.erase(index.begin() + i);
            break;
        }
    }
}

void BlockIndex::ReadFromFile(string in) {

    ifstream iFile;
    iFile.open(in);
    char trash;
    BlockIndexVariables temp;

    if (iFile >> numBlocks >> trash >> numAvail >> trash) {

        index.resize(1);
        iFile >> index[0].zipCode >> trash >> index[0].RBN >> trash >> index[0].active >> trash;

        for (int i = 0; i < numBlocks && !iFile.eof(); i++) {

            iFile >> temp.zipCode >> trash >> temp.RBN >> trash >> temp.active >> trash;
            index.push_back(temp);
        }
    }
}

void BlockIndex::PrintToFile(string out) {

    ofstream oFile;
    oFile.open(out);

    oFile << numBlocks << ',' << numAvail << ';';

    int i = 0;
    while (i < index.size()) {
        oFile << index[i].zipCode << ',' << index[i].RBN << ',' << index[i].active << ';';
        i++;
    }

}
