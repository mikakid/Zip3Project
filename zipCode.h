/**
 * @file ZipCode.h
 * @brief Class representing a zip code along with city name, state code, county, latitude, and longitude for each postal code.
 */

#ifndef ZIP_CODE
#define ZIP_CODE

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class ZipCode {
public:
    // Default constructor
    // @brief Initializes a ZipCode object with empty values.
    ZipCode();

    // Parameterized constructor
    // @brief Initializes a ZipCode object with specific values.
    // @pre Requires zip code, city, state code, county, latitude, and longitude.
    // @post ZipCode object initialized with given values.
    ZipCode(int newNum, string newCity, string newStateCode, string newCounty, float newLat, float newLon);

    // Copy constructor
    // @brief Initializes a ZipCode object as a copy of another ZipCode object.
    ZipCode(const ZipCode& oldZip);

    // Setters and Getters
    // @brief Set and get methods for ZipCode properties.
    void setNum(int newNum);
    const int getNum();
    void setCity(string newCity);
    const string getCity();
    void setStateCode(string newStateCode);
    const string getStateCode();
    void setCounty(string newCounty);
    const string getCounty();
    void setLat(float newLat);
    const float getLat();
    void setLon(float newLon);
    const float getLon();

    // Method to get the size of the ZipCode data
    // @brief Gets the size of the ZipCode data.
    // @return The size of the ZipCode data as an integer.
    const int getSize();

    // Method to print the ZipCode information
    // @brief Prints the details of the ZipCode object.
    void print();

    // Static method for reading zip codes from a file
    // @brief Reads zip codes from a file and returns them as a vector of ZipCode objects.
    // @pre Requires a filename as a string.
    // @return A vector of ZipCode objects.
    static vector<ZipCode> readFromFile(const string& filename);

private:
    int num;
    float lat;
    float lon;
    string stateCode;
    string city;
    string county;
};

#endif // ZIP_CODE
