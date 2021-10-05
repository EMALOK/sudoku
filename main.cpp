

#include <iostream>
#include <fstream>
#include <Windows.h>

#include "cell.h"
#include "sudoku.h"



using namespace std;

int main()
{
    //file input
    string filename;
    cout << "filename: ";
    cin >> filename;

    Sudoku sudoku(filename);

    cout << "SUDOKU:\n";

    sudoku.print();

    cout << "\n\n\n\n";

    sudoku.Solve();

    for(int i = 0; i < sudoku.solutions.size();i++){
        printf("solution n: %i\n",i + 1);

        printArray(sudoku.solutions[i]);
    }

    
}