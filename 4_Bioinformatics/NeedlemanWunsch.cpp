#include "NeedlemanWunsch.h"
#include "Exceptions.h"
#include <algorithm>
#include <iomanip>

NeedlemanWunsch::NeedlemanWunsch(std::string inFileA, std::string inFileB,  std::string outFile)
{
	try
	{
		mInputFileA = std::make_shared<FASTA>(inFileA.c_str());
		mInputFileB = std::make_shared<FASTA>(inFileB.c_str());
		mOutputFile = outFile;

		mFileAData = mInputFileA->GetData();
		mFileBData = mInputFileB->GetData();
	}
	catch (FileLoadExcept& fle)
	{
		std::cerr << fle.what();
		throw FileLoadExcept();
	}
}


NeedlemanWunsch::~NeedlemanWunsch()
{
}

void NeedlemanWunsch::RunAlgorithm()
{
	InitMatrix();
	PopulateMatrix();
	Backtrack();
	WriteResults();
}

void NeedlemanWunsch::InitMatrix()
{
	// TODO: Add a table for the enums
	mRows = mInputFileB->GetData().length() + 1,
	mCols = mInputFileA->GetData().length() + 1;
	std::string FileAData = mInputFileA->GetData();
	std::string FileBData = mInputFileB->GetData();
	mMatrix.resize(mRows);
	mDirections.resize(mRows);
	
	for (unsigned int row = 0; row < mRows; ++row)
	{
		mMatrix[row].resize(mCols);
		mDirections[row].resize(mCols);
	}

	int score = 0;
	mMatrix[0][0] = 0;
	// Populate FileA
	for (unsigned int col = 1; col < mCols; ++col)
	{
		mMatrix[0][col] = --score;
		mDirections[0][col] = LEFT;
	}
	score = 0;
	// Populate FileB
	for (unsigned int row = 1; row < mRows; ++row)
	{
		mMatrix[row][0] = --score;
		mDirections[row][0] = UP;
	}
}

void NeedlemanWunsch::PrintMatrix()
{
	for (unsigned int i = 0; i < mRows; ++i)
	{
		for (unsigned int j = 0; j < mCols; ++j)
		{
			std::cout << std::setw(3) << (mMatrix[i][j]) << std::setw(0) << "  |  ";
		}
		std::cout << std::endl;
	}
}

void NeedlemanWunsch::PrintDirectionMatrix()
{
	for (unsigned int i = 0; i < mRows; ++i)
	{
		for (unsigned int j = 0; j < mCols; ++j)
		{
			if (mDirections[i][j] == DIAGONAL)	std::cout << ("D") << "  |  ";
			else if (mDirections[i][j] == LEFT)	std::cout << ("L") << "  |  ";
			else if (mDirections[i][j] == UP)	std::cout << ("U") << "  |  ";
		}
		std::cout << std::endl;
	}
}

void NeedlemanWunsch::PopulateMatrix()
{
	// Define starting point
	for (unsigned int row = 1; row < mRows; ++row)
	{
		for (unsigned int col = 1; col < mCols; ++col)
		{
			mMatrix[row][col] = Max(row, col);
		}
	}
}

void NeedlemanWunsch::Backtrack()
{
	int row = mRows - 1,
		col = mCols - 1;

	mFileAData.insert(0, "_");
	mFileBData.insert(0, "_");
	while (row > 0 || col > 0)
	{
		if (mDirections[row][col] == DIAGONAL)
		{
			mResultingSequenceA.push_back(mFileAData[col]);
			mResultingSequenceB.push_back(mFileBData[row]);
			--row;
			--col;
		}
		else if (mDirections[row][col] == LEFT)
		{
			mResultingSequenceA.push_back(mFileAData[col]);
			mResultingSequenceB.push_back('_');
			--col;
		}
		else if (mDirections[row][col] == UP)
		{
			mResultingSequenceA.push_back('_');
			mResultingSequenceB.push_back(mFileBData[row]);
			--row;
		}
	}

	std::reverse(mResultingSequenceA.begin(), mResultingSequenceA.end());
	std::reverse(mResultingSequenceB.begin(), mResultingSequenceB.end());
}

void NeedlemanWunsch::WriteResults()
{
	std::ofstream out(mOutputFile, std::ios::out | std::ios::trunc);
	if (out.is_open())
	{
		// Write the data
		out << "A: " << mInputFileA->GetHeader() << "\n";
		out << "B: " << mInputFileB->GetHeader() << "\n";
		out << "Score: " << mMatrix[mRows - 1][mCols - 1] << "\n\n";

		int totalLength = mResultingSequenceA.length();
		int subsequenceLen, substrStart = 0;
		while (totalLength > 0)
		{
			subsequenceLen = (totalLength > 70) ? 70 : totalLength;

			out << mResultingSequenceA.substr(substrStart, subsequenceLen) << "\n";
			for (int i = 0; i < subsequenceLen; ++i)
			{
				if (mResultingSequenceA[substrStart + i] == mResultingSequenceB[substrStart + i])
				{
					out << "|";
				}
				else
				{
					out << " ";
				}
			}
			out << "\n";
			out << mResultingSequenceB.substr(substrStart, subsequenceLen) << "\n\n";
			substrStart += subsequenceLen;
			totalLength -= subsequenceLen;
		}
		out.close();
	}
}

short NeedlemanWunsch::Max(const unsigned int& row, const unsigned int& col)
{
	// Sub 1 since we start at 1 but the strings are 0 based
	char S = (mFileAData[col - 1] == mFileBData[row - 1]) ? 1 : -1;

	short weights[] = {
		mMatrix[row - 1][col - 1] + S, // Diagonal
		mMatrix[row][col - 1] - 1,	   // Left
		mMatrix[row - 1][col] - 1	   // Up
	};

	auto maxElemIdx = std::max_element(weights, weights+3);
	
	mDirections[row][col] = static_cast<Direction>(std::distance(weights, maxElemIdx));
		
	return *maxElemIdx;
}
