//
// Created by jasf on 7/6/16.
//

#include "StudentRecordsHandler.h"
#include "StudentRecord.h"

StudentRecordsHandler::StudentRecordsHandler(string filename, string filenameIndex, string filenameSecondaryIndex)
        : fileStudentRecords(filename),
          filePrimaryIndex(filenameIndex),
          fileSecondaryIndex(filenameSecondaryIndex) {


    inMemoryPrimaryIndex = loadStudentRecordPrimaryIndex(filePrimaryIndex.ifs, &qtdDeleted);
    inMemorySecondaryIndex = loadStudentRecordSecondaryIndex(fileSecondaryIndex.ifs);


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
    while(name.length() < 23){
        name += " ";
    }

    //find next id
    string lastPrimaryKey = (*inMemoryPrimaryIndex.rbegin()).first;
    string newPrimaryKey = getNextPrimaryKey(lastPrimaryKey);

    StudentRecord newRecord;
    newRecord.set(newPrimaryKey, d, name, k, str, str2);

    fileStudentRecords.clear();
    fileStudentRecords.ifs.seekg(0, ios_base::end);

    int byteAddrOfNewEntry = fileStudentRecords.ifs.tellg();

    StudentRecordUtil::writeRecord(fileStudentRecords.ifs, newRecord);

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
        filePrimaryIndex.ifs <<  entry.first << "\t" << get<0>(entry.second) << "\t" << get<1>(entry.second) << endl;
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
