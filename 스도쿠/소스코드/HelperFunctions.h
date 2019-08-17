#pragma once
#include <cstdlib>

int GenRandNum(int maxLimit);
bool UsedInRow(int grid[9][9], int row, int num);
bool UsedInCol(int grid[9][9], int col, int num);
bool UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num);
bool IsSafe(int grid[9][9], int row, int col, int num);