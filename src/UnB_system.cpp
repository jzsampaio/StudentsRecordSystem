//
// Created by jasf on 6/25/16.
//

#include<iostream>
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

    StudentRecordsHandler studentRecords(file1,
                                         primaryIndexFile1,
                                         secondaryIndexFile1,
                                         file2,
                                         file3,
                                         primaryIndexFile2,
                                         primaryIndexFile3);


    while (!shouldFinish) {

        printMenu();

        cin >> option;

        switch (option) {
            case 1 : {
                cout << "Entre com os dados do aluno: int, string, int , string, string :" << endl;
                int d, d2;
                string name, str1, str2;
                cin >> d >> name >> d2 >> str1 >> str2;

                name = name.substr(0, 23);
                str1 = str1.substr(0, 9);
                str2 = str2.substr(0, 2);

                studentRecords.addStudent(d, name, d2, str1, str2);
                break;
            }
            case 2 : {
                cout << "Entre com a chave primária associada ao aluno:" << endl;
                string primaryKey;
                cin >> primaryKey;

                studentRecords.removeStudent(primaryKey);

                break;

            }
            case 3 : {
                string primaryKey;
                int option;
                string grad;

                cout << "Entre com a chave primária associada ao aluno:" << endl;
                cin >> primaryKey;

                cout << "Entre com a dicsiplina (1 ou 2):" << endl;
                cin >> option;

                cout << "Entre com a nota do aluno:" << endl;
                cin >> grad;

                if (studentRecords.hasStudent(primaryKey)) {
                    studentRecords.addNota(primaryKey, option, grad);
                } else {
                    cout << "\t-------Error: Student is not in the system!-------" << endl;
                }


                break;

            }
            case 4 :
                studentRecords.printReport();
            case 5 :{
                string name;
                cout << "Entre com o nome do aluno a ser procurado:" << endl;
                cin >> name;

                studentRecords.printReportOnThisName(name);

                break;

            }
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


