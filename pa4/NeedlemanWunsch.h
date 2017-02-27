#pragma once
#include "FASTA.h"
#include <memory>

class NeedlemanWunsch
{
public:
	NeedlemanWunsch(std::string inFileA, std::string inFileB, std::string outFile);
	~NeedlemanWunsch();
	void RunAlgorithm();
	void InitMatrix();
	void PrintMatrix();
	void PopulateMatrix();

	void WriteResults();

private:
	std::vector<std::vector<short> > mMatrix;

	std::shared_ptr<FASTA> mInputFileA;
	std::shared_ptr<FASTA> mInputFileB;
	std::string mOutputFile;

	unsigned int mRows;
	unsigned int mCols;

	enum Direction : char
	{
		UP,
		LEFT,
		DIAGONAL
	};

	short Max(const unsigned int& row, const unsigned int& col);
};

