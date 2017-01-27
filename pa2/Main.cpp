// Main.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include "Sha1.h"
#include "ProcessData.h"

int main(int argc, char* argv[])
{
	ProcessData dataProcessor;
	// Basic hashing
	if (argc == 2)
	{
		std::cout << dataProcessor.calculateHash(argv[1]) << std::endl;
	}

	// Dictionary lookup
	if (argc == 3)
	{
		// argv[1] - dictionary file
		// argv[2] - password file
		std::ifstream dictFile(argv[1]);
		//ProcessData dataProcessor;
		//dataProcessor.calculateDictionaryHashes(dictFile);
		if (dictFile.is_open())
		{
			dataProcessor.calculateDictionaryHashes(dictFile);
			dataProcessor.dictionaryAttack(argv[2]);
		}
	}

	return 0;
}

