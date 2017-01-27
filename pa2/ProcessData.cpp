#include "ProcessData.h"

ProcessData::ProcessData()
{
	hashedDictionary = new std::unordered_map<std::string, std::string>(100000);
}


ProcessData::~ProcessData()
{
}

std::string ProcessData::calculateHash(char * input)
{
	// Hash the input password
	unsigned char hash[20];
	sha1::calc(input, strlen(input), hash);

	char hex_str[41];
	sha1::toHexString(hash, hex_str);

	std::string s(hex_str);
	return s;
}

std::string ProcessData::calculateHash(std::string input)
{
	// Hash the input password
	unsigned char hash[20];
	sha1::calc(input.c_str(), input.length(), hash);

	char hex_str[41];
	sha1::toHexString(hash, hex_str);

	std::string s(hex_str);
	return s;
}

void ProcessData::calculateDictionaryHashes(std::ifstream &dictFile)
{
	if (dictFile.is_open())
	{
		// Setup timer to check time elapsed
		Timer timer;
		timer.start();
		std::string phrase;
		while (std::getline(dictFile, phrase))
		{
			std::string hashedPhrase = calculateHash(phrase);
			hashedDictionary->insert(std::make_pair(hashedPhrase, phrase));
		}
		double elapsed = timer.getElapsed();
		std::cout << "The dictionary has been loaded.\n";
		std::cout << "Time elapsed: " << elapsed << "s.\n";
	}
	dictFile.close();
}

void ProcessData::dictionaryAttack(char * passwordFilename)
{
	std::ifstream passwordFile(passwordFilename);
	std::ofstream crackedPwrdsFile("pass_solved.txt");

	if (crackedPwrdsFile.is_open())
	{
		std::string hashToCrack;
		while (getline(passwordFile, hashToCrack))
		{
			auto searchResult = hashedDictionary->find(hashToCrack);
			crackedPwrdsFile << hashToCrack << ",";
			if (searchResult == hashedDictionary->end())
			{
				crackedPwrdsFile << "??\n";
			}
			else
			{
				crackedPwrdsFile << searchResult->second << std::endl;
			}
		}
	}

	passwordFile.close();
	crackedPwrdsFile.close();
}