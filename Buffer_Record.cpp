/**
 * @file Buffer_Record.cpp
 * @brief Implementation of the Buffer_Record class for handling record buffering.
 * @author Group 7
 * @details This file implements the Buffer_Record class, providing functionality to pack, unpack, 
 *          read, and write ZipCode records into a string buffer.
 */

#include "Buffer_Record.h"

/**
 * @brief Default constructor for Buffer_Record.
 * @post Initializes a new Buffer_Record object with an empty buffer.
 */
// Packs a ZipCode object into the buffer
void Buffer_Record::pack(ZipCode& z) {
    std::string temp;

    temp.push_back(',');
    temp.append(std::to_string(z.getNum()));
    temp.push_back(',');
    temp.append(z.getCity());
    temp.push_back(',');
    temp.append(z.getStateCode());
    temp.push_back(',');
    temp.append(z.getCounty());
    temp.push_back(',');
    temp.append(std::to_string(z.getLat()));
    temp.push_back(',');
    temp.append(std::to_string(z.getLon()));
    buf.append(std::to_string(temp.size() + 2));
    buf.append(temp);
}

/**
 * @brief Reads a string into the buffer.
 * @param recText The record text to be read into the buffer.
 * @post The buffer is set to the contents of recText.
 */
// Reads a string into the buffer
void Buffer_Record::read(const std::string& recText) {
    buf = recText;
}

/**
 * @brief Writes the buffer content into a given string.
 * @param blockText A reference to the string where the buffer content will be written.
 * @post The buffer's content is appended to blockText, and the buffer is cleared.
 */
// Writes buffer content into a given string
void Buffer_Record::write(std::string& blockText) {
    blockText.append(buf);
    buf = "";
}

/**
 * @brief Unpacks the buffer into a ZipCode object.
 * @param z A reference to a ZipCode object to be filled with unpacked data.
 * @return True if unpacking is successful, false otherwise.
 * @post If successful, z is filled with the unpacked data from the buffer.
 */
// Unpacks the buffer into a ZipCode object
bool Buffer_Record::unpack(ZipCode& z) {
    std::string temp;
    int size = buf.size();
    temp = "";
    index = 0;

    int fieldNumber = 1;

    if (index != size) { // execute only when buf is not empty
        for (int i = 0; i < size && index < buf.size() + 1; i++) {
            if (buf[index] != delim) {
                temp.push_back(buf[index]);
                if (buf[index + 1] == delim || index + 1 == size) {
                    if (fieldNumber == 1) {
                        z.setNum(std::stoi(temp));
                    } else if (fieldNumber == 2) {
                        z.setCity(temp);
                    } else if (fieldNumber == 3) {
                        z.setStateCode(temp);
                    } else if (fieldNumber == 4) {
                        z.setCounty(temp);
                    } else if (fieldNumber == 5) {
                        z.setLat(std::stof(temp));
                    } else {
                        z.setLon(std::stof(temp));
                    }
                    fieldNumber++;
                    temp = "";
                }
            }
            index++;
        }
        return true;
    }

    return false;
}
/**
 * @brief Clears the contents of the buffer.
 * @post The buffer is cleared and reset.
 */
void Buffer_Record::clear() {
    // Implementation
}