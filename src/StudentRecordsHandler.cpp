//
// Created by jasf on 7/6/16.
//

#include "StudentRecordsHandler.h"
#include "StudentRecord.h"

StudentRecordsHandler::StudentRecordsHandler(string filename, string filenameIndex, string filenameSecondaryIndex,
                                             string filenameGradesA,
                                             string filenameGradesB, string filenameIndexGradeA,
                                             string filenameIndexGradeB)
        : fileStudentRecords(filename),
          filePrimaryIndex(filenameIndex),
          fileSecondaryIndex(filenameSecondaryIndex),
          fileGradesA(filenameGradesA),
          fileGradesB(filenameGradesB),
          fileIndexGradeA(filenameIndexGradeA),
          fileIndexGradeB(filenameIndexGradeB) {

    reloadIndexes();


}

void StudentRecordsHandler::printPrimaryIndex() {

    cout << "Primary index for student records" << endl;
    cout << "primary key | line address on record file | line address of next with same name" << endl;

    for (auto it : inMemoryPrimaryIndex) {
        cout << it.first << "\t" << get<0>(it.second) << "\t" << get<1>(it.second) << endl;
    }


}

void StudentRecordsHandler::printSecondaryIndex() {
    cout << "Secondary name key index" << endl;
    cout << "name\tline addr of first item on linked list" << endl;
    for (auto it : inMemorySecondaryIndex) {
        cout << it.first << "\t" << it.second << endl;
    }

}

void StudentRecordsHandler::addStudent(int d, string name, int k, string str, string str2) {

    //every name in the system must have length 23
    while (name.length() < 23) {
        name += " ";
    }

    //find next id
    string lastPrimaryKey = (*inMemoryPrimaryIndex.rbegin()).first;
    string newPrimaryKey = getNextPrimaryKey(lastPrimaryKey);

    StudentRecord newRecord;
    newRecord.set(newPrimaryKey, to_string(d), name, to_string(k), str, str2);

    fileStudentRecords.clear();
    fileStudentRecords.ifs.seekg(0, ios_base::end);

    int byteAddrOfNewEntry = fileStudentRecords.ifs.tellg();

    StudentRecordUtil::writeStudent(fileStudentRecords.ifs, newRecord);

    int lineAddrOfNewEntry = fileStudentRecords.qtdLines++;
    fileStudentRecords.lineByteAddrMap[lineAddrOfNewEntry] = byteAddrOfNewEntry;

    int oldHeadOfList = -1;
    //is there another user with this name?
    if (inMemorySecondaryIndex.find(name) != inMemorySecondaryIndex.end()) {
        //case yes
        //we must update secondary index to point to the new head of list
        oldHeadOfList = inMemorySecondaryIndex[name];
        inMemorySecondaryIndex[name] = lineAddrOfNewEntry;
    } else {
        //case of new name
        oldHeadOfList = -1;
        inMemorySecondaryIndex[name] = lineAddrOfNewEntry;
    }
    rewriteSecondaryIndex();


    inMemoryPrimaryIndex.insert(make_pair(newPrimaryKey, make_tuple(lineAddrOfNewEntry, oldHeadOfList)));
    rewritePrimaryIndex();

}

void StudentRecordsHandler::rewritePrimaryIndex() {

    filePrimaryIndex.ifs.close();
    filePrimaryIndex.ifs.open(filePrimaryIndex.filename, ios_base::out | ios_base::trunc);

    ios::fmtflags originalFlags(filePrimaryIndex.ifs.flags());
    filePrimaryIndex.ifs << setfill('0') << setw(3) << qtdDeleted << endl;
    filePrimaryIndex.ifs.flags(originalFlags);

    for (auto entry : inMemoryPrimaryIndex) {
        filePrimaryIndex.ifs << entry.first << "\t" << get<0>(entry.second) << "\t" << get<1>(entry.second) << endl;
    }
    filePrimaryIndex.ifs.flags(originalFlags);
    filePrimaryIndex.ifs.close();
    filePrimaryIndex.ifs.open(filePrimaryIndex.filename, ios::in | ios::out);

    cout << "primary index with linked list for names re-written" << endl;

}

void StudentRecordsHandler::rewriteSecondaryIndex() {
    fileSecondaryIndex.ifs.close();
    fileSecondaryIndex.ifs.open(fileSecondaryIndex.filename, ios_base::out | ios_base::trunc);
    ios::fmtflags originalFlags(filePrimaryIndex.ifs.flags());

    for (auto entry : inMemorySecondaryIndex) {
        fileSecondaryIndex.ifs << left << setfill(' ') << setw(23) << get<0>(entry);
        fileSecondaryIndex.ifs.flags(originalFlags);
        fileSecondaryIndex.ifs << "\t" << get<1>(entry) << endl;
    }

    //reopen with regultar permitions
    fileSecondaryIndex.ifs.close();
    fileSecondaryIndex.ifs.open(fileSecondaryIndex.filename, ios::in | ios::out);

}

void StudentRecordsHandler::removeStudent(string primaryKey) {

    if (inMemoryPrimaryIndex.find(primaryKey) == inMemoryPrimaryIndex.end()) {
        cerr << "There's no such student in the system!" << endl;
        cerr.flush();
        return;
    }
    if (get<0>(inMemoryPrimaryIndex[primaryKey]) == -1) {
        cerr << "There's no such student in the system! The student has already been deleted!" << endl;
        cerr.flush();
        return;
    }

    get<0>(inMemoryPrimaryIndex[primaryKey]) = -1;
    qtdDeleted++;

    rewritePrimaryIndex();

    if (qtdDeleted > 5) {
        //read index to find deleted ids

        set<string> deadKeys;
        for (auto it : inMemoryPrimaryIndex) {
            if (get<0>(it.second) == -1) {
                deadKeys.insert(it.first);
            }
        }

        fileStudentRecords.removeLinesWithThesePrimaryKeys(deadKeys);
        fileGradesA.removeLinesWithThesePrimaryKeys(deadKeys);
        fileGradesB.removeLinesWithThesePrimaryKeys(deadKeys);

        qtdDeleted = 0;

        closeFiles();

        cout << "-----------------" << endl;
        cout << "Rebuilding index" << endl;
        cout << "-----------------" << endl;
        createSortedIndexesForFiles(fileStudentRecords.filename, fileGradesA.filename, fileGradesB.filename);

        reloadIndexes();

        cout << "-----------------" << endl;
        cout << "Loading new indes to memory" << endl;
        cout << "-----------------" << endl;
        restartFiles();


        return;

    } else {
        return;
    }


}

void StudentRecordsHandler::flush() {

    fileStudentRecords.flush();
    filePrimaryIndex.flush();
    fileSecondaryIndex.flush();
    fileGradesA.flush();
    fileGradesB.flush();
    fileIndexGradeA.flush();
    fileIndexGradeB.flush();

}

void StudentRecordsHandler::closeFiles() {
    fileStudentRecords.closeFile();
    filePrimaryIndex.closeFile();
    fileSecondaryIndex.closeFile();
    fileGradesA.closeFile();
    fileGradesB.closeFile();
    fileIndexGradeA.closeFile();
    fileIndexGradeB.closeFile();

}


void StudentRecordsHandler::restartFiles() {
    //reset line -> byte map
    fileStudentRecords.reset();
    filePrimaryIndex.reset();
    fileSecondaryIndex.reset();
    fileGradesA.reset();
    fileGradesB.reset();
    fileIndexGradeA.reset();
    fileIndexGradeB.reset();
}

void StudentRecordsHandler::reloadIndexes() {

    closeFiles();
    reOpenAll();

    inMemoryPrimaryIndex = loadStudentRecordPrimaryIndex(filePrimaryIndex.ifs, &qtdDeleted);
    inMemorySecondaryIndex = loadStudentRecordSecondaryIndex(fileSecondaryIndex.ifs);
    inMemoryClassAIndex = loadStudentRecordSecondaryIndexTypeB(fileIndexGradeA.ifs);
    inMemoryClassBIndex = loadStudentRecordSecondaryIndexTypeB(fileIndexGradeB.ifs);

}

void StudentRecordsHandler::reOpenAll() {
    fileStudentRecords.reOpen();
    filePrimaryIndex.reOpen();
    fileSecondaryIndex.reOpen();
    fileGradesA.reOpen();
    fileGradesB.reOpen();
    fileIndexGradeA.reOpen();
    fileIndexGradeB.reOpen();

}

bool StudentRecordsHandler::hasStudent(string primaryKey) {
    return inMemoryPrimaryIndex.find(primaryKey) != inMemoryPrimaryIndex.end();

}

void StudentRecordsHandler::addNota(string primaryKey, int classOpt, string grade) {
    StudentRecordSecondaryIndex *relevantIndex;
    LineMappedFile *relevantFile;
    switch (classOpt) {
        case 1:
            relevantIndex = &inMemoryClassAIndex;
            relevantFile = &fileGradesA;
            break;
        case 2:
            relevantIndex = &inMemoryClassBIndex;
            relevantFile = &fileGradesB;

            break;
        default:
            cerr << "Invalid option for class! only 2 registered!" << endl;
            return;
    }

    if (relevantIndex->find(primaryKey) != relevantIndex->end()) {
        //case of a student that is having its grade updated

        //find address of grade record on file
        int lineAddr = (*relevantIndex)[primaryKey];

        //read record, update it and write again
        relevantFile->seekLine(lineAddr);
        StudentGradeRecord currentRecord = StudentRecordUtil::readGradeRecord(relevantFile->ifs);
        currentRecord.updateGrade(grade);
        relevantFile->seekLine(lineAddr);
        StudentRecordUtil::writeGrade(relevantFile->ifs, currentRecord);


    } else {
        //case of a new student in that class


        //retrieve number and name of student
        //at this point we assume the student exists

        //retrieve studen record

        fileStudentRecords.reOpen();
        int lineAddr = get<0>(inMemoryPrimaryIndex[primaryKey]);
        fileStudentRecords.seekLine(lineAddr);

        StudentRecord studentRecord = StudentRecordUtil::readStudentRecord(fileStudentRecords.ifs);

        string number = studentRecord.v1;
        string name = studentRecord.name;

        StudentGradeRecord newEntry(primaryKey, number, name, grade);

        //add line (the following call already positions stream at end of file
        relevantFile->addLine();

        StudentRecordUtil::writeGrade(relevantFile->ifs, newEntry);


    }


}

void StudentRecordsHandler::printReport() {

    fileStudentRecords.reOpen();
    fileGradesA.reOpen();
    fileGradesB.reOpen();
    for (auto e : inMemoryPrimaryIndex) {
        cout << "----------------------------------------" << endl;
        string primaryKey = e.first;
        int lineAddrOnDataFile = get<0>(e.second);

        fileStudentRecords.seekLine(lineAddrOnDataFile);

        StudentRecord r = StudentRecordUtil::readStudentRecord(fileStudentRecords.ifs);

        string primaryKeyOnRecordFile = r.primaryKey;
        if (primaryKey != primaryKeyOnRecordFile)
            cout << "ERROR: keys diverge! Fix me untill you see no longer this msg!" << endl;
        string matricula = r.v1;
        string name = r.name;
        string operacao = r.v3;
        string curso = r.v4;
        string turma = r.v5;

        cout << "<" << matricula << " " << name << " " << operacao << " " << curso << " " << turma << ">" << endl;
        cout << "Computacao Quantica Avancada III:";

        if (inMemoryClassAIndex.find(primaryKey) != inMemoryClassAIndex.end()) {
            lineAddrOnDataFile = inMemoryClassAIndex[primaryKey];
            fileGradesA.seekLine(lineAddrOnDataFile);

            StudentGradeRecord gradeRec = StudentRecordUtil::readGradeRecord(fileGradesA.ifs);

            if (gradeRec.primaryKey != primaryKey) {
                cout << endl << "ERROR: keys diverge! Fix me untill you see no longer this msg! expected:" <<
                primaryKey << "| got:" << gradeRec.primaryKey << endl;
            }
            if (gradeRec.v1 != matricula) {
                cout << endl << "ERROR: matricula diverge! Fix me untill you see no longer this msg!" << endl;
            }

            cout << gradeRec.grade << endl;

        } else {
            cout << "SR" << endl;
        }

        cout << "Modelagem Aeroespacial Aliegina II:";

        if (inMemoryClassBIndex.find(primaryKey) != inMemoryClassBIndex.end()) {
            lineAddrOnDataFile = inMemoryClassBIndex[primaryKey];
            fileGradesB.seekLine(lineAddrOnDataFile);

            StudentGradeRecord gradeRec = StudentRecordUtil::readGradeRecord(fileGradesB.ifs);

            cout << gradeRec.grade << endl;

        } else {
            cout << "SR" << endl;
        }


    }

}

void StudentRecordsHandler::printReportOnThisName(string name) {

    //name must have 23 letters

    while (name.length() < 23) {
        name += " ";
    }

    //locate all primary keys with this name
    // list of primarykey, line on data file
    vector<pair<string, int>> selectedKeys;

    if (inMemorySecondaryIndex.find(name) == inMemorySecondaryIndex.end()) {
        cout << "No student with this name!" << endl;
        return;
    }

    int lineAddr = inMemorySecondaryIndex[name];

    stringstream ss;

    filePrimaryIndex.clear();

    string primaryKey, buffer;
    int lineAddrOnDataFile;
    int lineOfNext;
    while (lineAddr != -1) {

        filePrimaryIndex.seekLine(lineAddr + 1);

        getline(filePrimaryIndex.ifs, buffer);
        ss.clear();
        ss.str(buffer);

        ss >> primaryKey >> lineAddrOnDataFile >> lineOfNext;

        selectedKeys.push_back(make_pair(primaryKey, lineAddrOnDataFile));

        lineAddr = lineOfNext;
    }

    //now we locate the grades for every one of those students

    for (auto e : selectedKeys) {
        string key = e.first;
        int lineAddrOnDataFile = e.second;

        //load student record
        fileStudentRecords.seekLine(lineAddrOnDataFile);
        StudentRecord student = StudentRecordUtil::readStudentRecord(fileStudentRecords.ifs);
        string matricula = student.v1;
        string notaA, notaB;

        if (inMemoryClassAIndex.find(key) == inMemoryClassAIndex.end()) {
            notaA = "SR";
        } else {
            lineAddrOnDataFile = inMemoryClassAIndex[key];
            fileGradesA.seekLine(lineAddrOnDataFile);
            StudentGradeRecord gradeRec = StudentRecordUtil::readGradeRecord(fileGradesA.ifs);

            notaA = gradeRec.grade;
        }

        if (inMemoryClassBIndex.find(key) == inMemoryClassBIndex.end()) {
            notaB = "SR";
        } else {
            lineAddrOnDataFile = inMemoryClassBIndex[key];
            fileGradesB.seekLine(lineAddrOnDataFile);
            StudentGradeRecord gradeRec = StudentRecordUtil::readGradeRecord(fileGradesB.ifs);

            notaB = gradeRec.grade;
        }


        cout << "---------------" << endl;
        cout << "nome:" << student.name << endl;
        cout << "primary key:" << student.primaryKey << endl;
        cout << "matricula:" << matricula << endl;
        cout << "operacao:" << student.v3 << endl;
        cout << "curso:" << student.v4 << endl;
        cout << "turma:" << student.v5 << endl;
        cout << "\t Computacao Quantica Avancada III: " << notaA << endl;
        cout << "\t Modelagem Aeroespacial Alienigina II:: " << notaB << endl;
        cout << "---------------" << endl;

    }


}
