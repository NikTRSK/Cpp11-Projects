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
	}
		// File data has now been loaded into memblock array
		// Compress the data
		std::cout << "SIZE: " << size << std::endl;
		std::cout << "DATA: " << memblock << std::endl;
		mData.Compress(memblock, (int)size);

		std::cout << "Compression level: " << sizeof(mData.mSize)*100/size << "%\n";
		mHeader.sig[0] = 'R'; mHeader.sig[1] = 'L'; mHeader.sig[2] = 'E'; mHeader.sig[3] = '\x01';
		
		// Output the compressed data to a file
		mHeader.fileName = source + ".rl1";
		mHeader.fileNameLength = (char)(source.length());
		// Open the file for output, in binary mode, and overwrite an existing file std::ofstream arc(filename, std::ios::out|std::ios::binary|std::ios::trunc);
		std::ofstream arc(mHeader.fileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (arc.is_open())
		{
			// Setup the header
			arc.write(mHeader.sig, 4);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileSize)), 4);
			arc.write(reinterpret_cast<char*>(&(mHeader.fileNameLength)), 1);
			arc.write(source.c_str(), mHeader.fileNameLength);

			// Write the data
			std::cout << "mSize: " << mData.mSize << ", " << mData.mData << "\n" ;
			arc.write(mData.mData, mData.mSize); // Should this just be mSize?
			arc.close();
		}

		// Make sure to clean up!
		delete[] memblock;
	}

void RleFile::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock; // Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		 //mHeader.fileSize = (int)size;
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
	}
		// READ IN HEADER
		// Check for File Signature
		if (size < 4)
		{
			std::cout << "Wrong file format\n";
			return;
		}
		
		if (memblock[0] != 'R' || memblock[1] != 'L' || memblock[2] != 'E' || memblock[3] != '\x01')
		{
			std::cout << "Wrong file format. Non existent signature.\n";
		}
		mHeader.fileSize = *(reinterpret_cast<int*>(&memblock[4])); // Get the original file size
		mHeader.fileNameLength = *(reinterpret_cast<unsigned char*>(&memblock[8])); // Get the filename length
		//mHeader.fileName.copy(memblock, mHeader.fileNameLength, 9); // Get the filename
		for (unsigned int i = 0; i < mHeader.fileNameLength; ++i)
			mHeader.fileName.push_back(memblock[9 + i]);
		//mHeader.fileName = "t.bmp";
		//mHeader.fileName.copy(memblock, mHeader.fileNameLength, 9);
		std::cout << "Input: " << (int)mHeader.fileNameLength /**(reinterpret_cast<unsigned char*>(&memblock[8]))*//* <<
			*(reinterpret_cast<int*>(&memblock[10])) << *(reinterpret_cast<int*>(&memblock[11])) */<< std::endl;
		// Start at the beginning 
		for (int i = 0; i < 20; i++)
			std::cout << i << ": " << *(reinterpret_cast<unsigned char*>(&memblock[i])) << std::endl;
		std::cout << "OUT NAME: " << mHeader.fileName << std::endl;


		mData.Decompress(memblock + 9 + mHeader.fileNameLength, (int)size - 9 , mHeader.fileSize);
		
		//std::cout << "fName: " << mHeader.fileName << "mSize: " << mData.mSize << ", " << mData.mData << "\n";
		//std::cout << memblock[8] << ", " << memblock[9] << ", " << memblock[10] << std::endl;
		
		std::ofstream out(mHeader.fileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (out.is_open())
		{
			// Write the data
			std::cout << "mSize: " << mData.mSize << ", " << mData.mData << "\n";
			out.write(mData.mData, mData.mSize);
			out.close();
		}

		delete[] memblock;
}
