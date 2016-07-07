//
// Created by jasf on 6/25/16.
//

#include <sstream>
#include <iomanip>
#include "jIndexUtil.h"

string jIndexUtil::getNextKey(const jIndex &index) {
    string lastKey = index.rbegin()->first;
    const char *lastKeyCharArray = lastKey.c_str();
    int lastKeyCount;
    stringstream ss;

    sscanf(lastKeyCharArray, "ID%3d", &lastKeyCount);

    ss << "ID" << setfill('0') << setw(3) << ++lastKeyCount;

    return ss.str();
}

map<int, int> buildLineByteAddrMap(fstream &ifs, int *qtdLines) {
    map<int, int> out;
    string buffer;
    int currentPos = 0;
    int currentLine = 0;
    ifs.seekg(0);

    while (getline(ifs, buffer)) {
        out[currentLine++] = currentPos;

        currentPos = ifs.tellg();

    }

    *qtdLines = currentLine;

    return out;
}

LineMappedFile::LineMappedFile(string filename) {
    this->filename = filename;

    ifs.open(filename);

    if (!ifs.is_open()) {
        cerr << "could not open file:" << filename << endl;
    }

    lineByteAddrMap = buildLineByteAddrMap(ifs, &qtdLines);

}

void LineMappedFile::seekLine(int idx) {
    if(!ifs.good()){
        ifs.clear();
        ifs.seekg(0);
    }
    if (lineByteAddrMap.find(idx) == lineByteAddrMap.end()) {
        cerr << "seeking to invalid line!" << endl;

        ifs.seekg(ios_base::end);

    }
    else {
        ifs.seekg(lineByteAddrMap[idx]);
    }
}

void LineMappedFile::printLineByteMap() {
    string line;
    ifs.seekg(0);
    ifs.clear();
    for (auto it : lineByteAddrMap) {
        cout << "line#" << it.first << " byte:" << it.second << ":";
        ifs.seekg(it.second);
        std::getline(ifs, line);
        cout << line << endl;
    }
}

string LineMappedFile::readLastLine() {
    int lastLineByteAddr = lineByteAddrMap[qtdLines - 1];
    ifs.seekg(lastLineByteAddr);

    string buffer;
    std::getline(ifs, buffer);

    return buffer;
}

string findLastPrimaryKey(LineMappedFile &ifs) {
    string lastLine = ifs.readLastLine();
    string primaryKey;
    int id;

    stringstream ss(lastLine);

    ss >> primaryKey;


    return primaryKey;


}

string getNextPrimaryKey(const string &in) {
    int id;
    sscanf(in.c_str(), "ID%03d", &id);
    char out[5];

    sprintf(out, "ID%03d", (id + 1));

    return string(out);
}

void LineMappedFile::clear() {
    ifs.clear();

}
