#include <iostream>

#include "CreateIndexTools.h"

using namespace std;

void usage() {
    cout << "./gera_index lista1.txt lista2.txt lista3.txt" << endl;
}


int main(int argc, char **argv) {

    if (argc != 4) {
        cerr << "wrong input!" << endl;
        usage();
        return 0;
    }

    createSortedIndexesForFiles(argv[1], argv[2], argv[3]);

    return 0;
}

