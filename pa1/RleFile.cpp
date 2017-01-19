#include "RLEFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include "FileSystem.h"

void RleFile::CreateArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock; // Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		mHeader.fileSize = (int)size;
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
		// File data has now been loaded into memblock array
		// Compress the data
		std::cout << "SIZE: " << sizeof(memblock) << ", " << size << std::endl;
		std::cout << "DATA: " << memblock << std::endl;
		mData.Compress(memblock, (int)size);

		std::cout << "Compression level: " << sizeof(mData.mSize)*100/size << "%\n";
		mHeader.sig[0] = 'R'; mHeader.sig[1] = 'L'; mHeader.sig[2] = 'E'; mHeader.sig[3] = '\x01';
		
		// Output the compressed data to a file
		mHeader.fileName = source + ".rl1";
		mHeader.fileNameLength = (char)(mHeader.fileName.length());
		// Open the file for output, in binary mode, and overwrite an existing file std::ofstream arc(filename, std::ios::out|std::ios::binary|std::ios::trunc);
		std::ofstream arc(mHeader.fileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Setup the header
			arc.write(mHeader.sig, 4);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileSize)), 4);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileNameLength)), 4);
			arc.write(mHeader.fileName.c_str(), mHeader.fileNameLength);

			// Write the data
			std::cout << "mSize: " << mData.mSize << ", " << mData.mData << "\n" ;
			arc.write(mData.mData, sizeof(mData.mData)); // Should this just be mSize?
		}

		// Make sure to clean up!
		delete[] memblock;
	}
}

void RleFile::ExtractArchive(const std::string& source)
{
	// TODO
}
