#include "RleData.h"

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
//	if (inSize == 1)
//		mData = "\x01" + input[0];

	// TODO: Check for array size
/*	int compressedPosition = 0; 
	int conseq = 0; // stores the number of consequtive characters
	for (int i = 0; i < inSize; ++i)
	{
		conseq++;

		if (i + 1 >= inSize || input[i] != input[i + 1])
		{

			//mData[compressedPosition] = 
		}
	}*/
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