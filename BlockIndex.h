/**
* BlockIndex.h 
* Class containing the block index for the highest zip in each block
* 
*/
#pragma once

#ifndef BLOCKINDEX_H
#define BLOCKINDEX_H

#include <fstream>
#include <vector>
#include "Block.h"

using namespace std;

struct BlockIndexVariables {
    int zipCode;
    int RBN;
    bool active;
};

class BlockIndex {

private:
    int numBlocks, numAvail;
    vector<BlockIndexVariables> index;

public:
    /*
    * @brief Default constructor
    * @pre
    * @post
    */
    BlockIndex() { 
        index.clear();
         }

    /*
    * @brief Search function
    * @pre Takes an integer ZIP to search through the index 
    * @post Returns the block number that would contain this zip as an int
    */
    int Search(int zipCode);

    int FindHighest();

    /*
    * @brief Print to file function
    * @post Outputs the content of the index to the file  
    */
    void PrintToFile(string);

    /*
    * @brief Add function
    * @pre Adds the first parameter as a zip and the second as a RBN onto the back of the index
    * @post Returns void
    */
    void Add(Block& b, int r);

    /*
    * @brief Delete function
    * @pre Takes a block number to delete from the index  
    * @post Returns void 
    */
    void Del(int r);

    /**
    * @brief
    * @post
    */
    void ReadFromFile(string);
    /*
    * @brief Get number of available function
    * @post Returns the number of blocks available as an int 
    */
    int GetNumAvail() { 
        return numAvail; 
        }

    /*
    * @brief Get number of blocks function
    * @post Returns the number of blocks as an int 
    */
    int GetNumBlocks() { 
        return numBlocks;
         };

};

#endif // BLOCKINDEX_H
