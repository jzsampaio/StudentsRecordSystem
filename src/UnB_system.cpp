//
// Created by jasf on 6/25/16.
//

#include<iostream>

using namespace std;

void printMenu();

const string file1 = "lista1.txt";
const string file2 = "lista2.txt";
const string file3 = "lista3.txt";

const string primaryIndexFile1 = "index_lista1.txt";
const string primaryIndexFile2 = "index_lista2.txt";
const string primaryIndexFile3 = "index_lista3.txt";

const string secondaryIndexFile1 = "sec_lista1.txt";


typedef struct StudentRecord StudentRecord;

int main() {

    int option;
    bool shouldFinish;
    string studentCode, studentName, studentAge,

    while (!shouldFinish) {

        printMenu();

        cin >> option;

        switch (option) {
            case 1 :
                cout << "Entre com numero magico associado ao luno:" << endl;
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

