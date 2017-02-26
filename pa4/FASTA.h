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
	bool isValid(char c);
	bool isIgnored(char c);

private:
	std::string mHeader;
	std::string mData;

	std::vector<char> validCharacters = {'T', 'C', 'A', 'G'};
	std::vector<char> ignoredCharacters = {'\n', '\r', '\xff'};
};
