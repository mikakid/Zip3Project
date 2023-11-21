/**
 * LengthBuffer.h
 * Class for reading values from a specified file into a LengthBuffer.
 */

#ifndef LENGTHBUFFER_H
#define LENGTHBUFFER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/**
 * @brief Class to store each record and parse each field.
 */
class LengthBuffer {

private:
    int size;
    char delim;
    int max;
    int index;
    string buffer;

public:

    /**
     * @brief Constructor for the LengthBuffer class.
     * @pre Takes in the address of the us_postal_codes.csv file.
     * @post inFile, index, and buf are all initialized.
     */
    LengthBuffer();

    LengthBuffer(char delim, int bufferSize);

    void write(fstream& outFile);

    /**
     * @brief Reads from the CSV file and places it in a string.
     * @post Returns the string of one line from us_postal_codes.csv.
     */
    bool read(fstream& inFile, unsigned long offset);

    void pack(string& field);

    /**
     * @brief Separates each field from the line in the LengthBuffer.
     * @pre LengthBuffer must not be empty.
     * @post Makes the parameter string equal to the correct field in the record.
     */
    bool unpack(string& field);

    int getSize() { 
        return buffer.size(); }

    /**
     * @brief Gives the LengthBuffer string.
     * @post Returns the LengthBuffer string.
     */
    string getBuffer() { 
        return buffer; }

};
#endif
