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

	// Allocate the output data array (2 * inSize for safety)
	mData = new char[2 * inSize];

	int runLength = 0; // stores the number of runLengthutive characters
	for (unsigned int i = 0; i < inSize; ++i)
	{
		if (i + 1 < inSize && input[i] == input[i + 1])
		{
			runLength++;
			while (i + 1 < inSize && input[i] == input[i + 1])
			{
				runLength++;
				i++;
			}
			CompressPositiveRun(input, mSize, runLength, i);
		}
		else if (i + 1 < inSize && input[i] != input[i + 1])
		{
			runLength++;
			// inSize - 1 == i + 1 handles the last character of the input string
			while (i + 2 < inSize && input[i + 2] != input[i + 1] || inSize - 1 == i + 1)
			{
				runLength++;
				i++;
			}
			// handles single character run
			if (runLength != 1)
			{
				runLength = -runLength;
			}
			CompressNegativeRun(input, mSize, runLength, i);
		}
		// handle single character run at the end
		else if (runLength == 0)
		{
			CompressPositiveRun(input, mSize, ++runLength, i);
		}
		runLength = 0;
	}
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	// Clearup the class array
	delete[] mData;
	mSize = 0;

	// Allocate the output data array
	mData = new char[outSize];
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
				mData[mSize++] = input[i];
		}
		else // Negative runs
		{
			numOccurences = abs(input[i]);
			for (unsigned int k = 0; k < numOccurences; ++k)
				mData[mSize++] = input[++i];
		}
		++i;
	}
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
	// while loop handles cases longer than 127 characters
	while (runLength > 0)
	{
		if (runLength > 127)
		{
			mData[compressedPosition++] = 127;
		}
		else
		{
			mData[compressedPosition++] = runLength;
		}

		mData[compressedPosition++] = input[pos];
		runLength -= 127;
	}
}

void RleData::CompressNegativeRun(const char* input, int &compressedPosition, int &runLength, const int pos)
{
	unsigned int backwardsCounter = abs(runLength); // used to copy the data to the buffer
	// Single character run
	if (runLength == 1)
	{
		mData[compressedPosition++] = runLength;
		mData[compressedPosition++] = input[pos];
	}
	// while loop handles cases longer than 127 characters
	while (runLength < 0)
	{
		if (runLength < -127)
		{
			mData[compressedPosition++] = -127;
		}
		else
		{
			mData[compressedPosition++] = runLength;
		}

		for (int k = abs(std::max(runLength, -127)); k > 0; --k)
		{
			mData[compressedPosition++] = input[pos + 1 - backwardsCounter--];
		}

		runLength += 127;
	}
}