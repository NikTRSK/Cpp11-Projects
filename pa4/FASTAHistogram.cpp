#include "FASTAHistogram.h"



FASTAHistogram::FASTAHistogram(std::shared_ptr<FASTA> FASTAFile)
{
	mFASTAFile = FASTAFile;

	std::string FASTAData = mFASTAFile->GetData();

	mTotalAminoCount = 0;
	unsigned int state = 0,
				  base = 0;
	for (unsigned int i = 0; i < FASTAData.size(); ++i)
	{
		// 0: T, 1: C, 2: A, 3: G
		// Get the index in the state
		base = BaseToIdx(FASTAData[i]);
//		if (i == 66)
//			std::cout << FASTAData[i];
//		std::cout << state << ", " << base << " | " << FASTAData[i] << " | " << mCodonTable[state][base] << std::endl;
		if (mCodonTable[state][base] > 24)
		{
			auto & amino = mCodonFrequencies.find(mCodonTable[state][base]);
			if (amino != mCodonFrequencies.end())
			{
				++(amino->second);
				++mTotalAminoCount;
			}
			// Stop codon go to 0
			// else state 3
			state = 3;
		}
		else
		{
			state = mCodonTable[state][base];
		}

//		for (auto const & item : mCodonFrequencies) std::cout << item.first << ": " << item.second << std::endl;
//		base = BaseToIdx(FASTAData[i]);
	}
}


FASTAHistogram::~FASTAHistogram()
{
}

//const auto & FASTAHistogram::GetFrequencyTable() const
//{
//	return mCodonFrequencies;
//}
//
//const auto & FASTAHistogram::GetTotalAminoCount() const
//{
//	return mTotalAminoCount;
//}

//const std::unordered_map<char, std::string> & FASTAHistogram::GetCodeToNameMap() const
//{
//	return mCodeToFullNameMap;
//}

char FASTAHistogram::BaseToIdx(char c)
{
	switch (c)
	{
		case 'T':
			return 0;
		case 'C':
			return 1;
		case 'A':
			return 2;
		case 'G':
			return 3;
		default:
			return -1;
	}
}
