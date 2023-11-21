/**
* delimBuffer.CPP
* Member function definitions for the delimBuffer class.  
*/
#include "delimBuffer.h"
#include <iostream>
#include <string> 

/*
@brief General constructor for the DelimBuffer 
*/
delimBuffer::delimBuffer() {
	delim = ',';
	index = 0;
	buffer = "";
	size = 0;
	max = 1000;
}

/*
@brief Builds the specified constructor. 
@pre Receives the delimeter, and the maximum integer.
@param1 delim a character which is a comma.  
@param2 maxsize an int which is the maxsizef the buffer.
*/
delimBuffer::delimBuffer(char delim = ',', int maxsize = 1000) {
	index = 0;
	buffer = "";
	size = 0;
}

/*
@brief Reads from the delim file. 
@pre Receives a file and the and the offset for a record. 
@param1 inFile an ifstream variable which contains the address of the length-indicated file.  
@param2 offset an integer variable which contains the offset for the specific record.
*/
bool delimBuffer::read(ifstream& inFile) {
	index = 0;
	buffer = "";
	if (inFile.is_open() && !inFile.eof()) {				// execute only when the file is open and not at the end of the file
		getline(inFile,buffer);								// pull everything up to the next newline
		size = buffer.size();
		return true;
	}
	else
		return false;

}


bool delimBuffer::unpack(string& field) {
	if (index != size && size != 0) { // execute only when delimBuffer is not empty

		while (index < size && buffer[index] != delim) { // 
			field.push_back(buffer[index++]);
		}
		if (buffer[index] == delim)
			index++;
		return true;

	}
	return false;
}
