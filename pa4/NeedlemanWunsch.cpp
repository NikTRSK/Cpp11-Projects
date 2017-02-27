#include "NeedlemanWunsch.h"
#include "Exceptions.h"
#include <algorithm>

NeedlemanWunsch::NeedlemanWunsch(std::string inFileA, std::string inFileB,  std::string outFile)
{
	try
	{
		mInputFileA = std::make_shared<FASTA>(inFileA.c_str());
		mInputFileB = std::make_shared<FASTA>(inFileB.c_str());
		mOutputFile = outFile;
	}
	catch (FileLoadExcept& fle)
	{
		// TODO: handle the exception
		std::cout << fle.what();
	}
}


NeedlemanWunsch::~NeedlemanWunsch()
{
}

void NeedlemanWunsch::RunAlgorithm()
{
	InitMatrix();
	PrintMatrix();
}

void NeedlemanWunsch::InitMatrix()
{
	// TODO: Add a table for the enums
	mRows = mInputFileB->GetData().length() + 1,
	mCols = mInputFileA->GetData().length() + 1;
	std::string FileAData = mInputFileA->GetData();
	std::string FileBData = mInputFileB->GetData();
	mMatrix.resize(mRows + 1);
	int score = 0;
	for (unsigned int row = 0; row <= mRows; ++row)
	{
		mMatrix[row].resize(mCols + 1);
	}

	mMatrix[0][0] = 0;
	mMatrix[1][1] = 0;
	// Populate FileA
	for (unsigned int col = 1; col < mCols; ++col)
	{
		std::cout << FileAData[col - 1] << std::endl;
		mMatrix[0][col + 1] = FileAData[col-1];
		mMatrix[1][col + 1] = --score;
	}
	score = 0;
	// Populate FileB
	for (unsigned int row = 1; row < mRows; ++row)
	{
		std::cout << FileBData[row-1] << std::endl;
		mMatrix[row + 1][0] = FileBData[row-1];
		mMatrix[row + 1][1] = --score;
	}
}

void NeedlemanWunsch::PrintMatrix()
{
	for (unsigned int i = 0; i <= mRows; ++i)
	{
		for (unsigned int j = 0; j <= mCols; ++j)
		{
			if (i == 0 || j == 0)
			{
				std::cout << static_cast<char>(mMatrix[i][j]) << "  |  ";
			}
			else
				std::cout << (mMatrix[i][j]) << "  |  ";
		}
		std::cout << std::endl;
	}
}

void NeedlemanWunsch::PopulateMatrix()
{
	// Define starting point
	unsigned int row = 2, col = 2;
	for (row; row <= mRows; ++row)
	{
		mMatrix[row][col] = Max(row, col);
	}
}

void NeedlemanWunsch::WriteResults()
{
}

short NeedlemanWunsch::Max(const unsigned int& row, const unsigned int& col)
{
	short up = mMatrix[row - 1][col] -1;
	short down = mMatrix[row][col - 1] -1;
	short diag = mMatrix[row - 1][col - 1] + mMatrix[row][col];
	return std::max(diag, std::max(up, down));
}
