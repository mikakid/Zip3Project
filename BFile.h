#pragma once

#ifndef BFILE
#define BFILE

#include "BlockBuffer.h"
#include "Buffer_Record.h"
#include "zipCode.h"
#include "Block.h"
#include "BlockIndex.h"
#include "LengthBuffer.h"
#include "PrimaryIndex.h"

const int FILESIZE = 512;

class BFile {
public:
    /**
     * @brief Constructs a new BlockFile object with default settings.
     */
    BFile() {
        // Default constructor implementation
    }

    /**
     * @brief Constructs a BlockFile object and opens a specific file.
     * @param fileName The name of the file to be opened.
     */
    BFile(string fileName) {
        open(fileName);
    }

    /**
     * @brief Converts a length index to a block structure.
     * @param indexString The string index to be converted.
     * @param lengthData The length indicated data to be processed.
     */
    void lengthIndexToBlock(string indexString, string lengthData);

    /**
     * @brief Opens a file for reading and writing operations.
     * @param fileName The name of the file to open.
     */
    void open(string fileName) {
        inputData.open(fileName);
        outputData.open(fileName);
    }

    /**
     * @brief Closes the currently opened file.
     */
    void close() {
        inputData.close();
        outputData.close();
    }

    /**
     * @brief Reads the header information from the current file.
     */
    void readHeader();

    /**
     * @brief Writes header information to the current file.
     * @return A string representation of the header.
     */
    string writeHeader();

    /**
     * @brief Provides a physical representation of the file's data.
     * @return String containing a physical dump of the file.
     */
    string physicalDump();

    /**
     * @brief Provides a logical representation of the file's data.
     * @return String containing a logical dump of the file.
     */
    string logicalDump();

    /**
     * @brief Splits a block into two separate blocks.
     * @param b The block to be split.
     * @return True if the split was successful, false otherwise.
     */
    bool split(Block& b);

    /**
     * @brief Adds a new ZipCode record to the file.
     * @param zipCode The ZipCode object to be added.
     * @return True if the record was added successfully, false otherwise.
     */
    bool addRecord(ZipCode& zipCode);

    /**
     * @brief Deletes a ZipCode record from the file.
     * @param zipCode The zip code of the record to delete.
     * @return True if the record was deleted successfully, false otherwise.
     */
    bool deleteRecord(string zipCode);

    /**
     * @brief Retrieves the first relative block number (RBN) in the file.
     * @return The first RBN as an integer.
     */
    int getFirstRBN() const { return firstRBN; }

    /**
     * @brief Retrieves the available space indicator in the file.
     * @return The available space as an integer.
     */
    int getAvailableSpace() const { return availableSpace; }

private:
    int firstRBN, availableSpace, totalBlocks, totalRecords;

    ofstream outputData;
    ifstream inputData;
    BlockBuffer blockBuffer;
    BlockIndex blockIndex;
};

#endif //BFILE
