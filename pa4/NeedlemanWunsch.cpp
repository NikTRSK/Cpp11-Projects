#include "NeedlemanWunsch.h"
#include "Exceptions.h"

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
	unsigned int rows = mInputFileB->GetData().length(),
				 cols = mInputFileA->GetData().length();
	std::string FileAData = mInputFileA->GetData();
	std::string FileBData = mInputFileB->GetData();
	mMatrix.reserve(rows + 1);
	int score = 0;
	for (unsigned int row = 0; row < rows; ++row)
	{
		mMatrix[row].reserve(cols + 1);
	}

	mMatrix[0][0] = 0;
	// Populate FileA
	for (unsigned int col = 0; col < cols; ++col)
	{
		mMatrix[0][col + 1] = FileAData[col];
		mMatrix[1][col + 1] = --score;
	}
	score = 0;
	// Populate FileB
	for (unsigned int row = 0; row < rows; ++row)
	{
		mMatrix[row + 1][0] = FileBData[row];
		mMatrix[row + 1][1] = --score;
	}
}

void NeedlemanWunsch::PrintMatrix()
{
	for (unsigned int i = 0; i < mInputFileA->GetData().length(); ++i)
	{
		for (unsigned int j = 0; j < mInputFileB->GetData().length(); ++j)
		{
			std::cout << mMatrix[i][j] << "  |  ";
		}
		std::cout << std::endl;
	}
}

void NeedlemanWunsch::WriteResults()
{
}
