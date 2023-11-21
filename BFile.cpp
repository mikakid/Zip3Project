/**
 * @file BFile.cpp
 * @brief Implementation of the BFile class for managing block file operations.
 */

#include "BFile.h"

/**
 * @brief Default constructor for BFile.
 * Initializes the class without opening a file.
 */
BFile::BFile() {
    string index = "IndexFile.index";
    string data = "data.txt";

    outputData.open("DataFile.txt");
    inputData.open("DataFile.txt");

    for (int i = 0; i < 1024; i++) {
        outputData << '0';
    }

    lengthIndexToBlock(index, data);
}

/**
 * @brief Converts a length index to a block structure.
 * @param indexString The string index to be converted.
 * @param lengthData The length indicated data to be processed.
 */
// Converts a length index to a block structure.
void BFile::lengthIndexToBlock(string indexString, string lengthData) {
    Buffer_Record recParser;
    LengthBuffer libuf;
    ZipCode z;
    PrimaryIndex pi(indexString, lengthData);

    vector<IndexElement> ind;
    pi.getIndex(ind);

    fstream lid;
    lid.open(lengthData);

    for (int i = 0; i < ind.size(); i++) {
        libuf.read(lid, ind[i].offset);
        recParser.read(libuf.getBuffer());
        recParser.unpack(z);
        addRecord(z);
    }
    outputData.seekp(0);
    outputData << writeHeader();
}

/**
 * @brief Deletes a record based on the given zip code.
 * @param zipCode The zip code of the record to be deleted.
 * @return True if deletion is successful, false otherwise.
 */
// Deletes a record based on address.
bool BFile::deleteRecord(string zipCode) {
    Block currentBlock, previousBlock, nextBlock;
    int rbn = blockIndex.Search(stoi(zipCode));
    int prevRbn, nextRbn;

    if (rbn == 0)
        return false;
    else {
        blockBuffer.read(inputData, rbn);
        blockBuffer.unpack(currentBlock);
        prevRbn = currentBlock.getPreviousIndex();
        nextRbn = currentBlock.getNextIndex();

        if (currentBlock.removeRecord(stoi(zipCode))) {
            if (currentBlock.getSize() < 256) {
                if (prevRbn != 0) {
                    blockBuffer.clear();
                    blockBuffer.read(inputData, prevRbn);
                    blockBuffer.unpack(previousBlock);
                }
                if (nextRbn != 0) {
                    blockBuffer.clear();
                    blockBuffer.read(inputData, nextRbn);
                    blockBuffer.unpack(nextBlock);
                }
                if (previousBlock.getSize() < 256 && currentBlock.getSize() < 256) {
                    Block mergedBlock(previousBlock, currentBlock);
                    blockIndex.Add(mergedBlock, prevRbn);
                    blockBuffer.pack(mergedBlock);
                    blockBuffer.write(outputData, prevRbn);
                    nextBlock.setPreviousIndex(prevRbn);
                    blockBuffer.clear();
                    blockBuffer.pack(nextBlock);
                    blockBuffer.write(outputData, nextRbn);
                    return true;
                }
                if (nextBlock.getSize() < 256 && currentBlock.getSize() < 256) {
                    Block mergedBlock(currentBlock, nextBlock);
                    blockIndex.Add(mergedBlock, nextRbn);
                    blockBuffer.pack(mergedBlock);
                    blockBuffer.write(outputData, nextRbn);
                    previousBlock.setNextIndex(nextRbn);
                    blockBuffer.clear();
                    blockBuffer.pack(previousBlock);
                    blockBuffer.write(outputData, prevRbn);
                    return true;
                }
            }
            return true;
        } else
            return false;
    }
}

/**
 * @brief Adds a new ZipCode record to the block file.
 * @param z The ZipCode object to be added.
 * @return True if the record was added successfully, false otherwise.
 */
// Adds a new ZipCode record to the file.
bool BFile::addRecord(ZipCode &z) {
    Block tempBlock, newBlock;
    tempBlock.setActiveState(true);
    int rbn = blockIndex.Search(z.getNum());

    if (rbn == 0) {
        if (totalBlocks == 0) totalBlocks++;
        rbn = blockIndex.FindHighest();

        if (rbn == 0) {
            tempBlock.insertRecord(z);
            tempBlock.setPreviousIndex(0);
            tempBlock.setNextIndex(0);
            blockIndex.Add(tempBlock, 1);

            blockBuffer.pack(tempBlock);
            blockBuffer.write(outputData, 1);
            blockBuffer.clear();

            return true;
        } else {
            blockBuffer.read(inputData, rbn);
            blockBuffer.unpack(tempBlock);
            blockBuffer.clear();
            blockIndex.Add(tempBlock, rbn);

            if (!tempBlock.insertRecord(z)) {
                split(tempBlock);
                return addRecord(z);
            } else {
                blockBuffer.pack(tempBlock);
                blockBuffer.write(outputData, rbn);
                blockBuffer.clear();
                return true;
            }
        }
    } else {
        blockBuffer.read(inputData, rbn);
        blockBuffer.unpack(tempBlock);
        blockBuffer.clear();
        blockIndex.Add(tempBlock, rbn);

        if (!tempBlock.insertRecord(z)) {
            split(tempBlock);
            return addRecord(z);
        } else {
            blockIndex.Add(tempBlock, rbn);
            blockBuffer.pack(tempBlock);
            blockBuffer.write(outputData, rbn);
            blockBuffer.clear();
            return true;
        }
    }
    return false;
}

/**
 * @brief Reads the header information from the current file.
 */
// Reads the file header.
void BFile::readHeader() {
    string temp;
    inputData.seekg(0);
    for (int i = 0; i < FILESIZE; i++) {
        temp.push_back(inputData.get());
    }
}

/**
 * @brief Writes header information to the current file.
 * @return A string representation of the header.
 */
// Writes header information to the file.
string BFile::writeHeader() {
    string header;
    // File Structure Type
    header.append("File Structure: Blocked sequence set with comma separated fields, and length-indicated records\n");

    // File Version
    header.append("Version: 1.0\n");

    // Header record size
    header.append("Header Size: 512 bytes\n");

    // Size Format
    header.append("Format: ASCII\n");

    // Block size
    header.append("Block Size: 512 bytes\n");

    // Minimum block capacity
    header.append("Min Block Capacity: 256 bytes\n");

    // Index File Name
    header.append("Index File: Index\n");

    // Index File Schema
    header.append("File Schema: Highest zip code and RBN\n");

    // Record count
    header.append("Record Count: ");
    header.append(to_string(totalRecords));
    header.push_back('\n');

    // Block count
    header.append("Block Count: ");
    header.append(to_string(totalBlocks));
    header.push_back('\n');

    // Fields per record
    header.append("Fields: 6\n");

    // Fields
    header.append("ZipCode, Place Name, State, County, Lat, Long\n");

    // Type Schema
    header.append("Type Schema: Zip Code is an integer. Lon and Lat are floating point decimals. County, State Code, and City are strings.\n");

    // Primary key
    header.append("Zip code is the first key\n");

    // RBN link to avail list
    header.append("First Available Block: ");
    header.append(to_string(availableSpace));
    header.push_back('\n');

    // RBN link to active list
    header.append("First Active Block: ");
    header.append(to_string(firstRBN));
    header.push_back('\n');

    // Stale flag
    header.append("Stale: true");
    header.push_back('\n');

    return header;
}

/**
 * @brief Provides a physical representation of the file's data.
 * @return String containing a physical dump of the file.
 */
// Provides a physical dump of the file's data.
string BFile::physicalDump() {
    string output = "";
    output.append("List Head: ");
    output.append(to_string(getFirstRBN()));
    output.append("\nAvail Head: ");
    output.append(to_string(getAvailableSpace()));
    output.append("\n");

    Block tempBlock;
    vector<ZipCode> records;

    for (int i = 1; i <= totalBlocks; ++i) {
        blockBuffer.read(inputData, i);
        blockBuffer.unpack(tempBlock);
        blockBuffer.clear();

        if (tempBlock.isActive()) {
            output.append("RBN Prev: ");
            output.append(to_string(tempBlock.getPreviousIndex()));
            tempBlock.fetchRecords(records);

            for (int j = 0; j < tempBlock.getRecordCount(); j++) {
                output.push_back(' ');
                output.append(to_string(records[j].getNum()));
                output.push_back(' ');
            }

            output.append("RBN Next: ");
            output.append(to_string(tempBlock.getNextIndex()));
            output.append("\n");
        } else {
            output.append("RBN Prev:0\t*AVAILABLE*\tRBN Next: 0\n");
        }
    }

    return output;
}

/**
 * @brief Provides a logical representation of the file's data.
 * @return String containing a logical dump of the file.
 */
// Provides a logical dump of the file's data.
string BFile::logicalDump() {
    int rbn = 1;
    Block tempBlock;
    string zips;
    vector<ZipCode> records;

    zips.append("List Head: ");
    zips.append(to_string(getFirstRBN()));
    zips.append("\nAvail Head: ");
    zips.append(to_string(getAvailableSpace()));
    zips.append("\n");

    for (int i = 1; i <= totalBlocks; ++i) {
        if (rbn == 0) break;
        blockBuffer.read(inputData, rbn);
        blockBuffer.unpack(tempBlock);
        blockBuffer.clear();

        if (tempBlock.isActive()) {
            zips.append("RBN Prev: ");
            zips.append(to_string(tempBlock.getPreviousIndex()));
            tempBlock.fetchRecords(records);

            for (int j = 0; j < records.size(); j++) {
                zips.append(to_string(records[j].getNum()));
                zips.push_back(' ');
            }

            zips.append("RBN Prev: ");
            zips.append(to_string(tempBlock.getNextIndex()));
            zips.push_back('\n');
            rbn = tempBlock.getNextIndex();
        } else {
            zips.append("RBN Prev:0\t*AVAILABLE*\tRBN Next: 0\n");
        }
    }

    return zips;
}

/**
 * @brief Splits a block into two separate blocks.
 * @param b The block to be split.
 * @return True if the split was successful, false otherwise.
 */
// Splits a block into two parts.
bool BFile::split(Block& b) {
    if (b.isActive()) {
        int rbn, tempIndex;
        Block tempBlock1, tempBlock2;

        rbn = blockIndex.Search(b.calculateHighestZip());
        b.divideBlock(tempBlock1);

        if (b.getNextIndex() == 0) {
            tempBlock1.setNextIndex(0);
        } else {
            tempIndex = b.getNextIndex();
            blockBuffer.read(inputData, tempIndex);
            blockBuffer.unpack(tempBlock2);

            tempBlock1.setNextIndex(tempIndex);
            tempBlock2.setPreviousIndex(totalBlocks);
            blockBuffer.clear();
        }

        b.setNextIndex(++totalBlocks);
        tempBlock1.setPreviousIndex(rbn);
        tempBlock1.setActiveState(true);

        blockBuffer.pack(b);
        blockBuffer.write(outputData, rbn);
        blockBuffer.clear();
        blockIndex.Add(b, rbn);

        blockBuffer.pack(tempBlock1);
        blockBuffer.write(outputData, totalBlocks);
        blockBuffer.clear();
        blockIndex.Add(tempBlock1, totalBlocks);

        return true;
    }
    return false;
}
