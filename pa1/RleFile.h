#pragma once
#include "RleData.h"
#include <string>
#include <list>

class RleFile
{
	struct header
	{
		char sig[4];	// RLE file signature
		int fileSize;	// Originial file size (before compression)
		unsigned char fileNameLength;	// Length of the output filename
		std::string fileName;	// The name of the output file
	} mHeader;

	RleData mData;

public:
	void CreateArchive(const std::string& source);

	void ExtractArchive(const std::string& source);
};
