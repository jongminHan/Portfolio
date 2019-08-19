#pragma once
#include <cstdlib>
// 전역 함수들 모음

// 0이상 maxLimit 미만의 숫자를 무작위로 반환
int GenRandNum(int maxLimit);

// 행에 같은 숫자가 있을 경우 true 반환
bool UsedInRow(int grid[9][9], int row, int num);

// 열에 같은 숫자가 있을 경우 true 반환
bool UsedInCol(int grid[9][9], int col, int num);

// 3 x 3 상자에 같은 숫자가 있을 경우 true 반환
bool UsedInBox(int grid[9][9], int boxStartRow, int boxStartCol, int num);

// 행, 열, 상자 모두 같은 숫자가 없을 경우 true 반환
bool IsSafe(int grid[9][9], int row, int col, int num);