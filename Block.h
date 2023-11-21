/**
 * @file Block.h
 * @brief Defines the Block class for managing collections of ZipCode records.
 */
#pragma once


#include "zipCode.h"
#include <string>
#include <vector>
using namespace std;


#ifndef BLOCK
#define BLOCK



class Block {
public:
     /**
     * @brief Default constructor.
     * @post Creates an empty Block.
     */
    // Default constructor
    Block();

    /**
     * @brief Copy constructor.
     * @pre Requires an existing Block object.
     * @post Creates a new Block as a copy of the given Block.
     */
    // Copy constructor
    Block(const Block& old);

    /**
     * @brief Merge constructor, merges two Blocks into one.
     * @pre Requires two Block objects to be merged.
     * @post Merges the contents of the two Blocks into one.
     */
    // Merge constructor
    Block(Block& firstBlock, Block& secondBlock);

    /**
     * @brief Inserts a new ZipCode record into the Block.
     * @pre The Block should not exceed a certain size limit.
     * @post Adds a record to the Block, returns true if successful, false otherwise.
     */
    // Inserts a new ZipCode record
    bool insertRecord(ZipCode& newZip);

    /**
     * @brief Removes a ZipCode record from the Block.
     * @pre Requires a valid zip code to remove from the Block.
     * @post Removes a record from the Block, returns true if successful, false otherwise.
     */
    // Removes a ZipCode record
    bool removeRecord(int zip);

    /**
     * @brief Splits the Block into two parts.
     * @pre Requires an empty Block to split the contents into.
     * @post Divides the records between the current and the new Block.
     */
    // Splits the Block into two parts
    void divideBlock(Block& newBlock);

    // Getters with Doxygen @brief tags
    /**
     * @brief Get the next block index.
     */
    int getNextIndex() const { return next; };

    /**
     * @brief Get the previous block index.
     */
    int getPreviousIndex() const { return prev; };

    /**
     * @brief Check if the block is active.
     */
    bool isActive() const { return active; };

    /**
     * @brief Get the record count of the block.
     */
    int getRecordCount() const { return recCount; };

    /**
     * @brief Get the current size of the block.
     */
    int getSize() const { return currentSize; };

    /**
     * @brief Get the highest ZIP code in the block.
     */
    int getMaximumZip() const { return highestZip; };

    // Other methods
    void fetchRecords(vector<ZipCode>& recordsOut) const;
    bool searchZip(ZipCode& resultZip, int target);

    // Setters
    void setActiveState(bool state) { active = state; };
    void setNextIndex(int next) { this->next = next; };
    void setPreviousIndex(int prev) { this->prev = prev; };
    void setRecordCount(int recCount) { this->recCount = recCount; };
    void setSize(int currentSize) { this->currentSize = currentSize; };
    void setMaximumZip(int highestZip) { this->highestZip = highestZip; };

    // Calculate the highest ZIP code
    int calculateHighestZip();

private:
    // Calculate the size of a ZipCode record
    int calculateZipSize(const ZipCode& zipper) const;

    // Calculate the size of the Block header
    int calculateHeaderSize() const;

    // Member variables
    bool active;
    int prev, next;
    int highestZip, recCount, currentSize;
    vector<ZipCode> records;
};
