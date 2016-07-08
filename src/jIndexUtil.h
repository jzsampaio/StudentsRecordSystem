//
// Created by jasf on 6/25/16.
//

#ifndef GRADERECORDSYSTEM_JINDEX_H
#define GRADERECORDSYSTEM_JINDEX_H

#include <string>
#include <map>
#include <set>
#include <iostream>
#include <fstream>

using namespace std;
typedef map<string, int> jIndex;


class jIndexUtil {

public:
    static string getNextKey(const jIndex &index);

};

/**
 * build a map for idx of line -> byte in the file where to find this line
 *
 * Note that idx starts from 0
 */
map<int, int> buildLineByteAddrMap(fstream &ifs, int *qtdLines);

/**
 * A class that wraps an ifstream and a map of line to by inside the file
 * where to read that line.
 */
class LineMappedFile {
public:
    fstream ifs;
    string filename;
    map<int, int> lineByteAddrMap;
    int qtdLines;

    LineMappedFile(string filename);

    void construct(string filename);

    void reset();

    void seekLine(int idx);

    void printLineByteMap();

    string readLastLine();

    void clear();

    void closeFile();

    void reOpen();

    /**
     * removal is made by copying to a temporary file only non eliminated records, then erasing
     * original file, and copying back from temporary file to original file.
     *
     */
    void removeLinesWithThesePrimaryKeys(set<string> deadKeys);

    void flush();

    void addLine();

};

string findLastPrimaryKey(LineMappedFile &ifs);
string getNextPrimaryKey(const string &in);

#endif //GRADERECORDSYSTEM_JINDEX_H
