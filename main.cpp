/**
 * @file main_proj3.cpp
 * @brief Main file for the postal code database management system.
 * 
 * This file contains the main function and helper functions for managing 
 * a postal code database. It handles CSV file processing, record addition and deletion, 
 * database searching, and displaying specific records. It uses various classes like 
 * PrimaryIndex, LengthBuffer, ZipCode, BFile, Buffer_Record, and CSVReader to perform its operations.
 * 
 * @author Group 7
 */

//imports

#include "PrimaryIndex.h"
#include "delimBuffer.h"
#include "LengthBuffer.h"
#include "ZipCode.h"
#include "BFile.h"
#include "Buffer_Record.h"
#include "CSVReader.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Declarations for helper functions
void analyzeCSV(CSVReader &csvReader);
void addRecord(BFile& bf);
void delRecord(BFile& bf, const string& arg);
void handleFileImport(const string& filename);
void searchDatabase(const PrimaryIndex& indexList);
void displayRecordFromOffset(ifstream& FS, unsigned long offset);

/**
 * @brief Main function to process user commands and manage the postal code database.
 * 
 * Processes command-line arguments for different operations such as 
 * physical and logical data dump, record addition, deletion, file importing, and database searching.
 * 
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Exit status of the program.
 */
// main to process user commands and manage the postal code database.
int main(int argc, char* argv[]) {
    // Process CSV file at the beginning
    std::string csvFile = "us_postal_codes.csv";
    std::cout << "Processing us_postal_codes.csv.\n" << std::endl;
    CSVReader csvReader(csvFile);
    analyzeCSV(csvReader);

    // Process command-line arguments
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " -option [additional arguments]" << endl;
        return 1;
    }
    
    string option = argv[1];
    bFile bf;

    if (option == "-pd") {
        cout << bf.physicalDump();  // Updated method name
    } else if (option == "-ld") {
        cout << bf.logicalDump();  // Updated method name
    } else if (option == "-a") {
        addRecord(bf);  // Updated function call
    } else if (option == "-d" && argc == 3) {
        delRecord(bf, argv[2]);  // Updated function call
    } else if (option == "-r" && argc == 3) {
        handleFileImport(argv[2]);  // Unchanged
    } else if (option == "-z" && argc == 3) {
        PrimaryIndex indexList("primIndex.index", "data.txt);
        ifstream FS("data.txt");
        unsigned long offset = indexList.search(stoi(argv[2]));
        displayRecordFromOffset(FS, offset);
    } else {
        cout << "Invalid arguments" << endl;
        return -1;
    }

    return 0;
}
/**
 * @brief Adds a new record to the  database.
 * 
 * Prompts the user to enter details for a new postal code record and adds it to the database.
 * 
 * @param bf Reference to BFile object representing the database file.
 */
//add record 
void addRecord(BFile& b) {
    ZipCode address;  // Correct class name
    string temporary;
    float lat, lon;
    int zip;

    cout << "Zip Code: ";
    cin >> temporary;
    zip = stoi(temporary);  // Convert string to int
    address.setNum(zip);

    cout << "City: ";
    cin >> temporary;
    address.setCity(temporary);

    cout << "State Code: ";
    cin >> temporary;
    address.setStateCode(temporary);

    cout << "County: ";
    cin >> temporary;
    address.setCounty(temporary);

    cout << "Latitude: ";
    cin >> lat;  // Directly read as float
    address.setLat(lat);

    cout << "Longitude: ";
    cin >> lon;  // Directly read as float
    address.setLon(lon);

    if (b.addRecord(address))  // Correct object name
        cout << "Record added\n";
    else
        cout << "Record not added\n";
}

/**
 * @brief Deletes a record from the database based on the provided address
 * 
 * @param bf Reference to BFile object representing the database file.
 * @param arg String representing the zip code of the record to be deleted.
 */

void delRecord(blockFile& b, const string& arg) {
    if (b.delRecord(arg))
		cout << "Record deleted \n";
	else
		cout << "Failed to delete \n";
}

/**
 * @brief Handles importing of a file into the postal code database.
 * 
 * @param filename String representing the name of the file to be imported.
 */
void handleFileImport(const string& filename) {
    ifstream inFile(filename);
    primaryIndex indexList(inFile);
    cout << "File imported successfully" << endl;
    cout << "Do you want to search the database? (Y/N): ";
    char response;
    cin >> response;
    if (tolower(response) == 'y') {
        searchDatabase(indexList);
    }
}

/**
 * @brief Searches the database for a specific address and displays the record.
 * 
 * @param indexList Reference to PrimaryIndex object for searching the database.
 */
//Searches the database for a specific address and displays
void searchDatabase(const PrimaryIndex& indexList) {
    int valid_zip;
    cout << "Please enter a valid zip: ";
    cin >> valid_zip;
    unsigned long offset = indexList.search(valid_zip;);
    if (offset == 0) {
        cout << "cant find zip" << endl;
        return;
    }
    ifstream dFile("data.txt");
    displayRecordFromOffset(FS, offset);
}

/**
 * @brief Displays a record from the database at a specific offset.
 * 
 * @param FS Reference to ifstream object to read data from the database file.
 * @param offset Unsigned long representing the offset in the file where the record is located.
 */
//display @offset
void displayRecordFromOffset(ifstream& FS, unsigned long offset) {
    LengthBuffer showing_addr;
    indicated.read(FS, offset);
    for (int i = 0; i < 6; ++i) {
        string temp;
        showing_addr.unpack(temp);
        cout << (i == 0 ? "Zip Code: " : i == 1 ? "Place Name: " : i == 2 ? "State: " : i == 3 ? "County: " : i == 4 ? "Lat: " : "Long: ") << temp << endl;
    }
}
/**
 * @brief Analyzes and displays state statistics from a CSVReader object.
 * @param csvReader The CSVReader object to analyze.
 * @pre The CSVReader object is open and initialized.
 * @post State statistics are displayed for the given CSVReader object.
 */
//Analyzes and displays state statistics from a CSVReader object.
void analyzeCSV(CSVReader &csvReader) {
    //CSVReader csvReader(fileName);
    if (!csvReader.isOpen()) {
        std::cerr << "Failed to open CSV file." << std::endl;
        return;
    }
    // Read and process the CSV file.
    csvReader.ReadFile();


    // Close the CSV file.
    csvReader.close();
}