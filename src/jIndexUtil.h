//
// Created by jasf on 6/25/16.
//

#ifndef GRADERECORDSYSTEM_JINDEX_H
#define GRADERECORDSYSTEM_JINDEX_H

#include <string>
#include <map>

using namespace std;
typedef map<string, int> jIndex;


class jIndexUtil {

public:
    static string getNextKey(const jIndex &index);

};


#endif //GRADERECORDSYSTEM_JINDEX_H
