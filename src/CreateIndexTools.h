//
// Created by jasf on 6/25/16.
//

#ifndef GRADERECORDSYSTEM_CREATEINDEXTOOLS_H
#define GRADERECORDSYSTEM_CREATEINDEXTOOLS_H
#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

typedef tuple<string, int, string> RelevantForIndexConstructionUserData;
typedef vector<tuple<string, int, int>> PrimaryIndex;
typedef map<string, int> SecondaryIndex;
/**
 * returns vector of entries <primaryKey, addr on file> where primary key is a string
 * of type IDXXX, X in {0,1,...9} and addr on file is the address on the input file
 * where you'll find the record corresponding to the given primary key. The vector
 * is sorted according to the primary key
 *
 * The input file is taken to have records with fields of fixed length.
 * It is assumed one record per line.
 */
pair<PrimaryIndex, SecondaryIndex> buildPrimaryAndSecondaryIndexForFixedLengthFieldsFile(string filename);

/**
 * Build a secondary index implemented as linked lists for a fixed a file with
 * one record per line, each record with fixed length fields. We build the index
 * for the field 'name' which is assumed to be the third field inside the record.
 */
vector<pair<string, int>> buildSecondaryIndexForFixedLengthFieldsFile(string filename);

/**
 * returns vector of entries <primaryKey, addr on file> where primary key is a string
 * of type IDXXX, X in {0,1,...9} and addr on file is the address on the input file
 * where you'll find the record corresponding to the given primary key. The vector
 * is sorted according to the primary key.
 *
 *
 * The input file is taken to have records with fields with variable length. A '|'
 * is between consecutive fields and there's one recorde per line.
 */
vector<pair<string, int>> buildPrimaryIndexForVariableLengthFieldsFile(string filename);

/**
 * associated with every input file is an index file. Get the index filename for an input filename with
 * this method
 */
string getPrimaryIndexNameForInputFile(string filename);

string getSecondaryIndexNameForInputFile(string filename);

void writeIndexToFile(vector<pair<string, int>> index, string fileName, bool printQtdRemoved = false,
                      int qtdRemoved = 0);

/**
 * creates a sorted index for each the files. The firts file is supposed to be of records with fields of fixed length
 * and the second and third with fields of variable length.
 *
 * For both types(variable and fixed length fields) we assume a record per line.
 *
 * For variable length fields, it is assumed a '|' between fields.
 *
 * Indexes are written to file 'index_' + filename
 *
 */
void createSortedIndexesForFiles(string file0, string file1, string file2);

/**
 * loadIndex as a map of primary key -> address where you'll find the record associated with the primary key in the
 * data file corresponding to this index.
 *
 * Recall map in c++ keeps the elemts sorted, thus you can find the element with the highest key with
 * map.rbegin()
 */
map<string, int> loadIndex(string file);

#endif //GRADERECORDSYSTEM_CREATEINDEXTOOLS_H
