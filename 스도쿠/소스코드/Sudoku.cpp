#include "Sudoku.h"

Sudoku::Sudoku()
{
	// 81개의 격자 위치를 무작위로 섞음
	for (int i = 0; i < 81; i++)
	{
		mGridPos[i] = i;
	}
	std::random_shuffle(mGridPos, mGridPos + 81, GenRandNum);

	// 1에서부터 9까지의 숫자를 무작위로 섞음
	for (int i = 0; i < 9; i++)
	{
		mGuessNum[i] = i + 1;
	}
	std::random_shuffle(mGuessNum, mGuessNum + 9, GenRandNum);

	// 격자를 초기화
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mGrid[i][j] = 0;
		}
	}

}

void Sudoku::CreateSeed()
{
	SolveGridRecursive(); // 꽉찬 스도쿠를 생성

	// 꽉찬 스도쿠를 솔루션에 저장
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mSolnGrid[i][j] = mGrid[i][j];
		}
	}
}

bool Sudoku::FindUnassignedLocation(int grid[9][9], int& row, int& col)
{
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			if (grid[row][col] == UNASSIGNED)
			{
				return true;
			}
		}
	}

	return false;
}

int Sudoku::GetGrid(int row, int col)
{
	return mGrid[row][col];
}

int Sudoku::GetSolutionGrid(int row, int col)
{
	return mSolnGrid[row][col];
}

bool Sudoku::SolveGridRecursive()
{
	int row, col;

	// 비어있는 공간이 없으면 스도쿠 풀기 완료.
	if (!FindUnassignedLocation(mGrid, row, col))
	{
		return true; // 성공!
	}

	// 1에서부터 9까지의 숫자를 하나하나 대입
	for (int num = 0; num < 9; num++)
	{
		// 입력한 숫자가 '일단' 안전해보일 경우
		if (IsSafe(mGrid, row, col, mGuessNum[num]))
		{
			// 잠정적인 대입
			mGrid[row][col] = mGuessNum[num];

			// 올바른 해법이라면 true 반환
			if (SolveGridRecursive()) // 재귀
			{
				return true;
			}

			// 올바른 해법이 아니라면, 해당 숫자를 제거하고 다시 시도
			mGrid[row][col] = UNASSIGNED;
		}
	}

	return false; // backtracking을 유도하는 지점
}

void Sudoku::CountSolnRecursive(int& number)
{
	int row, col;

	if (!FindUnassignedLocation(mGrid, row, col))
	{
		number++;
		return;
	}


	for (int i = 0; i < 9 && number < 2; i++)
	{
		if (IsSafe(mGrid, row, col, mGuessNum[i]))
		{
			mGrid[row][col] = mGuessNum[i];
			CountSolnRecursive(number);
		}

		mGrid[row][col] = UNASSIGNED;
	}
}

void Sudoku::GenPuzzle()
{
	for (int i = 0; i < 81; i++)
	{
		int x = mGridPos[i] / 9;
		int y = mGridPos[i] % 9;
		int temp = mGrid[x][y];  // x행, y열의 숫자를 temp에 저장
		mGrid[x][y] = UNASSIGNED; // x행, y열을 unassign한다.

		// 2개 이상의  해법이 있을 경우, temp를 재입력
		int check = 0;
		CountSolnRecursive(check);
		if (check != 1)
		{
			mGrid[x][y] = temp;
		}
	}
}