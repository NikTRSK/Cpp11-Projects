#include "RleData.h"
#include <iostream>
#include <stdlib.h>

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


	// Check for array size
	unsigned int instanceSize = inSize;
	unsigned int charsLeft = inSize;

	int compressedPosition = 0;
	int conseq = 0; // stores the number of consequtive characters
	int nonConseq = 0;
	unsigned int i;
	for (unsigned int run = 0; run < ceil((double)inSize / MaxRunSize()); ++run)
	{
		if (charsLeft > MaxRunSize())
		{
			charsLeft -= 127;
			instanceSize = 127;
		}
		else
		{
			instanceSize = charsLeft;
			charsLeft = 0;
		}

		for (i = 0; i < instanceSize; ++i)
		{
			if (i + 1 < instanceSize && input[i] == input[i + 1])
				conseq++;
			else if (i + 1 < instanceSize && input[i] != input[i + 1])
			{
				if (i + 2 < instanceSize && input[i + 2] != input[i + 1] || instanceSize - 1 == i + 1)
					nonConseq--;
			}

			if (i + 1 < instanceSize && input[i] == input[i + 1] && nonConseq < 0) {
				conseq = 1;
				CompressNegativeRun(input, compressedPosition, nonConseq, i);
			}
			else if (i + 1 < instanceSize && input[i] != input[i + 1] && conseq > 0) {
				nonConseq = 0;
				CompressPositiveRun(input, compressedPosition, conseq, i);
			}
		}

		conseq++;
		nonConseq--;

		if (nonConseq < -1) {
			conseq = 0;
			mData[compressedPosition++] = nonConseq;
			for (int k = abs(nonConseq); k > 0; --k)
			{
				mData[compressedPosition++] = input[i - k];
				std::cout << input[k] << std::endl;
			}

			std::cout << "NONCONSEQ: " << nonConseq << std::endl;
			nonConseq = 0;
		}
		else if (conseq > 1) {
			nonConseq = 0;
			mData[compressedPosition++] = conseq;
			mData[compressedPosition++] = input[instanceSize - 1];
			conseq = 0;
		}
	}

	std::cout << "NUM: " << compressedPosition << "|  " << mData << std::endl;
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
	//std::cout << "CONSEQ: " << conseq << std::endl;
	mData[compressedPosition++] = runLength;
	mData[compressedPosition++] = input[pos];
	runLength = 0;
}

void RleData::CompressNegativeRun(const char* input, int &compressedPosition, int &runLength, const int pos)
{
	runLength--;
	//std::cout << "NONCONSEQ: " << nonConseq << std::endl;
	mData[compressedPosition++] = runLength;
	for (int k = abs(runLength); k > 0; --k)
		mData[compressedPosition++] = input[pos - k];
	runLength = 0;
}