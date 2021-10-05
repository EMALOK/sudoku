#pragma once

struct Cell
{
    bool possible[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    Cell(int v)
    {
        if (v != 0 && v < 10)
        {
            //è un valore accettabile

            for (int i = 0; i < 9; i++)
            {
                if (i == v - 1)
                {
                    possible[i] = true;
                }
                else
                {
                    possible[i] = false;
                }
            }
        }
    }

    Cell() = default;

    //index 1
    int GetVal()
    {
        int count = 0;
        int res = 0;
        for (int i = 0; i < 9; i++)
        {
            if (possible[i])
            {
                count++;
                res = i;
            }
        }

        //multiple solutions
        if (count > 1)
            return 0;

        //no solutions
        if (count == 0)
            return -1;

        //1 solutions
        return res + 1;
    }

    bool IsSolved()
    {
        int c = 0;
        for (int i = 0; i < 9; i++)
        {
            if (possible[i])
            {
                c++;
                if (c > 1)
                {
                    return false;
                }
            }
        }
        if (c == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //0 -> imp 1-> solved n -> incomplete
    int GetNumberOfPos()
    {
        int res = 0;
        for (int i = 0; i < 9; i++)
        {
            if (possible[i])
            {
                res++;
            }
        }
        return res;
    }

    //index 1
    void RemovePossibility(int n)
    {
        possible[n - 1] = false;
    }
};