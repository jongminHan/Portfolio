#include <iostream>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "HelperFunctions.h"

class Sudoku
{
public:
	// 스도쿠 생성
	Sudoku();

	virtual ~Sudoku() = default;

	// 스도쿠 seed 생성
	void CreateSeed();

	// 스도쿠를 푸는 함수. 재귀함수. Backtracking 기법 사용.
	bool SolveGridRecursive();

	// mGrid의 해법이 유일한지 확인하는 함수. 재귀함수.
	void CountSolnRecursive(int& number);

	// 퍼즐 생성
	void GenPuzzle();

	// 어떠한 숫자도 할당되지 않은 위치를 탐색
	bool FindUnassignedLocation(int grid[9][9], int& row, int& col);

	// 특정 위치의 숫자를 반환
	int GetGrid(int row, int col);

	// 특정 위치의 솔루션을 반환
	int GetSolutionGrid(int row, int col);
private:
	static const int UNASSIGNED = 0;  // 할당되지 않았을 경우 0을 입력
	int mGrid[9][9];     // 스도쿠 격자
	int mSolnGrid[9][9]; // 해법
	int mGuessNum[9]; // 1에서부터 9까지의 추정 숫자 저장
	int mGridPos[81]; // 격자 위치 저장
};


