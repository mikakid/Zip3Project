
//Buffer_Record.h
#pragma once
#ifndef BUFFER_RECORD
#define BUFFER_RECORD

#include <iostream>
#include <string>
#include <fstream>
#include "ZipCode.h"

class Buffer_Record {
public:
    /**
     * @brief Default constructor.
     * @post Initializes a new Buffer_Record object.
     */
    // Default Constructor - Initializes a new Buffer_Record object
    Buffer_Record() : buf(""), size(buf.size()), index(0) {}

    /**
     * @brief Sets the buffer to a given string.
     * @param recText The record text to be read into the buffer.
     * @pre Takes a string given by the BlockBuffer class.
     * @post Adds the string to the buffer.
     */
    // Reads record text into the buffer
    void read(const std::string& recText);

    /**
     * @brief Writes the buffer to the blockText string.
     * @param blockText The string where the buffer content will be written.
     * @pre Takes a string given by the BlockBuffer class to be written to.
     * @post Clears the buffer.
     */
    // Writes buffer content into blockText
    void write(std::string& blockText);

    /**
     * @brief Parses a ZipCode object onto the buffer.
     * @param zipCode The ZipCode object to be packed into the buffer.
     * @pre Requires a ZipCode object.
     * @post The buffer is filled with a record.
     */
    // Packs a ZipCode object into the buffer
    void pack(ZipCode& zipCode);

    /**
     * @brief Parses the buffer and turns it into a ZipCode object.
     * @param zipCode An empty ZipCode object to be filled with unpacked data.
     * @return True if the buffer was successfully unpacked into a ZipCode object, false otherwise.
     * @pre Receives an empty ZipCode object.
     * @post Returns true if the buffer was made into a ZipCode object or false if it wasn't.
     */
    // Unpacks the buffer into a ZipCode object
    bool unpack(ZipCode& zipCode);

    /**
     * @brief Clears the buffer.
     * @pre None.
     * @post Buffer is empty.
     */
    // Clears the contents of the buffer
    void clear() {
        buf = "";
        size = 0;
        index = 0;
    }

private:
    std::string buf;      // The buffer for storing record data
    const char delim = ','; // Delimiter for separating record fields
    int size;              // Size of the buffer
    int index;             // Index for tracking read/write operations
};

#endif // BUFFER_RECORD
