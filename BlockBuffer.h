// BlockBuffer.h
#pragma once

#ifndef BLOCKBUFFER
#define BLOCKBUFFER

#include <vector>
#include <iostream>
#include <string>
#include "ZipCode.h"
#include "Buffer_Record.h"
#include "Block.h"

using namespace std;

const int BUFSIZE = 512;

class BlockBuffer {
public:
    /**
     * @brief Constructs a BlockBuffer with an empty text buffer.
     */
    BlockBuffer() : blockText(""), index(0) {}

    /**
     * @brief Reads a block from a file based on its relative block number.
     * @param infile The input file stream to read from.
     * @param RBN The relative block number indicating the specific block in the file.
     * @post The content of the specified block is loaded into blockText.
     */
    void read(ifstream& infile, int RBN);

    /**
     * @brief Converts a Block object into a text representation.
     * @param b The Block object to be converted into text.
     * @post The Block object's data is converted into a string and stored in blockText.
     */
    void pack(Block& b);

    /**
     * @brief Writes the content of blockText to a file at a specific block position.
     * @param outfile The output file stream where the blockText will be written.
     * @param RBN The relative block number indicating the position in the file to write.
     * @post The content of blockText is written to the file at the specified block position.
     */
    void write(ofstream& outfile, int RBN);

    /**
     * @brief Parses the blockText into a Block object.
     * @param b An empty Block object that will be filled with data from blockText.
     * @post The blockText is parsed and its data is stored into the provided Block object.
     */
    void unpack(Block& b);

    /**
     * @brief Retrieves the content of the blockText buffer.
     * @return A string containing the content of blockText.
     */
    string getText() const { return blockText; };

    /**
     * @brief Clears the contents of the BlockBuffer.
     * @post blockText is cleared and the index is reset.
     */
    void clear() { blockText.clear(); index = 0; };

private:
    /**
     * @brief Reads and parses the header data from a Block object.
     * @param b The Block object from which the header data will be read.
     * @post The header data is extracted and stored into the provided Block object.
     */
    void readHeader(Block& b);

    /**
     * @brief Generates a string representation of a Block object's header data.
     * @param b The Block object whose header data will be written to a string.
     * @return A string containing the header data of the Block object.
     */
    string writeHeader(Block& b);

    string blockText;  // Text buffer for storing block content
    Block obj;         // Block object for temporary storage
    int index;         // Index used in reading and writing operations
};

#endif // BLOCKBUFFER
