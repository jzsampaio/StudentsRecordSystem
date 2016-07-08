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

    LineMappedFile fileGradesA;
    LineMappedFile fileGradesB;

    LineMappedFile fileIndexGradeA;
    LineMappedFile fileIndexGradeB;

    int qtdDeleted;
    StudentRecordPrimaryIndex inMemoryPrimaryIndex;
    StudentRecordSecondaryIndex inMemorySecondaryIndex;
    StudentRecordSecondaryIndex inMemoryClassAIndex;
    StudentRecordSecondaryIndex inMemoryClassBIndex;

    StudentRecordsHandler(string filename, string filenameIndex, string filenameSecondaryIndex, string filenameGradesA,
                          string filenameGradesB, string filenameIndexGradeA, string filenameIndexGradeB);

    void printPrimaryIndex();
    void printSecondaryIndex();

    void addStudent(int d, string name, int k, string str, string str2);

    /**
     * if the first item if true, then you need to remove the students from other indexes.
     * In this case, remove their entries in the benchmark files, rebuild each individual index
     * and then restart LineMappedFile
     */
    void removeStudent(string primaryKey);

    void closeFiles();

    void restartFiles();

    void flush();

    void reloadIndexes();

    void reOpenAll();

    bool hasStudent(string primaryKey);

    void addNota(string primaryKey, int classOpt, string grade);

    void printReport();

    void printReportOnThisName(string name);



private:

    void rewritePrimaryIndex();
    void rewriteSecondaryIndex();



};


#endif //GRADERECORDSYSTEM_STUDENTRECORDSHANDLER_H
