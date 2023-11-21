/**
 * PrimaryIndex.cpp
 * Member functions for the PrimaryIndex class.
 */

#include "PrimaryIndex.h"

using namespace std;

static const short NumStates = 57; // Number of possible states/regions

void PrimaryIndex::getIndex(vector<IndexElement>& returnValue) {
    IndexElement temp;

    int i = 0;
while (i < index.size()) {
    temp.zip = index[i].zip;
    temp.offset = index[i].offset;
    returnValue.push_back(temp);
    i++;
}

}

void PrimaryIndex::add(int zipCode, unsigned long offset) {
    IndexElement temp = {zipCode, offset};
    int i = 0;

    if (recordCount == 0) {
        index.push_back(temp);
    } else {
        while (i < recordCount && index[i].zip < zipCode) {
            i++;
        }
        if (i == recordCount) {
            index.push_back(temp);
        } else if (i == 0) {
            index.insert(index.begin(), temp);
        } else {
            vector<IndexElement>::iterator it;
            it = index.begin();
            index.insert(it + i, temp);
        }
    }

    recordCount++;
}

unsigned long PrimaryIndex::search(int targetZipCode) {
    int offset = binarySearch(targetZipCode, 0, recordCount - 1);
    return offset;
}

unsigned long PrimaryIndex::binarySearch(int target, int left, int right) {
    if (left > right) {
        return 0;
    }

    if (right >= left) {
        int mid = (left + right) / 2;

        if (index[mid].zip == target) {
            cout << "Target hit!: " << index[mid].zip << ' ' << index[mid].offset << endl;
            return index[mid].offset;
        } else if (index[mid].zip > target) {
            binarySearch(target, left, mid - 1);
        } else {
            binarySearch(target, mid + 1, right);
        }
    }
}

void PrimaryIndex::readIndex() {
    if (!indexFile.eof()) {
        int itemp;
        indexFile >> itemp;
        cout << itemp << " records in the file." << endl;

        int zip;
        unsigned long int offset;
        char temp;

        while (!indexFile.eof()) {
            indexFile >> zip >> temp >> offset;
            add(zip, offset);
        }
    }
}

void PrimaryIndex::writeToFile() {
    ofstream outFile;
    outFile.open("IndexFile.index");

    outFile << recordCount << "\n";

    for (int i = 0; i < recordCount; i++) {
        outFile << index[i].zip << "," << index[i].offset << "\n";
    }
}

void PrimaryIndex::readCSV(ifstream& infile) {
    indexFile.open("IndexFile.index");
    dataFile.open("DataFile.licsv");

    vector<vector<ZipCode>> states;
    states.resize(NumStates);
    string headerData = readIn(infile, states);
    cout << endl << printTable(states) << endl;

    transfer(states, headerData);

    writeToFile();
}

string PrimaryIndex::buildHeader(string headerData) {
    string record;
    int count = 1, temp;

    record.append("Structure Type: Length Indicated Comma Separated File\n");
    record.append("Version: 1.0\n");

    // Determine size of record
    record.append("Record Size: ");
    record.append(to_string(headerData.size()));
    record.push_back('\n');

    // Size format type
    record.append("Size Format: 2-digit ASCII\n");

    // Index File Name
    record.append("Index File: IndexFile.index\n");

    // Index File Schema Information
    record.append("Index File Schema: Listed by zip code then corresponding offset, sorted by zip code\n");

    // Record Count
    record.append("Record Count: 40933\n");

    // Count of fields per record 
    for (int j = 0; j < headerData.size(); j++) {
        if (headerData[j] == ',') {
            count++;
        }
    }

    record.append("Fields per Record: ");
    record.append(to_string(count));
    record.push_back('\n');

    // Name of each field
    record.append("Name of Fields: ");
    record.append(headerData);
    record.push_back('\n');

    // Type Schema
    record.append("Type Schema: Zip Code is an integer. Lon and Lat are floating point decimals. County, State Code, and City are strings.\n");

    // Indicate which field is primary key
    record.append("First Key: Zip Code\n");
    return record;
}

void PrimaryIndex::transfer(vector<vector<ZipCode>>& states, string headerData) {
    if (!dataFile.is_open()) {
        dataFile.open("DataFile.licsv");
    }

    string header = buildHeader(headerData);
    dataFile << header;

    string temp;
    LengthBuffer buf;
    unsigned long count = 0;
    unsigned long offsetSum = header.size() + 11;

    for (int i = 0; i < NumStates; i++) {
        for (int j = 0; j < states[i].size(); j++) {
            count = 0;

            temp = to_string(states[i][j].getNum());
            temp.push_back(',');
            temp.append(states[i][j].getCity());
            temp.push_back(',');
            temp.append(states[i][j].getStateCode());
            temp.push_back(',');
            temp.append(states[i][j].getCounty());
            temp.push_back(',');
            temp.append(to_string(states[i][j].getLat()));
            temp.push_back(',');
            temp.append(to_string(states[i][j].getLon()));

            count = temp.size();
            
            buf.pack(temp);
            buf.write(dataFile);

            add(states[i][j].getNum(), offsetSum);
            offsetSum += count + 2;
        }
    }
}

string PrimaryIndex::readIn(ifstream& inFile, vector<vector<ZipCode>>& states) {
    ZipCode temp;
    string item;
    string headerData;
    vector<string> headerElement;
    delimBuffer b;

    for (int i = 0; i < 3; ++i) {
        b.read(inFile);
        headerData.append(b.getBuffer());
    }

    for (int i = 0; i < headerData.size(); i++) {
        headerData[i] = tolower(headerData[i]);
    }
    for (int i = 0, j = 0, k = 0; i < headerData.size(); ++i, k = 0) {
        string temps = "";
        while (headerData[i] != ',' && i < headerData.size()) {
            if (isalpha(headerData[i])) {
                temps.push_back(headerData[i]);
            }
            i++;
        }

        headerElement.resize(headerElement.size() + 1);
        headerElement[j++] = temps;
    }

    int tag = 0;

    while (b.read(inFile)) {
        while (b.unpack(item)) {
            if (tag > 5) tag = 0;

            if (headerElement[tag] == "zipcode") {
                temp.setNum(stoi(item));
            } else if (headerElement[tag] == "placename") {
                temp.setCity(item);
            } else if (headerElement[tag] == "state") {
                temp.setStateCode(item);
            } else if (headerElement[tag] == "county") {
                temp.setCounty(item);
            } else if (headerElement[tag] == "lat") {
                temp.setLat(stof(item));
            } else if (headerElement[tag] == "long") {
                temp.setLon(stof(item));
            }
            ++tag;
            item = "";
        }
        states[stateSelector(temp.getStateCode())].push_back(ZipCode(temp));
    }

    return headerData;
}

string PrimaryIndex::printTable(vector<vector<ZipCode>>& states) {
    string output;

    output.append("*****************************************************\n");
    output.append("*State\t|East\t\t|West\t\t|North\t\t|South\t*\n");
    output.append("*****************************************************\n");

    for (int i = 0; i < NumStates; i++) {
        output.append("*");
        output.append(states[i][0].getStateCode());
        output.append("\t|");
        output.append(to_string(states[i][eastest(states[i])].getNum()));
        output.append("\t\t|");
        output.append(to_string(states[i][westest(states[i])].getNum()));
        output.append("\t\t|");
        output.append(to_string(states[i][northest(states[i])].getNum()));
        output.append("\t\t|");
        output.append(to_string(states[i][southest(states[i])].getNum()));
        output.append("\t*\n");
    }
    output.append("*****************************************************\n");
    output.append("*State\t|East\t\t|West\t\t|North\t\t|South\t*\n");
    output.append("*****************************************************");

    return output;
}

short PrimaryIndex::northest(vector<ZipCode> state) {
    short x = 0;
    for (int i = 1; i < state.size(); i++) {
        if (state[i].getLat() > state[i - 1].getLat()) {
            x = i;
        }
    }
    return x;
}

short PrimaryIndex::southest(vector<ZipCode> state) {
    short x = 0;
    for (int i = 1; i < state.size(); i++) {
        if (state[i].getLat() < state[i - 1].getLat()) {
            x = i;
        }
    }
    return x;
}

short PrimaryIndex::eastest(vector<ZipCode> state) {
    short x = 0;
    for (int i = 1; i < state.size(); i++) {
        if (state[i].getLon() < state[i - 1].getLon()) {
            x = i;
        }
    }
    return x;
}

short PrimaryIndex::westest(vector<ZipCode> state) {
    short x = 0;
    for (int i = 1; i < state.size(); i++) {
        if (state[i].getLon() > state[i - 1].getLon())
			x = i;
	}
	return x;
}

/**
* @brief Chooses which state array index is correct
* with the use of a switch statement
* @pre two character state code in a string is used as parameter
* @post Returns the correct array index as an int
*/
short PrimaryIndex::stateSelector(const std::string& stateCode) {
    static const std::unordered_map<std::string, short> stateMap = {
        {"AA", 0}, {"AK", 1}, {"AL", 2}, {"AP", 3}, {"AR", 4},
        {"AZ", 5}, {"CA", 6}, {"CO", 7}, {"CT", 8}, {"DC", 9},
        {"DE", 10}, {"FL", 11}, {"FM", 12}, {"GA", 13}, {"HI", 14},
        {"IA", 15}, {"ID", 16}, {"IL", 17}, {"IN", 18}, {"KS", 19},
        {"KY", 20}, {"LA", 21}, {"MA", 22}, {"MD", 23}, {"ME", 24},
        {"MH", 25}, {"MI", 26}, {"MN", 27}, {"MO", 28}, {"MP", 29},
        {"MS", 30}, {"MT", 31}, {"NC", 32}, {"ND", 33}, {"NE", 34},
        {"NH", 35}, {"NJ", 36}, {"NM", 37}, {"NV", 38}, {"NY", 39},
        {"OH", 40}, {"OK", 41}, {"OR", 42}, {"PA", 43}, {"PW", 44},
        {"RI", 45}, {"SC", 46}, {"SD", 47}, {"TN", 48}, {"TX", 49},
        {"UT", 50}, {"VA", 51}, {"VT", 52}, {"WA", 53}, {"WI", 54},
        {"WV", 55}, {"WY", 56}
    };

    auto it = stateMap.find(stateCode);
    if (it != stateMap.end()) {
        return it->second;
    }
    else {
        return -1;
    }
}