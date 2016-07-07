//
// Created by jasf on 7/6/16.
//

#ifndef GRADERECORDSYSTEM_STUDENTRECORDSHANDLER_H
#define GRADERECORDSYSTEM_STUDENTRECORDSHANDLER_H


#include "jIndexUtil.h"
#include "CreateIndexTools.h"

class StudentRecordsHandler {
public:
    LineMappedFile fileStudentRecords;
    LineMappedFile filePrimaryIndex;
    LineMappedFile fileSecondaryIndex;

    int qtdDeleted;
    StudentRecordPrimaryIndex inMemoryPrimaryIndex;
    StudentRecordSecondaryIndex inMemorySecondaryIndex;

    StudentRecordsHandler(string filename, string filenameIndex, string filenameSecondaryIndex);

    void printPrimaryIndex();
    void printSecondaryIndex();

    void addStudent(int d, string name, int k, string str, string str2);

private:

    void rewritePrimaryIndex();
    void rewriteSecondaryIndex();



};


#endif //GRADERECORDSYSTEM_STUDENTRECORDSHANDLER_H
