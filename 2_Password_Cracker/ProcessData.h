#ifndef PROCESS_DATA
#define PROCESS_DATA

#include "string"
#include "fstream"
#include <unordered_map>
#include "Sha1.h"
#include "Timer.h"
#include <iostream>
#include <utility>
#include <string>
#include <tbb/parallel_invoke.h>
#include <map>
#pragma once
class ProcessData
{
private:
	std::unordered_map<std::string, std::string> *mHashedDictionary;
	std::vector<std::pair<std::string, std::string> > mSolvedPasswords;
	std::multimap<std::string, int> mPasswordsToBruteForce;

	/* HELPER FUNCTIONS */
	/* Converts 0-35 to a char */
	char ConvertToChar(int number);
	/* Brute Force helper function */
	void BruteForceInRange(std::multimap<std::string, int> &passwordsToCrack, int from, int to);
	/* Checks mPasswordsToBruteForce against a single permutation */
	void ProcessPermutation(std::string const &phrase);

public:
	ProcessData();
	~ProcessData();

	/* Crack Passwords */
	void CrackPasswords(char * dictFile, char * passFile);
	/* Hashes a char* string */
	std::string CalculateHash(char * input);
	/* Hashes a std::string string */
	std::string CalculateHash(std::string const &input);
	/* Calculates hashes from an input file */
	void CalculateDictionaryHashes(char * dictionary);
	/* Perform a dictionarry attack on an input file with hashed password */
	void DictionaryAttack(char * passwordFilename);
	/* Perform a Single Threaded Brute Force attack */
	void BruteForceAttackSingleThreaded();
	/* Perform a Parallel Brute Force attack */
	void BruteForceAttackParallel();
	/* Writes mSolvedPasswords to a file */
	void WritePasswordToFile();
};

#endif