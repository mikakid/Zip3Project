/**
 * @file Block.cpp
 * @brief Implementation of the Block class for managing collections of ZipCode records.
 * @author Group 7
 * @details This file contains the implementation of the Block class, which includes
 *          methods for manipulating blocks of ZipCode records, such as adding, removing,
 *          and splitting blocks.
 */

#include "Block.h"

/**
 * @brief Constructor that initializes a new, empty block.
 * @post Creates a block with default initial values and clears any existing records.
 */
// Constructor: Initializes a new, empty block
Block::Block() {
    active = false;
    recCount = 0;
    currentSize = calculateHeaderSize() + 1;
    highestZip = 0;
    prev = 0; 
    next = 0;
    records.clear();
}

/**
 * @brief Copy constructor for creating a copy of an existing block.
 * @param old A constant reference to the Block object to be copied.
 * @post Creates a new Block object as a copy of the provided Block.
 */
// Copy constructor: Creates a copy of an existing block
Block::Block(const Block& old) {
    active = old.active;
    recCount = old.recCount;
    currentSize = old.currentSize;
    highestZip = old.highestZip;
    prev = old.prev;
    next = old.next;
    records = old.records; // Using direct assignment for vector copy
}

/**
 * @brief Merge constructor that merges two Blocks into one.
 * @param firstBlock A reference to the first Block object to be merged.
 * @param secondBlock A reference to the second Block object to be merged.
 * @post Merges the contents of both Blocks into a single new Block.
 */
// Merge constructor: Merges two blocks into one
Block::Block(Block& firstBlock, Block& secondBlock) {
    if (secondBlock.getMaximumZip() < firstBlock.getMaximumZip()) {
        swap(firstBlock, secondBlock);
    }

    records.reserve(firstBlock.records.size() + secondBlock.records.size());
    records.insert(records.end(), firstBlock.records.begin(), firstBlock.records.end());
    records.insert(records.end(), secondBlock.records.begin(), secondBlock.records.end());

    active = true;
    recCount = firstBlock.recCount + secondBlock.recCount;
    currentSize = firstBlock.currentSize + secondBlock.currentSize;
    prev = firstBlock.prev;
    next = secondBlock.next;
    secondBlock.active = false;

    calculateHighestZip();
}

/**
 * @brief Inserts a new ZipCode record into the Block.
 * @param newZip A reference to the ZipCode object to be added.
 * @pre The Block should not exceed a certain size limit.
 * @post Adds a ZipCode record to the Block. Returns true if successful, false otherwise.
 * @return Boolean indicating whether the record was successfully added.
 */
// Inserts a new ZipCode record into the block
bool Block::insertRecord(ZipCode& newZip) {
    int tempsize = calculateHeaderSize();
    int count = calculateZipSize(newZip);

    if (count + currentSize < 512) {
        auto position = lower_bound(records.begin(), records.end(), newZip,
                                    [](const ZipCode& a, const ZipCode& b) { return a.getNum() < b.getNum(); });
        records.insert(position, newZip);
        recCount++;
        calculateHighestZip();
        currentSize = (currentSize - tempsize) + calculateHeaderSize();
        return true;
    }
    return false;
}

/**
 * @brief Splits the current Block into two by dividing its records.
 * @param newBlock A reference to the Block where the second half of records will be moved.
 * @pre Assumes that newBlock is empty.
 * @post Divides the records between the current Block and newBlock.
 */
// Splits the block into two blocks
void Block::divideBlock(Block& newBlock) {
    int midpoint = records.size() / 2;
    newBlock.records.assign(records.begin() + midpoint, records.end());
    newBlock.recCount = newBlock.records.size();
    newBlock.calculateHighestZip();
    newBlock.currentSize = newBlock.calculateHeaderSize() + newBlock.calculateZipSize(newBlock.records);

    records.resize(midpoint);
    recCount = records.size();
    calculateHighestZip();
    currentSize = calculateHeaderSize() + calculateZipSize(records);

    newBlock.setNextIndex(getNextIndex());
    newBlock.setPreviousIndex(this); // Assuming 'prev' is an int, not a pointer
}

/**
 * @brief Removes a ZipCode record from the Block.
 * @param zip The zip code number of the record to remove.
 * @pre The Block must contain the ZipCode record to be removed.
 * @post Removes the specified ZipCode record from the Block. Decrements the record count.
 * @return Boolean indicating whether the record was successfully removed.
 */
// Removes a ZipCode record from the block
bool Block::removeRecord(int zip) {
    auto it = find_if(records.begin(), records.end(), [zip](const ZipCode& record) { return record.getNum() == zip; });
    if (it != records.end()) {
        int count = calculateZipSize(*it);
        records.erase(it);
        recCount--;
        currentSize -= count;
        calculateHighestZip();
        active = recCount > 0;
        return true;
    }
    return false;
}

/**
 * @brief Calculates the highest ZIP code among the records in the block.
 * @return The highest ZIP code value in the block.
 * @post Sets the highestZip member to the highest ZIP code found.
 */
// Calculates the highest ZIP code in the block
int Block::calculateHighestZip() {
    highestZip = records.empty() ? 0 : max_element(records.begin(), records.end(), 
        [](const ZipCode& a, const ZipCode& b) { return a.getNum() < b.getNum(); })->getNum();
    return highestZip;
}

/**
 * @brief Calculates the size of a ZipCode record.
 * @param zipper A constant reference to the ZipCode object.
 * @return The size of the ZipCode record in bytes.
 */
// Calculates the size of a ZipCode record
int Block::calculateZipSize(const ZipCode& zipper) const {
    ostringstream oss;
    oss << zipper.getNum() << ',' << zipper.getCity() << ',' << zipper.getStateCode() << ','
        << zipper.getCounty() << ',' << zipper.getLat() << ',' << zipper.getLon();
    return oss.str().size();
}

/**
 * @brief Calculates the size of the block header.
 * @return The size of the block header in bytes.
 */
// Calculates the size of the block header
int Block::calculateHeaderSize() const {
    ostringstream oss;
    oss << prev << ',' << next << ',' << recCount << ',' << currentSize << ',' << highestZip << ';';
    return oss.str().size();
}

/**
 * @brief Retrieves all ZipCode records in the block.
 * @param recordsOut A vector reference to store the fetched records.
 * @post The recordsOut vector contains all the records from the block.
 */
// Retrieves all ZipCode records in the block
void Block::fetchRecords(vector<ZipCode>& recordsOut) const {
    recordsOut = records;
}

/**
 * @brief Searches for a specific ZipCode in the block based on the given target number.
 * @param resultZip A reference to a ZipCode object to store the found record.
 * @param target The target zip code number to search for.
 * @return Boolean indicating whether the zip code was found.
 * @post If found, resultZip is set to the matching ZipCode record.
 */
// Searches for a specific ZipCode in the block
bool Block::searchZip(ZipCode& resultZip, int target) {
    auto it = find_if(records.begin(), records.end(), [target](const ZipCode& record) { return record.getNum() == target; });
    if (it != records.end()) {
        resultZip = *it;
        return true;
    }
    return false;
}
