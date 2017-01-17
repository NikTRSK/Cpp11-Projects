#include "RleData.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

void RleData::Compress(const char* input, size_t inSize)
{
	// what happens if the user passes in a negative value for the size

	// TODO
	// Clearup the class array
	delete[] mData;
	mSize = 0;

	// Allocate the output data array (2*inSize for safety)
	mData = new char[2 * inSize];
	mSize = 2 * inSize;

	// Base case: len = 1
	if (inSize == 1) {
		mData[0] = 1;
		mData[1] = input[0];
	}

	int compressedPosition = 0;
	int conseq = 0; // stores the number of consequtive characters
	int nonConseq = 0;
	unsigned int i;

	for (i = 0; i < inSize; ++i)
	{
		if (i + 1 < inSize && input[i] == input[i + 1])
		{
			conseq++;
		}
		else if (i + 1 < inSize && input[i] != input[i + 1])
		{
			if (i + 2 < inSize && input[i + 2] != input[i + 1] || inSize - 1 == i + 1)
				nonConseq--;
		}

		if (i + 1 < inSize && input[i] == input[i + 1] && nonConseq < 0) {
			conseq = 1;
			CompressNegativeRun(input, compressedPosition, nonConseq, i);
		}
		else if (i + 1 < inSize && input[i] != input[i + 1] && conseq > 0) {
			nonConseq = 0;
			CompressPositiveRun(input, compressedPosition, conseq, i);
		}
	}

	conseq++;
	nonConseq--;

	if (nonConseq < -1) {
		conseq = 0;
		// edge case: when at the end of array don't need to subtract so add 1
		CompressNegativeRun(input, compressedPosition, ++nonConseq, i);
		nonConseq = 0;
	}
	else if (conseq > 1)
	{
		nonConseq = 0;
		// edge case: when at the end of array don't need to add so subtract 1
		CompressPositiveRun(input, compressedPosition, --conseq, i-1); // i-1 to account for the end of the array
	}

	//std::cout << "NUM: " << compressedPosition << "|  " << mData << std::endl;
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// TODO
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}

void RleData::CompressPositiveRun(const char* input, int &compressedPosition, int &runLength, const int pos)
{
	runLength++;
	//std::cout << "CONSEQ: " << runLength << std::endl;
	while (runLength > 0)
	{
		//std::cout << "CONSEQ: " << runLength << std::endl;
		if (runLength > 127)
			mData[compressedPosition++] = 127;
		else
			mData[compressedPosition++] = runLength;

		mData[compressedPosition++] = input[pos];
		runLength -= 127;
	}
	runLength = 0;
}

void RleData::CompressNegativeRun(const char* input, int &compressedPosition, int &runLength, const int pos)
{
	runLength--;
	/*std::cout << "NONCONSEQ: " << runLength << std::endl;*/
	unsigned int backwardsCounter = abs(runLength);
	while (runLength < 0)
	{
		//std::cout << "NONCONSEQ: " << runLength << std::endl;
		if (runLength < -127)
			mData[compressedPosition++] = -127;
		else
			mData[compressedPosition++] = runLength;
		
		for (int k = abs(std::max(runLength, -127)); k > 0; --k)
			mData[compressedPosition++] = input[pos - backwardsCounter--];

		runLength += 127;
	}
	runLength = 0;
}