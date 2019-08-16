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
	Sudoku();
	virtual ~Sudoku() = default;
	void CreateSeed();
	void PrintGrid();
	bool SolveGrid();
	void CountSoln(int& number);
	void GenPuzzle();
	bool FindUnassignedLocation(int grid[9][9], int& row, int& col);
	int GetGrid(int row, int col);
	int GetSolutionGrid(int row, int col);
private:
	static const int UNASSIGNED = 0;
	int mGrid[9][9];
	int mSolnGrid[9][9];
	int mGuessNum[9];
	int mGridPos[81];
};


