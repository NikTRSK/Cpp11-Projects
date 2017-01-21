#include "RleData.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>

void RleData::Compress(const char* input, size_t inSize)
{
	// what happens if the user passes in a negative value for the size

	// Clearup the class array
	delete[] mData;
	mSize = 0;

	// Allocate the output data array (2*inSize for safety)
	mData = new char[2 * inSize];
	//mSize = 2 * inSize;

	// Base case: len = 1
	if (inSize == 1) {
		mData[0] = 1;
		mData[1] = input[0];
	}

	int compressedPosition = 0;
	int conseq = 0; // stores the number of consequtive characters
	//unsigned int i;
	//std::cout << "!_NUM: " << inSize << "|  " << mData << std::endl;
	for (unsigned int i = 0; i < inSize; ++i)
	{
		if (i + 1 < inSize && input[i] == input[i + 1])
		{
			conseq++;
			while (i + 1 < inSize && input[i] == input[i + 1])
			{
				conseq++;
				i++;
			}
			CompressPositiveRun(input, compressedPosition, conseq, i);
		}
		else if (i + 1 < inSize && input[i] != input[i + 1])
		{
			conseq++;
			// inSize - 1 == i + 1 handles the last character of the input string
			while (i + 2 < inSize && input[i + 2] != input[i + 1] || inSize - 1 == i + 1)
			{
				conseq++;
				i++;
			}
			if (conseq != 1)
				conseq = -conseq;
			CompressNegativeRun(input, compressedPosition, conseq, i);
		}
		// handle single character run at the end
		else if (conseq == 0)
		{
			CompressPositiveRun(input, compressedPosition, ++conseq, i);
		}
	}

	mSize = compressedPosition;
	//std::cout << "NUM: " << compressedPosition << "|  " << mData << std::endl;
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// Clearup the class array
	delete[] mData;
	mSize = 0;

	//std::cout << "OUTPUT: " << input[0] << ", " << input[1] << std::endl;
	// Allocate the output data array (2*inSize for safety)
	mData = new char[2 * outSize];
	mSize = outSize;

	unsigned int position = 0;
	unsigned int i = 0;
	unsigned int numOccurences = 0;
	while(i < inSize)
	{
		// Positive runs
		if (input[i] > 0)
		{
			numOccurences = input[i];
			i++;
			for (unsigned int k = 0; k < numOccurences; ++k)
				mData[position++] = input[i];
		}
		else // Negative runs
		{
			numOccurences = abs(input[i]);
			for (unsigned int k = 0; k < numOccurences; ++k)
				mData[position++] = input[++i];
		}
		++i;
	}

	//std::cout << "OUTSIZE: " << outSize << " |pos: " << position << " |OUTPUT: " << mData << std::endl;
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
	//runLength++;
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
	//runLength--;
	//std::cout << "NONCONSEQ: " << runLength << std::endl;
	unsigned int backwardsCounter = abs(runLength);
	//while (runLength < 0)
	if (runLength == 1)
	{
		mData[compressedPosition++] = runLength;
		mData[compressedPosition++] = input[pos];
	}
	while (runLength < 0)
	{
		//std::cout << "NONCONSEQ: " << runLength << std::endl;
		if (runLength < -127)
			mData[compressedPosition++] = -127;
		else
			mData[compressedPosition++] = runLength;
		
		for (int k = abs(std::max(runLength, -127)); k > 0; --k)
		{
			//std::cout << "i: " << pos +1 - backwardsCounter << std::endl;
			mData[compressedPosition++] = input[pos + 1 - backwardsCounter--];
		}

		runLength += 127;
	}
	runLength = 0;
}