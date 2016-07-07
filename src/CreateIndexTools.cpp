//
// Created by jasf on 6/25/16.
//

#include "CreateIndexTools.h"
#include <set>
#include <assert.h>

/**
 * returns list of <primary ,key address, name> of record in the order they appear on the file
 *
 * This was created to be used with list1.txt file.
 *
 * This consider fields appear in this order:
 * primary key, number, name, number, letter, 2 letters
 *
 * The key address is the number of the line inside the record file in which you find
 * the record correspoding to the given primary key
 */

vector<tuple<string, int, string>> readFixedLengthFieldFile(string filename) {
    ifstream inputFile;
    stringstream stringStream;

    inputFile.open(filename);


    vector<tuple<string, int, string>> out;
    if (!inputFile.is_open()) {
        cerr << "Cannot open input file: " << filename << endl;
        return out;

    }

    string buffer;
    string primaryKey, name;
    char nameBuffer[30];
    int numberIdontCare;
    int currentRecordIdx = 0;



    //read first file assuming it has fixed length fields
    while (getline(inputFile, buffer)) {
        stringStream.str(buffer);

        // only the first field(the primary key) matters for us right now
        stringStream >> primaryKey >> numberIdontCare;
        stringStream.get();

        stringStream.read(nameBuffer, 23);

        name = string(nameBuffer, nameBuffer + 23);

        out.push_back(make_tuple(primaryKey.c_str(), currentRecordIdx, name.c_str()));

        //get byte adderss of the next recrod
        currentRecordIdx++;


    }

    inputFile.close();

    cout << "Index created for fixed length fields input data file:" << filename << endl;

    return out;

}

/**
 * returns primary key -> address of record in the order they appear on the file
 *
 * The address is the index of the line inside the records file where you'll find the record for the given
 * primery key
 *
 * Assumes one record per line, a '|' between fields of the same record
 */
vector<pair<string, int>> readVariableLengthFieldFile(string filename) {
    ifstream inputFile;
    stringstream stringStream;

    inputFile.open(filename);

    vector<pair<string, int>> out;
    if (!inputFile.is_open()) {
        cerr << "Cannot open input file: " << filename << endl;
        return out;

    }

    string buffer;
    string primaryKey;
    int currentRecordIdx = 0;

    while (getline(inputFile, buffer)) {
        stringStream.str(buffer);

        getline(stringStream, primaryKey, '|');

        out.push_back(make_pair(primaryKey, currentRecordIdx));

        currentRecordIdx++;

    }

    inputFile.close();

    cout << "Index created for variable length fields input data file:" << filename << endl;

    return out;

}

vector<pair<string, int>> sortIndex(vector<pair<string, int>> unsortedEntries) {
    vector<pair<string, int>> out;
    vector<string> primaryKeyVector;
    map<string, int> keyAddrMap;

    for (auto entry : unsortedEntries) {
        primaryKeyVector.push_back(entry.first);
        keyAddrMap[entry.first] = entry.second;
    }

    sort(primaryKeyVector.begin(), primaryKeyVector.end());

    for (auto key : primaryKeyVector)
        out.push_back(make_pair(key, keyAddrMap[key]));

    return out;

}


/**
 * returns two things:
 * a) a vector of tuple<string, int, int> sorted according to the fist string (supposed the primary key)
 * the first int is the address of the byte where to locate the record inside lista1.txt. The second is the index of
 * the line inside the primary index where you find a user of the same name of this user. -1 if there is no next user.
 * b) a map of 'name' -> index of the line in the primary index file for lista1.txt where you locate a reference to the
 * first user with named 'name'
 */
pair<PrimaryIndex, SecondaryIndex> createPrimaryAndSecondaryIndex(
        vector<RelevantForIndexConstructionUserData> unsortedEntries) {
    vector<tuple<string, int, int>> primaryIndexData;
    map<string, int> secondaryIndexData;

    //vector with all primary keys
    vector<string> primaryKeyVector;
    // map of primary key to the address where we locate the record inside the data file
    map<string, int> keyAddrMap;
    // map of primary key to name
    map<string, string> keyNameMap;
    // set of all unique names
    set<string> uniqueNames;
    //map of unique name to list of primary keys with that name
    map<string, vector<string>> nameKeysMap;

    //fill in the above maps
    for (auto entry : unsortedEntries) {
        string primaryKey = get<0>(entry);
        int addrOnDataFile = get<1>(entry);
        string name = get<2>(entry);

        primaryKeyVector.push_back(primaryKey);
        keyAddrMap[primaryKey] = addrOnDataFile;
        keyNameMap[primaryKey] = name;

        if (nameKeysMap.find(name) == nameKeysMap.end()) {
            nameKeysMap[name] = vector<string>();
        }

        nameKeysMap[name].push_back(primaryKey);
        uniqueNames.insert(name);
    }

    sort(primaryKeyVector.begin(), primaryKeyVector.end());

    // maps the primary key to its line in the primary index
    // the line of the entry is given by the order of the primary keys (from here on they are sorted)
    map<string, int> primaryKeyEntryIdxOnPrimaryIdxMap;
    for (int i = 0; i < primaryKeyVector.size(); i++) {
        primaryKeyEntryIdxOnPrimaryIdxMap[primaryKeyVector[i]] = i;
    }

    // creates a map of name -> list of references where to find the entry in the primary index of the next user with that name
    // the i-th position in of these lists is corresponding the the i-th position inside nameKeysMap
    map<string, vector<int>> namePrimaryIndexIdxNext;
    map<string, vector<int>> namePrimaryIndexIdx;
    for (auto namePrimaryKey : nameKeysMap) {
        string name = namePrimaryKey.first;
        const vector<string> &keysWithThisName = namePrimaryKey.second;

        namePrimaryIndexIdxNext[name] = vector<int>();
        namePrimaryIndexIdx[name] = vector<int>();

        //we make the i-th student with the given name to point to the i-1-th student with that name
        for (int i = 0; i < keysWithThisName.size(); i++) {
            namePrimaryIndexIdx[name].push_back(primaryKeyEntryIdxOnPrimaryIdxMap[keysWithThisName[i]]);
        }

        //the first item we read is made the last of the queue
        namePrimaryIndexIdxNext[name].push_back(-1);
        for (int i = 1; i < keysWithThisName.size(); i++) {
            namePrimaryIndexIdxNext[name].push_back(namePrimaryIndexIdx[name][i - 1]);
        }


    }
    // aggregate the secondary index data
    for (string name : uniqueNames) {
        secondaryIndexData[name] = *(namePrimaryIndexIdx[name].end() - 1);
    }

    // creates a map primary key -> index of the next user with the same name as the user with this primary key
    // again, the index is the number of the line inside the primary index where you'll find this other user with same name
    // -1 is places if this is the last user with its name

    map<string, int> primaryKeyNextWithSameNameMap;

    for (string name : uniqueNames) {
        // retrieve list of primery keys with the given name
        const vector<string> &usersWithThisName = nameKeysMap[name];
        vector<int> &nextOnTheList = namePrimaryIndexIdxNext[name];

        if (nextOnTheList.size() == 1)
            nextOnTheList[0] = -1;

        assert(usersWithThisName.size() == nextOnTheList.size());

        for (int i = 0; i < usersWithThisName.size(); i++) {
            string primaryKey = usersWithThisName[i];
            int next = nextOnTheList[i];

            if (primaryKeyNextWithSameNameMap.find(primaryKey) != primaryKeyNextWithSameNameMap.end()) {
                cerr << "We've already defined a name for this user! ERROR!" << endl;
            }

            primaryKeyNextWithSameNameMap[primaryKey] = next;
        }

    }



    //aggregate the primary index data from all we've done so far
    for (auto key : primaryKeyVector)
        primaryIndexData.push_back(make_tuple(key, keyAddrMap[key], primaryKeyNextWithSameNameMap[key]));

    return make_pair(primaryIndexData, secondaryIndexData);

}

pair<PrimaryIndex, SecondaryIndex> buildPrimaryAndSecondaryIndexForFixedLengthFieldsFile(string filename) {

    auto unsortedRelevantData = readFixedLengthFieldFile(filename);
    return createPrimaryAndSecondaryIndex(unsortedRelevantData);
}

vector<pair<string, int>> buildPrimaryIndexForVariableLengthFieldsFile(string filename) {
    auto unsortedEntries = readVariableLengthFieldFile(filename);
    return sortIndex(unsortedEntries);
}

void writeIndexToFile(vector<pair<string, int>> index, string fileName, bool printQtdRemoved, int qtdRemoved) {

    ofstream f(fileName);

    if (!f.is_open()) {
        cerr << "Cannot open file:" << fileName << " for writting index!" << endl;
        return;
    }

    if (printQtdRemoved) {
        f << setfill('0') << setw(3) << qtdRemoved << endl;
    }

    for (auto entry : index) {
        f << entry.first << "\t" << entry.second << endl;
    }

    f.close();

    cout << "simple primary index written to:" << fileName << endl;

}

void writeIndexToFile(PrimaryIndex index, string fileName, bool printQtdRemoved, int qtdRemoved) {

    ofstream f(fileName);

    if (!f.is_open()) {
        cerr << "Cannot open file:" << fileName << " for writting index!" << endl;
        return;
    }

    if (printQtdRemoved) {
        f << setfill('0') << setw(3) << qtdRemoved << endl;
    }

    for (auto entry : index) {
        f << get<0>(entry) << "\t" << get<1>(entry) << "\t" << get<2>(entry) << endl;
    }

    f.close();

    cout << "primary index with linked list for names written to:" << fileName << endl;

}

void writeSecondaryIndexToFile(SecondaryIndex index, string fileName) {

    ofstream f(fileName, ios::out | ios::trunc);

    if (!f.is_open()) {
        cerr << "Cannot open file:" << fileName << " for writting index!" << endl;
        return;
    }

    for (auto entry : index) {
        f << get<0>(entry) << "\t" << get<1>(entry) << endl;
    }

    f.close();

    cout << "secondary index written to:" << fileName << endl;

}

string getPrimaryIndexNameForInputFile(string filename) {
    return "index_" + filename;
}

void createSortedIndexesForFiles(string file0, string file1, string file2) {
    auto indexForFirstFile = buildPrimaryAndSecondaryIndexForFixedLengthFieldsFile(file0);
    writeIndexToFile(indexForFirstFile.first, getPrimaryIndexNameForInputFile(file0), true, 0);

    writeSecondaryIndexToFile(indexForFirstFile.second, getSecondaryIndexNameForInputFile(file0));

    auto indexForSecondFile = buildPrimaryIndexForVariableLengthFieldsFile(file1);
    writeIndexToFile(indexForSecondFile, getPrimaryIndexNameForInputFile(file1));

    auto indexForThirdFile = buildPrimaryIndexForVariableLengthFieldsFile(file2);
    writeIndexToFile(indexForThirdFile, getPrimaryIndexNameForInputFile(file2));

}

string getSecondaryIndexNameForInputFile(string filename) {
    return "sec_" + filename;
}

map<string, int> loadIndex(string file) {
    map<string, int> out;
    ifstream is(file);
    string lineBuffer;
    stringstream lineStream;
    string key;
    int addressOnDataFile;


    if (!is.is_open()) {
        cerr << "Could not open file:" << file << endl;
        return out;
    }

    while (getline(is, lineBuffer)) {
        lineStream.str(lineBuffer);
        lineStream >> key >> addressOnDataFile;

        if (out.find(key) != out.end()) {
            cerr << "Error on index! Multiple definition of same key!" << endl;
        }

        out.insert(make_pair(key, addressOnDataFile));
    }

    return out;

}

StudentRecordPrimaryIndex loadStudentRecordPrimaryIndex(fstream &is, int *qtdDeleted) {
    StudentRecordPrimaryIndex out;
    string lineBuffer;
    stringstream lineStream;
    string primaryKey;
    int lineAddrOnDataFile, lineAddrOfNext;
    int qtdDel;


    if (!is.is_open()) {
        cerr << "Could not open file:" << is << " to load primary student records index from" << endl;
        return out;
    }
    if (!is.good()) {
        is.clear();
        is.seekg(0);
    }


    getline(is, lineBuffer);
    lineStream.str(lineBuffer);
    lineStream >> qtdDel;
    *qtdDeleted = qtdDel;

    char primaryKeyBuffer[10];

    while (getline(is, lineBuffer)) {
        sscanf(lineBuffer.c_str(), "%s\t%d\t%d", primaryKeyBuffer, &lineAddrOnDataFile, &lineAddrOfNext);
        primaryKey = string(primaryKeyBuffer);

        if (out.find(primaryKey) != out.end()) {
            cerr << "Error on index! Multiple definition of same key:" << primaryKey << "!" << endl;
            cerr << flush;
        }

        out.insert(make_pair(primaryKey, make_tuple(lineAddrOnDataFile, lineAddrOfNext)));
    }

    return out;
}

StudentRecordSecondaryIndex loadStudentRecordSecondaryIndex(fstream &is) {
    map<string, int> out;
    string lineBuffer;
    stringstream lineStream;
    string key;
    int addressOnDataFile;


    if (!is.is_open()) {
        cerr << "Could not open file:" << is << " to load secondary name index from" << endl;
        return out;
    }
    if (!is.good()) {
        is.clear();
        is.seekg(0);
    }

    char nameBuffer[200];
    while (getline(is, lineBuffer)) {
        sscanf(lineBuffer.c_str(), "%23c\t%d", nameBuffer, &addressOnDataFile);
        key = string(nameBuffer);

        if (out.find(key) != out.end()) {
            cerr << "Error on index! Multiple definition of same key:" << key << " while loading secondary index!" <<
            endl;
        }

        out.insert(make_pair(key, addressOnDataFile));
    }

    return out;
}

ClassGradesIndex loadClassGrades(ifstream &is) {
    map<string, int> out;
    string lineBuffer;
    stringstream lineStream;
    string key;
    int addressOnDataFile;


    if (!is.is_open()) {
        cerr << "Could not open file:" << is << " to load class grades index from" << endl;
        return out;
    }
    if (!is.good()) {
        is.clear();
        is.seekg(0);
    }

    while (getline(is, lineBuffer)) {
        lineStream.str(lineBuffer);
        lineStream >> key >> addressOnDataFile;

        if (out.find(key) != out.end()) {
            cerr << "Error on index! Multiple definition of same key!" << endl;
        }

        out.insert(make_pair(key, addressOnDataFile));
    }

    return out;
}
