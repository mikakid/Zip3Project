/**
 * PrimaryIndex.h
 * Class containing the primary index and the byte offset of the data file
 * for the corresponding primary key.
 */

#ifndef PRIMARYINDEX_H
#define PRIMARYINDEX_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include "LengthBuffer.h"
#include "zipCode.h"
#include "delimBuffer.h"

struct IndexElement {

    int zip;
    unsigned long int offset;
};

class PrimaryIndex {

private:

    string printTable(vector<vector<ZipCode>>&); // output data table

    short stateSelector(const string& stateCode);    // return index of state with the given 2-letter code

    short northest(vector<ZipCode>); // searches a given state to find the most northern zip code

    short southest(vector<ZipCode>); // searches a given state to find the most southern zip code

    short eastest(vector<ZipCode>); // searches a given state to find the most eastern zip code

    short westest(vector<ZipCode>); // searches a given state to find the most western zip code

    string readIn(ifstream& inFile, vector<vector<ZipCode>>& states);

    unsigned long binarySearch(int target, int left, int right);

    void transfer(vector<vector<ZipCode>>&, string);

    string buildHeader(string);

    vector<IndexElement> index;
    int recordCount;
    fstream dataFile, indexFile;    

public:

    PrimaryIndex(string indexFileName = "IndexFile.index", string dataFileName = "DataFile.licsv") { 
        indexFile.open(indexFileName); dataFile.open(dataFileName); readIndex(); indexFile.close(); dataFile.close(); }

    PrimaryIndex(ifstream& infile) { 
        readCSV(infile); }

    void add(int zipCode, unsigned long offset);

    unsigned long search(int targetZipCode);

    void writeToFile();

    void readIndex();

    void readCSV(ifstream&);

    void getIndex(vector<IndexElement>& returnValue);

    int getSize() { 
        return index.size(); }

    int getOffset(int i) { return index[i].offset; };

};

#endif
