#include "Sudoku.h"


// START: Intialising
Sudoku::Sudoku()
{

	// Randomly shuffling the array of removing mGrid positions
	for (int i = 0; i < 81; i++)
	{
		mGridPos[i] = i;
	}

	std::random_shuffle(mGridPos, mGridPos + 81, GenRandNum);

	// Randomly shuffling the guessing number array
	for (int i = 0; i < 9; i++)
	{
		mGuessNum[i] = i + 1;
	}

	std::random_shuffle(mGuessNum, mGuessNum + 9, GenRandNum);

	// Initialising the mGrid
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mGrid[i][j] = 0;
		}
	}

}
// END: Initialising

// START: Create seed mGrid
void Sudoku::CreateSeed()
{
	SolveGrid();

	// Saving the solution mGrid
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			mSolnGrid[i][j] = mGrid[i][j];
		}
	}
}
// END: Create seed mGrid


// START: Printing the mGrid
void Sudoku::PrintGrid()
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (mGrid[i][j] == 0)
			{
				std::cout << ".";
			}
			else
			{
				std::cout << mGrid[i][j];
			}
			std::cout << "|";
		}
		std::cout << std::endl;
	}

}
// END: Printing the mGrid


// START: Helper functions for solving mGrid
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

// START: Modified and improved Sudoku solver
bool Sudoku::SolveGrid()
{
	int row, col;

	// If there is no unassigned location, we are done
	if (!FindUnassignedLocation(mGrid, row, col))
	{
		return true; // success!
	}

	// Consider digits 1 to 9
	for (int num = 0; num < 9; num++)
	{
		// if looks promising
		if (IsSafe(mGrid, row, col, mGuessNum[num]))
		{
			// make tentative assignment
			mGrid[row][col] = mGuessNum[num];

			// return, if success, yay!
			if (SolveGrid())
			{
				return true;
			}

			// failure, unmake & try again
			mGrid[row][col] = UNASSIGNED;
		}
	}

	return false; // this triggers backtracking

}
// END: Modified and improved Sudoku Solver


// START: Check if the mGrid is uniquely solvable
void Sudoku::CountSoln(int& number)
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
			CountSoln(number);
		}

		mGrid[row][col] = UNASSIGNED;
	}

}
// END: Check if the mGrid is uniquely solvable


// START: Gneerate puzzle
void Sudoku::GenPuzzle()
{
	for (int i = 0; i < 81; i++)
	{
		int x = mGridPos[i] / 9;
		int y = mGridPos[i] % 9;
		int temp = mGrid[x][y];
		mGrid[x][y] = UNASSIGNED;

		// If now more than 1 solution , replace the removed cell back.
		int check = 0;
		CountSoln(check);
		if (check != 1)
		{
			mGrid[x][y] = temp;
		}
	}
	std::cout << std::endl;
}
// END: Generate puzzle