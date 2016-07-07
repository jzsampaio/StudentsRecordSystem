//
// Created by jasf on 6/25/16.
//

#include<iostream>
#include "jIndexUtil.h"
#include "StudentRecordsHandler.h"

using namespace std;

void printMenu();

const string file1 = "lista1.txt";
const string file2 = "lista2.txt";
const string file3 = "lista3.txt";

const string primaryIndexFile1 = "index_lista1.txt";
const string primaryIndexFile2 = "index_lista2.txt";
const string primaryIndexFile3 = "index_lista3.txt";

const string secondaryIndexFile1 = "sec_lista1.txt";


int main() {

    int option;
    bool shouldFinish;
    string studentCode, studentName, studentAge;

    LineMappedFile fileClassA(file2);
    LineMappedFile fileClassB(file3);

    LineMappedFile fileClassAPrimaryIndex(primaryIndexFile2);
    LineMappedFile fileClassBPrimaryIndex(primaryIndexFile3);

    StudentRecordsHandler studentRecords(file1, primaryIndexFile1, secondaryIndexFile1);

    cout << "last key:" << findLastPrimaryKey(studentRecords.filePrimaryIndex) << endl;
    cout << "next key:" << getNextPrimaryKey(findLastPrimaryKey(studentRecords.filePrimaryIndex)) << endl;

    studentRecords.printPrimaryIndex();

    studentRecords.printSecondaryIndex();


    while (!shouldFinish) {

        printMenu();

        cin >> option;

        switch (option) {
            case 1 :
                cout << "Entre com os dados do aluno: int, string, int , string, string :" << endl;
                int d, d2;
                string name, str1, str2;
                cin >> d >> name >> d2 >> str1 >> str2;

                name = name.substr(0, 23);
                str1 = str1.substr(0, 9);
                str2 = str2.substr(0, 2);

                studentRecords.addStudent(d, name, d2, str1, str2);
                break;
            case 2 :
                break;
            case 3 :
                break;
            case 4 :
                break;
            case 5 :
                break;
            case 6 :
                cout << "Fim da execucao" << endl;
                shouldFinish = true;
        }


    }


}

void printMenu() {
    cout << "Sistem de Notas UnB v0.1 beta" << endl;
    cout << "1. Inserir novo aluno na UnB" << endl;
    cout << "2. Jubilar um aluno da UnB" << endl;
    cout << "3. Dar nota ao aluno numa disciplina" << endl;
    cout << "4. Relatrio" << endl;
    cout << "5. Buscar" << endl;
    cout << "6. Sair" << endl;
}


