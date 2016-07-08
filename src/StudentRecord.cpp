//
// Created by jasf on 6/25/16.
//

#include <iostream>
#include <sstream>
#include "StudentRecord.h"

int StudentRecord::lengthOfField[6] = {5, 6, 23, 5, 9, 2};


StudentRecord StudentRecordUtil::readStudentRecord(fstream &fin) {
    char buffer0[StudentRecord::lengthOfField[0] + 1];
    char buffer1[StudentRecord::lengthOfField[1] + 1];
    char buffer2[StudentRecord::lengthOfField[2] + 1];
    char buffer3[StudentRecord::lengthOfField[3] + 1];
    char buffer4[StudentRecord::lengthOfField[4] + 1];
    char buffer5[StudentRecord::lengthOfField[5] + 1];

    string lineBuffer;

    getline(fin, lineBuffer);

    stringstream lineStream(lineBuffer);

    string v0, v1, v2, v3, v4, v5;

    lineStream.read(buffer0, StudentRecord::lengthOfField[0]);
    lineStream.get();
    lineStream.read(buffer1, StudentRecord::lengthOfField[1]);
    lineStream.get();
    lineStream.read(buffer2, StudentRecord::lengthOfField[2]);
    lineStream.get();
    lineStream.read(buffer3, StudentRecord::lengthOfField[3]);
    lineStream.get();
    lineStream.read(buffer4, StudentRecord::lengthOfField[4]);
    lineStream.get();
    lineStream.read(buffer5, StudentRecord::lengthOfField[5]);

    buffer0[StudentRecord::lengthOfField[0]] = '\0';
    buffer1[StudentRecord::lengthOfField[1]] = '\0';
    buffer2[StudentRecord::lengthOfField[2]] = '\0';
    buffer3[StudentRecord::lengthOfField[3]] = '\0';
    buffer4[StudentRecord::lengthOfField[4]] = '\0';
    buffer5[StudentRecord::lengthOfField[5]] = '\0';

    v0 = string(buffer0);
    v1 = string(buffer1);
    v2 = string(buffer2);
    v3 = string(buffer3);
    v4 = string(buffer4);
    v5 = string(buffer5);

    return StudentRecord(v0, v1, v2, v3, v4, v5);

}

string fill(string in, int size, char c){
    while(in.length() < size)
        in += c;

    return in;
}
string fillLeft(string in, int size, char c){
    while(in.length() < size)
        in = c + in;

    return in;
}

void StudentRecord::set(string v0, string v1, string v2, string v3, string v4, string v5) {
    this->primaryKey = fill(v0, StudentRecord::lengthOfField[0], ' ');
    this->v1 = fillLeft(v1, StudentRecord::lengthOfField[1], '0');
    this->name = fill(v2, StudentRecord::lengthOfField[2], ' ');
    this->v3 = fill(v3, StudentRecord::lengthOfField[3], ' ');
    this->v4 = fill(v4, StudentRecord::lengthOfField[4], ' ');
    this->v5 = fill(v5, StudentRecord::lengthOfField[5], ' ');

}

StudentRecord::StudentRecord() {
}

StudentRecord::~StudentRecord() {

}

StudentRecord::StudentRecord(const StudentRecord &a) {
    this->primaryKey = a.primaryKey;
    this->v1 = a.v1;
    this->name = a.name;
    this->v3 = a.v3;
    this->v4 = a.v4;
    this->v5 = a.v5;
}

StudentGradeRecord::StudentGradeRecord() {

}

StudentGradeRecord::StudentGradeRecord(const StudentGradeRecord &a) {
    this->primaryKey = a.primaryKey;
    this->v1 = a.v1;
    this->name = a.name;
    this->grade = a.grade;

}

StudentGradeRecord::~StudentGradeRecord() {

}

void StudentGradeRecord::set(string v0, string v1, string v2, string v3) {
    this->primaryKey = v0;
    this->v1 = v1;
    this->name = v2;
    this->grade = v3;

    lengthOfField[0] = (int) primaryKey.length();
    lengthOfField[1] = (int) this->v1.length();
    lengthOfField[2] = (int) name.length();
    lengthOfField[3] = (int) this->grade.length();

}

StudentGradeRecord::StudentGradeRecord(string v0, string v1, string v2, string v3) {
    set(v0, v1, v2, v3);


}

StudentRecord::StudentRecord(string v1, string v2, string v3, string v4, string v5, string v6) {
    set(v1, v2, v3, v4, v5, v6);

}


void StudentRecordUtil::writeGrade(fstream &stream, const StudentGradeRecord &record) {
    auto originalFlags = stream.flags();

    stream << record.primaryKey << "|";
    stream << record.v1 << "|";
    stream << record.name << "|";
    stream << record.grade << endl;


}

void StudentGradeRecord::updateGrade(string newVal) {
    if(newVal.length() != grade.length()){
        cerr << "Trying to update grade with more digits than originally! Refusing update!" << endl;
        return;
    }

    grade = newVal;


}

void StudentRecordUtil::writeStudent(fstream &fout, const StudentRecord &record, int pos) {
    fout.clear();
    fout.seekg(fout.tellg(), ios_base::beg);

    if (!fout.is_open()) {
        cerr << "output stream is not ok!" << endl;
        return;
    }

    ios::fmtflags f(cout.flags());
    if (pos >= 0) {
        fout.seekp(pos);
    }

    fout << left;

    fout << setfill(' ') << setw(StudentRecord::lengthOfField[0]) << record.primaryKey << " ";
    fout << right << setfill('0') << setw(StudentRecord::lengthOfField[1]) << record.v1 << " " << left;
    fout << setfill(' ') << setw(StudentRecord::lengthOfField[2]) << record.name << " ";
    fout << setw(StudentRecord::lengthOfField[3]) << record.v3 << " ";
    fout << setfill(' ') << setw(StudentRecord::lengthOfField[4]) << record.v4 << " ";
    fout << setfill(' ') << setw(StudentRecord::lengthOfField[5]) << record.v5;

    fout.flags(f);

    fout << endl;
    flush(fout);


}

StudentGradeRecord StudentRecordUtil::readGradeRecord(fstream &stream) {
    stream.clear();
    stream.seekg(stream.tellg(), ios_base::beg);
    stream.clear();

    string lineBuffer;

    getline(stream, lineBuffer);

    stringstream lineStream(lineBuffer);

    string v0, v1, v2, v3;

    getline(lineStream, v0, '|');
    getline(lineStream, v1, '|');
    getline(lineStream, v2, '|');
    getline(lineStream, v3, '\r');

    return StudentGradeRecord(v0, v1, v2, v3);
}
