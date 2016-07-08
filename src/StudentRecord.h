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
    static int lengthOfField[6];

    string primaryKey;
    string v1;
    string name;
    string v3;
    string v4;
    string v5;

    StudentRecord();

    StudentRecord(string v1, string v2, string v3, string v4, string v5, string v6);

    StudentRecord(const StudentRecord &a);

    virtual ~StudentRecord();

    void set(string v0, string v1, string v2, string v3, string v4, string v5);
};

class StudentGradeRecord {
public:
    int lengthOfField[4];

    string primaryKey;
    string v1;
    string name;
    string grade;

    StudentGradeRecord();

    StudentGradeRecord(string v0, string v1, string v2, string v3);

    StudentGradeRecord(const StudentGradeRecord &a);

    virtual ~StudentGradeRecord();

    void set(string v0, string v1, string v2, string v3);

    void updateGrade(string newVal);

};

class StudentRecordUtil {
public:
    /**
     * if pos >= 0, moves stream position to 'pos' before writing.
     */
    static void writeStudent(fstream &fout, const StudentRecord &record, int pos = -1);

    static StudentRecord readStudentRecord(fstream &fin);

    /**
     * write grade at the position the strem is
     */
    static void writeGrade(fstream &stream, const StudentGradeRecord &record);

    static StudentGradeRecord readGradeRecord(fstream &stream);


};


#endif //GRADERECORDSYSTEM_STUDENTRECORD_H
