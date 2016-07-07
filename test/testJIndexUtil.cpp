//
// Created by jasf on 6/25/16.
//

#include <assert.h>
#include <sstream>
#include <iomanip>

#include "jIndexUtil.h"
#include "StudentRecord.h"

int main() {

    stringstream ss;
    string currentKey, nextKey;
    jIndex a;

    for (int i = 0; i < 1000; i += 20) {
        ss.str(std::string());
        ss.clear();

        ss << "ID" << setfill('0') << setw(3) << i;

        currentKey = ss.str();
        a[currentKey] = 0;

        ss.str(std::string());
        ss.clear();
        ss << "ID" << setfill('0') << setw(3) << (i + 1);

        nextKey = ss.str();

        assert(jIndexUtil::getNextKey(a) == nextKey);

    }

    const string testFileName = "writingTest.txt";

    fstream ofs(testFileName);
    StudentRecord studentRecordA, studentRecordB;

    studentRecordA.set("ID001", 24312, "Leonardo", 34, "G", "AB");
    studentRecordB.set("ID019", 312272, "Andreza", 34, "G", "AB");

    ofs.seekg(0, ios_base::end);
    StudentRecordUtil::writeRecord(ofs, studentRecordA);
    StudentRecordUtil::writeRecord(ofs, studentRecordB);

    ofs.close();


}

