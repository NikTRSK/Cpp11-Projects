// Main.cpp : Defines the entry point for the console application.
//

#include "RleTests.h"
#include "RleData.h"
#include <cstring>
#include <iostream>
#include <string>
#include <sstream>
#include "RLEFile.h"

void Part1Tests()
{
	TestFixtureFactory::theInstance().runTests();
}

// Check if the a string ends with a certain substring
bool ends_with(std::string input, std::string substring) {
	size_t found = input.substr(input.length() - 1 - 3, input.length() - 1).find(substring);
	if (found != std::string::npos)
		return true;
	else
		return false;
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		// Just the tests
		Part1Tests();
	}
	else
	{
		// TODO: Get the file name from argv[1] and either compress/decompress
		std::string inputFileName = argv[1];
		RleFile archiver;

		// Get the extension and check if it is and archive
		if (ends_with(inputFileName, ".rl1"))
		{
			archiver.ExtractArchive(inputFileName);
		}
		else
		{
			archiver.CreateArchive(inputFileName);
		}
	}

	return 0;
}

