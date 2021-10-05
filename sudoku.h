#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <stack>
#include <array>
#include <vector>

#include "cell.h"

#define RED SetConsoleTextAttribute(hConsole, 4);
#define WHITE SetConsoleTextAttribute(hConsole, 15);
#define GREEN SetConsoleTextAttribute(hConsole, 10);
#define BLUE SetConsoleTextAttribute(hConsole, 9);
#define MAGENTA SetConsoleTextAttribute(hConsole, 13);

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

struct Sudoku
{

    array<array<Cell, 9>, 9> sudoku_;
    array<array<Cell, 9>, 9> sudokuCopy_;
    vector<array<array<Cell, 9>, 9>> solutions;

    stack<array<array<Cell, 9>, 9>> sudokuStack;

    Sudoku()
    {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                sudoku_[y][x] = Cell();
                sudokuCopy_[y][x] = Cell();
            }
        }
    }

    Sudoku(string filename)
    {

        fstream file(filename);

        for (int i = 0; i < 9; i++)
        {
            string line;
            file >> line;

            for (int c = 0; c < 9; c++)
            {
                sudoku_[i][c] = Cell(line[c] - '0');
                sudokuCopy_[i][c] = Cell(line[c] - '0');
            }
        }
    }

    void print()
    {

        for (int y = 0; y < 9; y++)
        {

            if (y == 3 || y == 6)
            {
                cout << "----------------------\n";
            }

            for (int x = 0; x < 9; x++)
            {
                if (x == 3 || x == 6)
                {
                    cout << " |";
                }

                if (sudoku_[y][x].GetVal() > 0)
                {
                    cout << " " << sudoku_[y][x].GetVal();
                }
                else
                {
                    if (sudoku_[y][x].GetVal() == -1)
                    {
                        RED
                                cout
                            << " X";
                        WHITE
                    }
                    else
                    {
                        cout << "  ";
                    }
                }
            }

            cout << endl;
        }
    }

    void printRemaining(int y, int x)
    {

        cout << "|";

        for (int p = 0; p < 9; p++)
        {
            if (sudoku_[y][x].possible[p])
            {
                cout << p + 1 << "|";
            }
            else
            {
                cout << " |";
            }
        }

        cout << "\n";
    }

    void applyRules()
    {

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        for (int y = 0; y < 9; y++)
        {

            for (int x = 0; x < 9; x++)
            {
                if (!sudoku_[y][x].IsSolved())
                {
                    //orriziontal rule
                    for (int o = 0; o < 9; o++)
                    {
                        if (sudoku_[y][o].IsSolved() && sudoku_[y][x].possible[sudoku_[y][o].GetVal() - 1] && o != x)
                        {
#ifdef LOG

                            BLUE;
                            printf("rm %i @ %i,%i orri ", sudoku_[y][o].GetVal(), o, y);

#endif

                            sudoku_[y][x].RemovePossibility(sudoku_[y][o].GetVal());

#ifdef LOG

                            printRemaining(y, x);
                            WHITE;

#endif
                        }
                    }

                    //vertical rule
                    for (int v = 0; v < 9; v++)
                    {
                        if (sudoku_[v][x].IsSolved() && sudoku_[y][x].possible[sudoku_[v][x].GetVal() - 1] && v != y)
                        {
#ifdef LOG

                            GREEN;
                            printf("rm %i @ %i,%i vert ", sudoku_[v][x].GetVal(), x, v);

#endif
                            sudoku_[y][x].RemovePossibility(sudoku_[v][x].GetVal());

#ifdef LOG

                            printRemaining(y, x);
                            WHITE;

#endif
                        }
                    }

                    //cell rule
                    int cellX = x / 3;
                    int cellY = y / 3;

                    for (int cy = cellY * 3; cy < (cellY + 1) * 3; cy++)
                    {
                        for (int cx = cellX * 3; cx < (cellX + 1) * 3; cx++)
                        {
                            if (sudoku_[cy][cx].IsSolved() && (cx != x && cy != y) && sudoku_[y][x].possible[sudoku_[cy][cx].GetVal() - 1])
                            {
#ifdef LOG

                                MAGENTA;
                                printf("rm %i @ %i,%i orri ", sudoku_[cy][cx].GetVal(), cx, cy);

#endif
                                sudoku_[y][x].RemovePossibility(sudoku_[cy][cx].GetVal());

#ifdef LOG

                                printRemaining(y, x);
                                WHITE;

#endif
                            }
                        }
                    }

#ifdef LOG

                    if (sudoku_[y][x].GetNumberOfPos() == 1)
                    {
                        GREEN;
                    }
                    else if (sudoku_[y][x].GetNumberOfPos() == 0)
                    {
                        RED;
                    }
                    else
                    {
                        BLUE;
                    }

                    printf("cell: %i,%i ", x, y);
                    printRemaining(y, x);

                    WHITE;

#endif
                }
            }
        }
    }

    void findMostRuledCell(int *px, int *py)
    {
        int min = 10;
        int cx = -1;
        int cy = -1;

        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {

#ifdef LOG

                printf("c:%i,%i,pos:%i min:%i\n", x, y, sudoku_[y][x].GetNumberOfPos(), min);

#endif

                if (min > sudoku_[y][x].GetNumberOfPos() && sudoku_[y][x].GetNumberOfPos() != 1)
                {
                    min = sudoku_[y][x].GetNumberOfPos();
                    cx = x;
                    cy = y;
                }

                if (sudoku_[y][x].GetNumberOfPos() == 0)
                {
                    *px = -1;
                    *py = -1;
                    return;
                }
            }
        }

        *px = cx;
        *py = cy;
    }

    bool areCopyAndOriginalEqual()
    {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                for (int p = 0; p < 9; p++)
                {
                    if (sudoku_[y][x].possible[p] != sudokuCopy_[y][x].possible[p])
                    {
//difference found
#ifdef LOG

                        cout << "difference found\n";

#endif
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool isSolved()
    {
        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                if (!sudoku_[y][x].IsSolved())
                {
                    return false;
                }
            }
        }
        return true;
    }

    void Solve()
    {

        int minX, minY;

        do
        {
            sudokuCopy_ = sudoku_;

            applyRules();

#ifdef LOG

            print();

#endif

            findMostRuledCell(&minX, &minY);

        } while (!areCopyAndOriginalEqual() && !(minX == -1 || minY == -1));

        if (isSolved())
        {
            solutions.push_back(sudoku_);
        }

        //TODO check for -1 in findMostRuledCell result

        findMostRuledCell(&minX, &minY);

#ifdef LOG

        cout << minX << minY << " ";

        printRemaining(minY, minX);

#endif

        if (minX == -1 || minY == -1)
        {
            //no solution

#ifdef LOG

            cout << "no current solution\n";

#endif

            return;
        }

        for (int p = 0; p < 9; p++)
        {

#ifdef LOG

            RED;
            printf("trying:%i @ %i,%i\n", p, minX, minY);
            WHITE;

#endif

            if (sudoku_[minY][minX].possible[p])
            {
#ifdef LOG

                cout << "push" << p + 1 << "\n";

#endif

                sudokuStack.push(sudoku_);

                sudoku_[minY][minX] = Cell(p + 1);
                Solve();

#ifdef LOG

                cout << "pop" << p + 1 << "\n";

#endif

                sudoku_ = sudokuStack.top();
                sudokuStack.pop();
            }
        }
    }
};

void printArray(array<array<Cell, 9>, 9> sudoku2print)
{
    for (int y = 0; y < 9; y++)
    {

        if (y == 3 || y == 6)
        {
            cout << "----------------------\n";
        }

        for (int x = 0; x < 9; x++)
        {
            if (x == 3 || x == 6)
            {
                cout << " |";
            }

            if (sudoku2print[y][x].GetVal() > 0)
            {
                cout << " " << sudoku2print[y][x].GetVal();
            }
            else
            {
                if (sudoku2print[y][x].GetVal() == -1)
                {
                    RED
                            cout
                        << " X";
                    WHITE
                }
                else
                {
                    cout << "  ";
                }
            }
        }

        cout << endl;
    }
}
