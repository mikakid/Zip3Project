/**
 * @file ZipCode.cpp
 * @brief Implementation of member functions for the ZipCode class.
 * @author Group7
 */

#include "zipCode.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Default Constructor
ZipCode::ZipCode() {
    // @brief Default Constructor
    // @post Initializes a ZipCode object with default (empty or zero) values.
    num = -1;
    city = "";
    stateCode = "";
    county = "";
    lat = 0;
    lon = 0;
}

// Parameterized Constructor
ZipCode::ZipCode(int newNum, string newCity, string newStateCode, string newCounty, float newLat, float newLon) {
    // @brief Parameterized Constructor
    // @pre Accepts individual parameters for each member variable.
    // @post Initializes a ZipCode object with provided values.
    num = newNum;
    city = newCity;
    stateCode = newStateCode;
    county = newCounty;
    lat = newLat;
    lon = newLon;
}

// Copy Constructor
ZipCode::ZipCode(const ZipCode& oldZip) {
    // @brief Copy Constructor
    // @pre Accepts an existing ZipCode object.
    // @post Creates a new ZipCode object as a copy of the existing one.
    num = oldZip.num;
    city = oldZip.city;
    stateCode = oldZip.stateCode;
    county = oldZip.county;
    lat = oldZip.lat;
    lon = oldZip.lon;
}

// Function to get the size of the ZipCode data
const int ZipCode::getSize() {
    // @brief Gets the size of the ZipCode data.
    // @return The size of the ZipCode data as an integer.
    string size = "";
    string temp = "";

    temp.append(to_string(num));
    size.push_back(',');
    temp.append(city);
    size.push_back(',');
    temp.append(stateCode);
    size.push_back(',');
    temp.append(county);
    size.push_back(',');
    temp.append(to_string(lat));
    size.push_back(',');
    temp.append(to_string(lon));

    size.append(to_string(temp.size()));
    size.push_back(',');
    size.append(temp);

    return size.size();
}

// Static method to read zip codes from a file
vector<ZipCode> ZipCode::readFromFile(const string& filename) {
    // @brief Reads zip codes from a file and creates ZipCode objects.
    // @pre Expects a filename as a string.
    // @return A vector of ZipCode objects.
    vector<ZipCode> zipCodes;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        int num;
        string city, stateCode, county;
        float lat, lon;

        ss >> num;
        getline(ss, city, ',');
        getline(ss, stateCode, ',');
        getline(ss, county, ',');
        ss >> lat >> lon;

        ZipCode zip(num, city, stateCode, county, lat, lon);
        zipCodes.push_back(zip);
    }

    return zipCodes;
}
