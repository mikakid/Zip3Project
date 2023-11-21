/**
* delimBuffer.H
* Class for reading in values from a specified file to a delimBuffer.   
*/

#ifndef DELIMBUFFER_h
#define DELIMBUFFER_h

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

/**
@brief class to store each record and parse each field
*/
class delimBuffer {

private:
	char delim;
	int size;
	int max;
	int index;
	string buffer;

public:

	/**
	@brief Constructor for the delimBuffer class
	@pre Takes in the address to the us_postal_codes.csv file  
	@post inFile, index and buf are all initialized
	*/
	delimBuffer();

	delimBuffer(char, int);

    /**
	@brief Seperates each field from the line on the delimBuffer
	@pre delimBuffer must not be empty
	@post Makes parameter string equal to correct field in record
	*/
	bool unpack(string & field);

	/**
	@brief reads from csv file and places on string
	@post returns the string of one line of us_postal_codes.csv 
	*/
	bool read(ifstream& inFile);

	/**
	@brief Gives the delimBuffer string  
	@post Returns the delimBuffer string  
	*/
	void setBuffer(string x) { 
		buffer = x; };

	string getBuffer() { 
		return buffer; };

};
#endif