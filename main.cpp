/**
Main program 
@brief reads zip codes in from a csv file, then prints a table organized by state with most east, west, north, and south zips
@author Jake Haapoja, Ken Stearns, Nathan O'Connor, Zach Sawicki
*/

/**
Header Record Architecture:
file structure type {blocked sequence set with comma separated fields, and length-indicated records}
version of your file structure type (we may add features in subsequent versions)	--> 1.0
header record size  --> 512
number of bytes for each record size integer (if fixed-size) --> 2
size format type {ASCII or binary}	--> ASCII
block size {default to (512 Bytes / block)} -->512
minimum block capacity {default to 50%}, except for, possibly, the last block --> 50
index file name
index file schema information {how to read the index file format}
record count
block count
count of fields per record
for each field:
	-name or ID
	-type schema (format to read or write)
indicate (ordinally) which field serves as the primary key
RBN link to the block avail-list {RBN ↔ Relative Block Number}
RBN link to the active sequence set list
stale flag
*/

#include "PrimaryIndex.h"
#include "delimBuffer.h"
#include "LengthBuffer.h"
#include "zipCode.h"
#include "BFile.h" 
#include "Buffer_Record.h"
#include <vector>
#include <iostream>
using namespace std;


const string manual =
"sample input: programname -r filename.csv\noptions: \n-r <filename.csv>\n-z <zip code> \nprogram must be run once with a csv file to generate the datafile and index";

void addRecord(BFile& b);
void delRecord(BFile& b, string arg);

/*
* @brief main function that takes command line arguments
* @param argument count, string of arguments
*/

int main(int argc, char* argv[]) {
	
	fstream iFile, dFile;
	ifstream inFile;	// filestream objects
	string filename;
	int queryZip;
	unsigned long offset = 0;
	LengthBuffer indicated;

	for (int i = 0; i < argc; i++) {
		cout << argv[i] << ' ';
	}

	/*if (argc != 3) {
		cout << "Invalid Input" << endl << manual;
		return -1;
	}*/
	string arg2, arg1 = argv[1];
	if (argc == 3)
		arg2 = argv[2];

	if (arg1 == "-pd") { // phyisical order dump

		blockFile b;
		cout << b.pdump();
	}
	else if (arg1 == "-ld") { // logical order dump


		blockFile b;
		cout << b.ldump();
	
	}
	else if(arg1 == "-b"){

		blockFile b;


	}
	else if(arg1 == "-a"){ //add a record

		blockFile b;
		addRecord(b);

	}
	else if(arg1 == "-d"){ //delete a record

		blockFile b;
		delRecord(b, arg2);

	}
	else if(arg1 == "-r") {	// translate delim file to LI, offers to search

		char response;
		int zipResponse;

		filename = argv[2];
		inFile.open(filename); // access the data file and associate to filestream object

		PrimaryIndex indexList(inFile);

		iFile.open("IndexFile.index");
		dFile.open("DataFile.licsv");

		cout << "file imported successfully\n";
		cout << "do you want to search the database? (Y/N): ";
		cin >> response;
		if (tolower(response) == 'y') {
			cout << "\nPlease enter a valid zip: ";
			cin >> zipResponse;

			offset = indexList.search(zipResponse);
			if (offset == 0) { cout << "Sorry! We can't seem to find that one."; return -2; }
			indicated.read(dFile, offset);
			for (int i = 0; i < 6; i++) {
				string temp = "";
				switch(i) {
				case 0:
					cout << "\nZip Code: ";
					indicated.unpack(temp);
					cout << temp;
					break;
				case 1: 
					cout << " Place Name: ";
					indicated.unpack(temp);
					cout << temp;
					break;
				case 2:
					cout << " State: ";
					indicated.unpack(temp);
					cout << temp;
					break;
				case 3:
					cout << " County: ";
					indicated.unpack(temp);
					cout << temp;
					break;
				case 4:
					cout << " Lat: ";
					indicated.unpack(temp);
					cout << temp;
					break;
				default:
					cout << " Long: ";
					indicated.unpack(temp);
					cout << temp;
					break;
				}
			}
		}

		return 1;
	}
	else if (argc == 3 && arg1 == "-z") {	// search for zip
		
		PrimaryIndex indexList("IndexFile.index", "DataFile.licsv");
		queryZip = stoi(arg2);
		offset = indexList.search(queryZip);
		if (offset == 0) { cout << "Sorry! We can't seem to find that one.\n"; return -2; }

		dFile.open("DataFile.licsv");
		indicated.read(dFile, offset);
		dFile.close();

		for (int i = 0; i < 6; i++) {
			string temp = "";
			switch(i) {
			case 0:
				cout << "Zip Code: ";
				indicated.unpack(temp);
				cout << temp;
				break;
			case 1: 
				cout << " Place Name: ";
				indicated.unpack(temp);
				cout << temp;
				break;
			case 2:
				cout << " State: ";
				indicated.unpack(temp);
				cout << temp;
				break;
			case 3:
				cout << " County: ";
				indicated.unpack(temp);
				cout << temp;
				break;
			case 4:
				cout << " Lat: ";
				indicated.unpack(temp);
				cout << temp;
				break;
			default:
				cout << " Long: ";
				indicated.unpack(temp);
				cout << temp;
				break;
			}
		}
	}

	else {	// invalid arguments 

		cout << "invalid arguments";
		return -1;
	}
	

	cout << endl << "Hello World!" << endl;

	dFile.close();  // definitely nothing to see here
	iFile.close();
	inFile.close();

	return 1;
}

void addRecord(blockFile& b){

	ZipCode z;
	string temporary;


	cout << "Zip Code: ";
	cin >> temporary;
	z.setNum(stoi(temporary));

	cout << "City: ";
	cin >> temporary;
	z.setCity(temporary);

	cout << "State Code: ";
	cin >> temporary;
	z.setStateCode(temporary);

	cout << "County: ";
	cin >> temporary;
	z.setCounty(temporary);

	cout << "Latitude: ";
	cin >> temporary;
	z.setLat(stoi(temporary));

	cout << "Longitude: ";
	cin >> temporary;
	z.setLon(stoi(temporary));

	if (b.addRecord(z))
		cout << "Record added successfully\n";
	else
		cout << "Failed to add record\n";

}

void delRecord(blockFile& b, string arg){


	if (b.delRecord(arg))
		cout << "Record deleted successfully\n";
	else
		cout << "Failed to delete record\n";

}
