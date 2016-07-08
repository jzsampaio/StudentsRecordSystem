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
    construct(filename);

}

void LineMappedFile::seekLine(int idx) {
    if (!ifs.good()) {
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

void LineMappedFile::removeLinesWithThesePrimaryKeys(set<string> deadKeys) {

    string filenameTmp = "tmp" + filename;

    fstream tmpFileStream;

    //erase tmp file
    tmpFileStream.open(filenameTmp, ios_base::out | ios_base::in | ios_base::trunc);
    if (!tmpFileStream.is_open()) {
        cerr << "could not open temporary file:" << filenameTmp << endl;
    }
    tmpFileStream.seekg(0);

    if (ifs.is_open()) {
        ifs.close();
    }

    ifs.open(filename, ios_base::in | ios_base::out);

    ifs.seekg(0);


    string buffer;
    char primaryKeyBuffer[10];
    string keyOnCurrentLine;

    int deleted = 0;
    //read original file and copy to tmp file only what is not filtered
    while (getline(ifs, buffer)) {

        sscanf(buffer.c_str(), "%5c", primaryKeyBuffer);
        primaryKeyBuffer[5] = 0;
        keyOnCurrentLine = string(primaryKeyBuffer);

        if (deadKeys.find(keyOnCurrentLine) != deadKeys.end()) {
            //case current line is supposed to be eliminated
            deleted ++;


        } else {
            //case current line is not supposed to be eliminated

            //copy to tmp file
            tmpFileStream.write(buffer.c_str(), buffer.length());
            tmpFileStream << endl;
        }

    }

    //erase original file
    ifs.close();
    ifs.open(filename, ios_base::out | ios_base::trunc);

    //copy tmp to original file
    tmpFileStream.seekg(0);
    tmpFileStream.clear();
    while(getline(tmpFileStream, buffer)){
        ifs.write(buffer.c_str(), buffer.length());
        ifs << endl;

    }

    tmpFileStream.close();

    ifs.close();

}

void LineMappedFile::flush() {
    if(ifs.is_open()){
        ifs.flush();
    }

}

void LineMappedFile::construct(string filename) {
    this->filename = filename;

    ifs.open(filename);

    if (!ifs.is_open()) {
        cerr << "could not open file:" << filename << endl;
    }

    lineByteAddrMap = buildLineByteAddrMap(ifs, &qtdLines);

}

void LineMappedFile::reset() {
    construct(filename);

}

void LineMappedFile::closeFile() {
    if(ifs.is_open())
        ifs.close();

}

void LineMappedFile::reOpen() {
    closeFile();
    ifs.open(filename);
    lineByteAddrMap = buildLineByteAddrMap(ifs, &qtdLines);
}

void LineMappedFile::addLine() {
    ifs.clear();

    ifs.seekg(0, ios_base::end);

    lineByteAddrMap[qtdLines++] = (int)ifs.tellg();

    ifs.clear();



}
