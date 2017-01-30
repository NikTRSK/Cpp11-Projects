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
#pragma once
class ProcessData
{
private:
	std::unordered_map<std::string, std::string> *mHashedDictionary;
	std::vector<std::pair<std::string, std::string> > mSolvedPasswords;
	std::vector<int> mPasswordsToBruteForce;

	//std::vector<std::string, std::string> mPasswordsToBruteForce;

public:
	ProcessData();
	~ProcessData();

	std::string calculateHash(char * input);
	std::string calculateHash(std::string input);
	void calculateDictionaryHashes(std::ifstream &dictFile);
	void dictionaryAttack(char * passwordFilename);
	void bruteForceAttack();
	void bruteForceAttackSingleThreaded();
	void bruteForceAttackParallel();
	char convertToChar(int number);
	void writePasswordToFile();
};

#endif