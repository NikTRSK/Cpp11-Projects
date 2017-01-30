#include "ProcessData.h"

ProcessData::ProcessData()
{
	mHashedDictionary = new std::unordered_map<std::string, std::string>(100000);
}


ProcessData::~ProcessData()
{
	delete mHashedDictionary;
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
			mHashedDictionary->insert(std::make_pair(hashedPhrase, phrase));
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

	std::string hashToCrack;
	while (getline(passwordFile, hashToCrack))
	{
		auto searchResult = mHashedDictionary->find(hashToCrack);
		std::pair<std::string, std::string> crackAttempt;
		crackAttempt.first = hashToCrack;
		if (searchResult == mHashedDictionary->end())
		{
			crackAttempt.second = "??";
		}
		else
		{
			crackAttempt.second = searchResult->second;
		}
		mSolvedPasswords.push_back(crackAttempt);
		if (crackAttempt.second == "??")
		{
			mPasswordsToBruteForce.push_back(mSolvedPasswords.size()-1);
		}
	}

	passwordFile.close();
	
}

void ProcessData::bruteForceAttack()
{
	int numPerm = 0;
	int arr[4] = { 0 };
	std::vector<std::string> solution;


	while (arr[0] < 36)
	{
		arr[3]++;
		if (arr[3] > 35) {
			arr[3] = 0;
			arr[2]++;
			if (arr[2] > 35) {
				arr[2] = 0;
				arr[1]++;
				if (arr[1] > 35) {
					arr[1] = 0;
					arr[0]++;
				}
			}
		}
		std::string phrase = { convertToChar(arr[0]), convertToChar(arr[1]), convertToChar(arr[2]), convertToChar(arr[3]) };
		std::string phrase2 = {std::to_string(arr[0]) + " " + std::to_string(arr[1]) + " " + std::to_string(arr[2]) + " " + std::to_string(arr[3])};
		solution.push_back(phrase2 + " \t| " + phrase);
		numPerm++;
	}




	std::ofstream test("test.txt");
	if (test.is_open())
	{
		for (std::string i : solution) {
			test << i << std::endl;
		}
	}
	test << "PERMS: " << numPerm << ", " << solution.size();
	test.close();	
}

void ProcessData::bruteForceAttackSingleThreaded()
{
	// Setup timer
	Timer timer;
	timer.start();

	for (auto indexToCrack = std::end(mPasswordsToBruteForce)-1; indexToCrack != std::begin(mPasswordsToBruteForce); --indexToCrack)
	{
		int arr[4] = { 0 };
		while (arr[0] < 36)
		{
			arr[3]++;
			if (arr[3] > 35)
			{
				arr[3] = 0;
				arr[2]++;
				if (arr[2] > 35)
				{
					arr[2] = 0;
					arr[1]++;
					if (arr[1] > 35)
					{
						arr[1] = 0;
						arr[0]++;
					}
				}
			}
			std::string phrase = { convertToChar(arr[0]), convertToChar(arr[1]), convertToChar(arr[2]), convertToChar(arr[3]) };
			if (mSolvedPasswords[*indexToCrack].first == calculateHash(phrase))
			{
				mSolvedPasswords[*indexToCrack].second = phrase;
				mPasswordsToBruteForce.erase(indexToCrack);
			}
		}
	}

	double elapsed = timer.getElapsed();
	std::cout << "Brute force attack finished.\n";
	std::cout << "Time elapsed: " << elapsed << "s.\n";
	std::cout << mPasswordsToBruteForce.size() << std::endl;
}

void ProcessData::bruteForceAttackParallel()
{
}

char ProcessData::convertToChar(int number)
{
	// Handle numbers 0-9
	if (number >= 0 && number <= 9)
		return '0' + number;
	// Convert the number to a letter (a-z)
	return static_cast<char>(number + 87);
}

void ProcessData::writePasswordToFile()
{
	std::ofstream crackedPwrdsFile("pass_solved.txt");
	if (crackedPwrdsFile.is_open())
	{
		for (const auto& password : mSolvedPasswords)
		{
			crackedPwrdsFile << password.first << "," << password.second << std::endl;
		}
	}
	crackedPwrdsFile.close();
}
