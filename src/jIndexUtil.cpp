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
