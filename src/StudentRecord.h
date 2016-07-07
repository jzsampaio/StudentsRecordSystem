//
// Created by jasf on 6/25/16.
//

#ifndef GRADERECORDSYSTEM_STUDENTRECORD_H
#define GRADERECORDSYSTEM_STUDENTRECORD_H

#include<string>
#include<fstream>
#include<iomanip>

using namespace std;

union RecordField {
    int intVal;
    string strVal;
};

enum TypeOfField {
    INT, STRING
};

class StudentRecord {
public:
    static int QTD_FIELDS;
    static int lengthOfField[6];
    static TypeOfField typeOfField[6];

    string v0;
    int v1;
    string v2;
    int v3;
    string v4;
    string v5;

    StudentRecord();

    StudentRecord(const StudentRecord &a);

    virtual ~StudentRecord();

    void set(string v1, int v2, string v3, int v4, string v5, string v6);
};

class StudentRecordUtil {
public:
    /**
     * if pos >= 0, moves stream position to 'pos' before writing.
     */
    static void writeRecord(fstream &fout, const StudentRecord &record, int pos = -1);

    static StudentRecord readRecord(istream &fin);
};


#endif //GRADERECORDSYSTEM_STUDENTRECORD_H
