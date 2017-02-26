#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "Exceptions.h"
#include <vector>
#include <memory>

class FASTA
{
public:
	FASTA(const char * filename);
	~FASTA();
	const std::string & GetHeader() const;
	const std::string & GetData() const;

protected:
	bool IsValid(char c);
	bool IsIgnored(char c);

private:
	std::string mHeader;
	std::string mData;

	std::vector<char> mValidCharacters = { 'T', 'C', 'A', 'G' };
	std::vector<char> mIgnoredCharacters = { '\n', '\r', '\xff' };
};