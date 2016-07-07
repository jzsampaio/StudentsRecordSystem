//
// Created by jasf on 6/25/16.
//

#include <iostream>
#include "StudentRecord.h"

int StudentRecord::QTD_FIELDS = 6;
int StudentRecord::lengthOfField[6] = {5, 6, 23, 5, 9, 2};
TypeOfField StudentRecord::typeOfField[6] = {STRING, INT, STRING, INT, STRING, STRING};

void StudentRecordUtil::writeRecord(fstream &fout, const StudentRecord &record, int pos) {

    fout.seekg(fout.tellg(), ios_base::beg);
    if (!fout.is_open()) {
        cerr << "output stream is not ok!" << endl;
        return;
    }

    ios::fmtflags f(cout.flags());
    if(pos >= 0){
        fout.seekp(pos);
    }

    fout << left;

    fout << setfill(' ') << setw(StudentRecord::lengthOfField[0]) << record.v0 << " ";
    fout << right << setfill('0') << setw(StudentRecord::lengthOfField[1]) << record.v1 << " " << left;
    fout << setfill(' ') << setw(StudentRecord::lengthOfField[2]) << record.v2 << " ";
    fout << setw(StudentRecord::lengthOfField[3]) << record.v3 << " ";
    fout << setfill(' ') << setw(StudentRecord::lengthOfField[4]) << record.v4 << " ";
    fout << setfill(' ') << setw(StudentRecord::lengthOfField[5]) << record.v5;

    fout.flags(f);

    fout << endl;
    flush(fout);


}

StudentRecord StudentRecordUtil::readRecord(istream &fin) {
    StudentRecord out;
//
//    for (int i = 0; i < StudentRecord::QTD_FIELDS; i++) {
//        fin.read((char *) (&out.fields[i]), StudentRecord::lengthOfField[i]);
//    }

    return out;

}

void StudentRecord::set(string v1, int v2, string v3, int v4, string v5, string v6) {
    this->v0 = v1;
    this->v1 = v2;
    this->v2 = v3;
    this->v3 = v4;
    this->v4 = v5;
    this->v5 = v6;

}

StudentRecord::StudentRecord() {
}

StudentRecord::~StudentRecord() {

}

StudentRecord::StudentRecord(const StudentRecord &a) {
    this->v0 = a.v0;
    this->v1 = a.v1;
    this->v2 = a.v2;
    this->v3 = a.v3;
    this->v4 = a.v4;
    this->v5 = a.v5;
}
