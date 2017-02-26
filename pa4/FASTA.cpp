#include "FASTA.h"



FASTA::FASTA(const char * filename)
{
	std::ifstream file(filename, std::ios::ate);
	if (!file.is_open())
	{
		throw FileLoadExcept();
	}
	// Get the total number of characters in the file
	std::ifstream::pos_type fileLength = file.tellg();
//	std::cout << "^^^^^^^^" << fileSize << std::endl;
	file.seekg(0, std::ios::beg); // Seek back to start of file
	mData.reserve(static_cast<unsigned int>(fileLength));
	// Get the header from the file
	getline(file, mHeader);
	if (mHeader[0] != '>')
	{
		throw FileLoadExcept();
	}
	// Erase the first character of the header
	mHeader.erase(0, 1);
	
	std::cout << mHeader << std::endl;

	// Read in the data file
	char c;
	while (file.get(c))
	{
		if (isValid(c))
		{
			mData.push_back(c);
		}
		// If the character is invalid throw exception.
		// Else just keep going
		else if (!isIgnored(c))
		{
			throw FileLoadExcept();
		}
	}

	std::cout << mData << std::endl;
}


FASTA::~FASTA()
{
}

bool FASTA::isValid(char c)
{
	for (auto const& ch : validCharacters)
	{
		if (c == ch)
		{
			return true;
		}
	}
	return false;
}

bool FASTA::isIgnored(char c)
{
	for (auto const& ch : ignoredCharacters)
	{
		if (c == ch)
		{
			return true;
		}
	}
	return false;
}
