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
	//std::vector<int> mPasswordsToBruteForce;
	//std::unordered_map<std::string, int> mPasswordsToBruteForce;
	std::multimap<std::string, int> mPasswordsToBruteForce;

public:
	ProcessData();
	~ProcessData();

	std::string CalculateHash(char * input);
	std::string CalculateHash(std::string input);
	void CalculateDictionaryHashes(std::ifstream &dictFile);
	void DictionaryAttack(char * passwordFilename);
	void BruteForceAttackSingleThreaded();
	void BruteForceAttackParallel();
	char ConvertToChar(int number);
	//void BruteForceInRange(std::vector<int> &passwordsToCrack, int from, int to);
	void BruteForceInRange(std::multimap<std::string, int> &passwordsToCrack, int from, int to);
	void WritePasswordToFile();
};

#endif