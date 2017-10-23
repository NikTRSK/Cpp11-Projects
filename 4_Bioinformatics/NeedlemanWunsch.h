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
	void PopulateMatrix();
	void Backtrack();

	void PrintMatrix();
	void PrintDirectionMatrix();

	void WriteResults();

private:
	enum Direction : char
	{
		DIAGONAL, // 0
		LEFT, // 1
		UP // 2
	};

	std::vector<std::vector<short> > mMatrix;
	std::vector<std::vector<Direction> > mDirections;

	std::shared_ptr<FASTA> mInputFileA;
	std::shared_ptr<FASTA> mInputFileB;
	std::string mOutputFile;

	unsigned int mRows;
	unsigned int mCols;

	std::string mFileAData;
	std::string mFileBData;

	std::string mResultingSequenceA;
	std::string mResultingSequenceB;

	short Max(const unsigned int& row, const unsigned int& col);
};

