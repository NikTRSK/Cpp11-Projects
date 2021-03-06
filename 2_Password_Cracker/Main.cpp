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
		std::cout << dataProcessor.CalculateHash(argv[1]) << std::endl;
	}

	// Password cracking attack
	if (argc == 3)
	{
		// argv[1] - dictionary file
		// argv[2] - password file
		dataProcessor.CrackPasswords(argv[1], argv[2]);
	}

	return 0;
}