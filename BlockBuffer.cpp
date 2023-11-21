/**
 * @file BlockBuffer.cpp
 * @brief Implementation of the BlockBuffer class for handling block operations in files.
 */

#include "BlockBuffer.h"

/**
 * @brief Reads a block from a file based on its relative block number.
 * @param inFile The input file stream to read from.
 * @param RBN The relative block number indicating the specific block in the file.
 */

void BlockBuffer::read(ifstream& inFile, int RBN) {
    unsigned long NBR = static_cast<unsigned long>(RBN) * 512;
    int index = 0;
    inFile.seekg(NBR);
    while (!inFile.eof() && index != BUFSIZE) {
        blockText.push_back(inFile.get());
        index++;
    }
    index = 0;
}

/**
 * @brief Converts a Block object into a text representation.
 * @param b The Block object to be converted into text.
 */
// pack & storerecords
void BlockBuffer::pack(Block& b) {
    Buffer_Record rec;
    vector<ZipCode> records;

    string temp = writeHeader(b);
    blockText.append(temp);

    
    b.fetchRecords(records);
    for (auto& record : records) {
        rec.pack(record);
        rec.write(blockText);
    }
}

/**
 * @brief Writes the content of blockText to a file at a specific block position.
 * @param outfile The output file stream where the blockText will be written.
 * @param RBN The relative block number indicating the position in the file to write.
 */
void BlockBuffer::write(ofstream& outfile, int RBN) {
    unsigned long NBR = static_cast<unsigned long>(RBN) * 512;
    outfile.seekp(NBR);

    for (int i = 0; i < 512; i++) {
        outfile << (i < blockText.size() ? blockText[i] : ' ');
    }
    outfile.flush();
    blockText = "";
}

/**
 * @brief Parses the blockText into a Block object.
 * @param b An empty Block object that will be filled with data from blockText.
 */
void BlockBuffer::unpack(Block& b) {
    readHeader(b);
    // Unpack blockText into Block object
    ZipCode tempZip;
    string temp;
    Buffer_Record rec;
    int recSize;
    int count;
    int numRecs = b.getRecordCount();
    int recCounter = 0;
    int tempCurrentSize = b.getSize();

    b.setSize(0);

    while (index != tempCurrentSize) {
        if (recCounter == numRecs)
            break;

        while (blockText[index] != ',') {
            temp.push_back(blockText[index++]);
        }
        index++;
        recSize = stoi(temp) - 3;
        temp = "";
        count = 0;
        while (index != blockText.size() && count != recSize) {
            temp.push_back(blockText[index++]);
            count++;
        }

        rec.read(temp);
        rec.unpack(tempZip);

        b.insertRecord(tempZip);
        recCounter++;
        temp = "";
    }
    index = 0;
    blockText = "";
    b.setSize(tempCurrentSize);
}

/**
 * @brief Reads and parses the header data from a Block object.
 * @param b The Block object from which the header data will be read.
 */
// Parse header data from blockText
void BlockBuffer::readHeader(Block& b) {
    int headerSize;
    index = 0;
    string temp;

    for (index; blockText[index] != ','; index++) {
        temp.push_back(blockText[index]);
    }
    index++;
    b.setPreviousIndex(stoi(temp));
    temp = "";

    for (index; blockText[index] != ','; index++) {
        temp.push_back(blockText[index]);
    }
    index++;
    b.setNextIndex(stoi(temp));
    temp = "";

    for (index; blockText[index] != ','; index++) {
        temp.push_back(blockText[index]);
    }
    index++;
    b.setRecordCount(stoi(temp));
    temp = "";

    for (index; blockText[index] != ','; index++) {
        temp.push_back(blockText[index]);
    }
    index++;
    b.setSize(stoi(temp));
    temp = "";

    for (index; blockText[index] != ';'; index++) {
        temp.push_back(blockText[index]);
    }
    index++;
    b.setMaximumZip(stoi(temp));
    temp = "";
}

/**
 * @brief Generates a string representation of a Block object's header data.
 * @param b The Block object whose header data will be written to a string.
 * @return A string containing the header data of the Block object.
 */
string BlockBuffer::writeHeader(Block& b) {
// Create header string from Block object's attributes
    string header, temp;
// Store block attributes as ASCII
    header.append(to_string(b.getPreviousIndex()));
    header.push_back(',');
    header.append(to_string(b.getNextIndex()));
    header.push_back(',');
    header.append(to_string(b.getRecordCount()));
    header.push_back(',');
    header.append(to_string(b.getSize()));
    header.push_back(',');
    header.append(to_string(b.getMaximumZip()));
    header.push_back(';');
    return header;
}
