#ifndef PROCESS_DATA
#define PROCESS_DATA

#include "string"
#include "fstream"
#include <unordered_map>
#include "Sha1.h"
#include "Timer.h"
#include <iostream>
#include <utility>
#pragma once
class ProcessData
{
private:
	std::unordered_map<std::string, std::string> *hashedDictionary;
	//typedef std::unordered_map<std::string, std::string> dictionaryMap;
	//dictionaryMap *hashedDictionary;

public:
	ProcessData();
	~ProcessData();

	std::string calculateHash(char * input);
	std::string calculateHash(std::string input);
	void calculateDictionaryHashes(std::ifstream &dictFile);
	void dictionaryAttack(char * passwordFilename);
};

#endif