//
// Created by jasf on 6/25/16.
//

#include <assert.h>
#include <sstream>
#include <iomanip>
#include <StudentRecordsHandler.h>

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

    string filenameStudentRecords = "writingTest.txt";

    fstream ofs;
    ofs.open(filenameStudentRecords, ios_base::in | ios_base::out | ios_base::trunc);

    if (!ofs.is_open()) {
        cerr << "could not open file to write!" << endl;
    }
    StudentRecord studentRecordA, studentRecordB;

    vector<StudentRecord> records;

    for (int i = 0; i < 20; i++) {
        char id[6];
        sprintf(id, "ID%03d", i);
        id[5] = 0;

        records.emplace_back(string(id), to_string(i), "name#" + to_string(i), "34", "G", "AB");
    }

    for (auto e : records)
        StudentRecordUtil::writeStudent(ofs, e);

    vector<StudentRecord> readRecords;

    ofs.close();

    ofs.open(filenameStudentRecords, ios_base::in);
    if (!ofs.is_open()) {
        cerr << "could not open file to write!" << endl;
    }

    fpos<mbstate_t> readThisPos = 0;
    string lineBuffer;


    while (getline(ofs, lineBuffer)) {
        ofs.seekg(readThisPos);

        readRecords.push_back(StudentRecordUtil::readStudentRecord(ofs));
        readThisPos = ofs.tellg();

    }

    assert(readRecords.size() == records.size());

    bool hasProblem = false;
    for (int i = 0; i < readRecords.size(); i++) {
        const StudentRecord &expected = records[i];
        const StudentRecord &got = readRecords[i];

        if (expected.primaryKey.compare(got.primaryKey) != 0) {
            cerr << "error fielf primary key for record #" << i << " got: " << got.primaryKey << "|\t expected:" <<
            expected.primaryKey << endl;
            hasProblem = true;
        }
        if (stoi(expected.v1) != stoi(got.v1)) {
            cerr << "error field v1 for record #" << i << " got:" << stoi(got.v1) << "|\t expected:" <<
            stoi(expected.v1) << endl;
            hasProblem = true;
        }
        if (expected.name.compare(got.name) != 0) {
            cerr << "error field name for record #" << i << " got: " << got.name << "|\t expected:" << expected.name <<
            endl;
            hasProblem = true;
        }
        if (stoi(expected.v3) != stoi(got.v3)) {
            cerr << "error fielf v3 for record #" << i << " got: " << got.v3 << "|\t expected:" << expected.v3 << endl;
            hasProblem = true;
        }
        if (expected.v4.compare(got.v4) != 0) {
            cerr << "error field v4 for record #" << i << " got: " << got.v4 << "|\t expected:" << expected.v4 << endl;
            hasProblem = true;
        }
        if (expected.v5.compare(got.v5) != 0) {
            cerr << "error field v5 for record #" << i << " got: " << got.v5 << "|\t expected:" << expected.v5 << endl;
            hasProblem = true;
        }

    }

    ofs.close();
    string filenameGradesA = "gradesA.txt";

    ofs.open(filenameGradesA, ios_base::out | ios_base::in | ios_base::trunc);

    vector<StudentGradeRecord> gradesA;
    for (int i = 0; i < 15; i++) {
        char id[6];
        sprintf(id, "ID%03d", i);
        id[5] = 0;

        gradesA.emplace_back(string(id), to_string(i), "name#" + to_string(i), "9.8");
    }

    for (auto e : gradesA)
        StudentRecordUtil::writeGrade(ofs, e);

    ofs.close();

    ofs.open(filenameGradesA, ios_base::out | ios_base::in);

    if (!ofs.is_open()) {
        cerr << "could not open file: " << filenameGradesA << endl;
    }

    readThisPos = 0;
    lineBuffer;

    vector<StudentGradeRecord> readGradesA;

    while (getline(ofs, lineBuffer)) {
        ofs.seekg(readThisPos);

        readGradesA.push_back(StudentRecordUtil::readGradeRecord(ofs));
        readThisPos = ofs.tellg();

    }

    if (readGradesA.size() != gradesA.size()) {
        cerr << "qtd written:" << gradesA.size() << " qtd read:" << readGradesA.size() << endl;
    }
    assert(readGradesA.size() == gradesA.size());

    for (int i = 0; i < gradesA.size(); i++) {
        const StudentGradeRecord &expected = gradesA[i];
        const StudentGradeRecord &got = readGradesA[i];

        if (expected.primaryKey.compare(got.primaryKey) != 0) {
            cerr << "expected:" << expected.primaryKey << "| got:" << got.primaryKey << "|" << endl;
            hasProblem = true;
        }
        if (expected.v1.compare(got.v1) != 0) {
            cerr << "expected:" << expected.v1 << "| got:" << got.v1 << "|" << endl;
            hasProblem = true;
        }
        if (expected.name.compare(got.name) != 0) {
            cerr << "expected:" << expected.name << "| got:" << got.name << "|" << endl;
            hasProblem = true;
        }
        if (expected.grade.compare(got.grade) != 0) {
            cerr << "expected:" << expected.grade << "| got:" << got.grade << "|" << endl;
            hasProblem = true;
        }


    }

    assert(!hasProblem);


    createSortedIndexesForFiles(filenameStudentRecords, filenameGradesA, filenameGradesA);

    StudentRecordsHandler handler(filenameStudentRecords,
                                  "index_" + filenameStudentRecords,
                                  "sec_" + filenameStudentRecords,
                                  (std::basic_string<char, char_traits<_CharT>, allocator<_CharT>>()),
                                  (std::basic_string<char, char_traits<_CharT>, allocator<_CharT>>()),
                                  (std::basic_string<char, char_traits<_CharT>, allocator<_CharT>>()),
                                  (std::basic_string<char, char_traits<_CharT>, allocator<_CharT>>()));

    set<string> deadKeys;

    deadKeys.insert("ID010");
    deadKeys.insert("ID015");

    handler.fileStudentRecords.removeLinesWithThesePrimaryKeys(deadKeys);

    LineMappedFile fileGradesA(filenameGradesA);

    fileGradesA.removeLinesWithThesePrimaryKeys(deadKeys);


}

