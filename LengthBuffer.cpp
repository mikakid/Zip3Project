/**
 * LengthBuffer.cpp
 * Member function definitions for the LengthBuffer class.
 */
#include "LengthBuffer.h"
#include "ZipCode.h"
#include <iostream>
#include <string> 

using namespace std;
/*
 * @brief General constructor for the LengthBuffer.
 */
LengthBuffer::LengthBuffer() {
    delim = ',';
    index = 0;
    buffer = "";
    size = 0;
    max = 1000;
}

/*
 * @brief Builds the specified constructor.
 * @pre Receives the delimiter and the maximum buffer size.
 * @param1 delimiter a character, default is a comma.
 * @param2 maxsize an integer, the maximum size of the buffer.
 */
LengthBuffer::LengthBuffer(char delim, int max) {
    this->delim = delim;
    this->index = 0;
    this->buffer = "";
    this->size = 0;
    this->max = max;
}

/*
 * @brief Writes to the index file.
 * @pre Receives the index file and an instance of the ZipCode class.
 * @param1 outFile an ofstream variable containing the index file address.
 * @param2 record a ZipCode class variable.
 * @post Returns true or false if the file wrote correctly.
 */
void LengthBuffer::write(fstream& outFile) {
    buffer.insert(0, to_string(buffer.size()));
    outFile << buffer;
    buffer = "";
}

/*
 * @brief Reads from the length-indicated file.
 * @pre Receives a file and the offset for a record.
 * @param1 inFile an ifstream variable containing the address of the length-indicated file.
 * @param2 offset an integer variable containing the offset for the specific record.
 */
bool LengthBuffer::read(fstream& inFile, unsigned long offset) {
    char temp1, temp2;
    int length;
    index = 0;
    buffer = "";
    size = 0;

    inFile.seekg(offset);    // seek to start of record

    temp1 = inFile.get();   // get first two characters to decode length
    temp2 = inFile.get();
    buffer.push_back(temp1);   // add characters to buffer
    buffer.push_back(temp2);

    length = stoi(buffer);     // convert length ascii to int

    buffer = "";

    int i = 0;
    while (i < length) {
        inFile.get(temp1);
        buffer.push_back(temp1);
        size++;
        i++;
    }

    return true;
}

/*
 * @brief Unpacks a string of record fields.
 * @pre Receives a string of fields.
 * @param1 field a string reference of record fields.
 * @post Returns true if the string was unpacked; otherwise, it returns false.
 */
bool LengthBuffer::unpack(string& field) {
    if (index != size && size != 0) {   // execute only when LengthBuffer is not empty

        while (index < size && buffer[index] != delim) {
            field.push_back(buffer[index++]);
        }
        if (buffer[index] == delim)
            index++;
        return true;

    }
    return false;
}

void LengthBuffer::pack(string& field) {
    buffer.append(field);
}
