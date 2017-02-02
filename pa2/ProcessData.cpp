#include "ProcessData.h"

ProcessData::ProcessData()
{
	mHashedDictionary = new std::unordered_map<std::string, std::string>(100000);
}


ProcessData::~ProcessData()
{
	delete mHashedDictionary;
}

std::string ProcessData::CalculateHash(char * input)
{
	// Hash the input password
	unsigned char hash[20];
	sha1::calc(input, strlen(input), hash);

	char hex_str[41];
	sha1::toHexString(hash, hex_str);

	std::string s(hex_str);
	return s;
}

std::string ProcessData::CalculateHash(std::string input)
{
	// Hash the input password
	unsigned char hash[20];
	sha1::calc(input.c_str(), input.length(), hash);

	char hex_str[41];
	sha1::toHexString(hash, hex_str);

	std::string s(hex_str);
	return s;
}

void ProcessData::CalculateDictionaryHashes(std::ifstream &dictFile)
{
	if (dictFile.is_open())
	{
		// Setup timer to check time elapsed
		Timer timer;
		timer.start();
		std::string phrase;
		while (std::getline(dictFile, phrase))
		{
			std::string hashedPhrase = CalculateHash(phrase);
			mHashedDictionary->insert(std::make_pair(hashedPhrase, phrase));
		}
		double elapsed = timer.getElapsed();
		std::cout << "The dictionary has been loaded.\n";
		std::cout << "Time elapsed: " << elapsed << "s.\n";
	}
	dictFile.close();
}

void ProcessData::DictionaryAttack(char * passwordFilename)
{
	std::ifstream passwordFile(passwordFilename);

	std::string hashToCrack;
	Timer timer;
	timer.start();
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
			mPasswordsToBruteForce.insert({hashToCrack, mSolvedPasswords.size() - 1});
			//mPasswordsToBruteForce.push_back(mSolvedPasswords.size()-1);
		}
	}

	double elapsed = timer.getElapsed();
	std::cout << "Dictionary attack finished.\n";
	std::cout << "Time elapsed: " << elapsed << "s.\n";

	passwordFile.close();
	
}

void ProcessData::BruteForceAttackSingleThreaded()
{
	// Setup timer
	Timer timer;
	timer.start();

	BruteForceInRange(mPasswordsToBruteForce, 0, 35);

	double elapsed = timer.getElapsed();
	std::cout << "Brute force attack (serial) finished.\n";
	std::cout << "Time elapsed: " << elapsed << "s.\n";
	std::cout << mPasswordsToBruteForce.size() << std::endl;
}

void ProcessData::BruteForceAttackParallel()
{
	Timer timer;
	timer.start();

	tbb::parallel_invoke(
		[this] { BruteForceInRange(mPasswordsToBruteForce, 0, 3); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 4, 7); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 8, 11); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 12, 15); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 16, 19); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 20, 23); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 24, 27); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 28, 31); },
		[this] { BruteForceInRange(mPasswordsToBruteForce, 32, 35); }
	);

	double elapsed = timer.getElapsed();
	std::cout << "Brute force attack (parallel) finished.\n";
	std::cout << "Time elapsed: " << elapsed << "s.\n";
	std::cout << mPasswordsToBruteForce.size() << std::endl;
}

char ProcessData::ConvertToChar(int number)
{
	// Handle numbers 0-9
	if (number >= 0 && number <= 9)
		return '0' + number;
	// Convert the number to a letter (a-z)
	return static_cast<char>(number + 87);
}

void ProcessData::BruteForceInRange(std::multimap<std::string, int> &passwordsToCrack, int from, int to)
{
	int arr[4] = { 0 };
	arr[0] = from;
	while (arr[0] <= to)
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
		std::string phrase = { ConvertToChar(arr[0]), ConvertToChar(arr[1]), ConvertToChar(arr[2]), ConvertToChar(arr[3]) };
		std::string hashedPermutation = CalculateHash(phrase);
		auto searchResult = mPasswordsToBruteForce.equal_range(hashedPermutation);
		for (auto crackedIdx = searchResult.first; crackedIdx != searchResult.second; ++crackedIdx)
		{
			mSolvedPasswords[crackedIdx->second].second = phrase;
		}
		if (arr[0] == 0)
		{
			phrase = { ConvertToChar(arr[1]), ConvertToChar(arr[2]), ConvertToChar(arr[3]) };
			hashedPermutation = CalculateHash(phrase);
			auto searchResult = mPasswordsToBruteForce.equal_range(hashedPermutation);
			for (auto crackedIdx = searchResult.first; crackedIdx != searchResult.second; ++crackedIdx)
			{
				mSolvedPasswords[crackedIdx->second].second = phrase;
			}
			if (arr[1] == 0)
			{
				phrase = { ConvertToChar(arr[1]), ConvertToChar(arr[2]), ConvertToChar(arr[3]) };
				hashedPermutation = CalculateHash(phrase);
				auto searchResult = mPasswordsToBruteForce.equal_range(hashedPermutation);
				for (auto crackedIdx = searchResult.first; crackedIdx != searchResult.second; ++crackedIdx)
				{
					mSolvedPasswords[crackedIdx->second].second = phrase;
				}
				if (arr[2] == 0)
				{
					phrase = { ConvertToChar(arr[1]), ConvertToChar(arr[2]), ConvertToChar(arr[3]) };
					hashedPermutation = CalculateHash(phrase);
					auto searchResult = mPasswordsToBruteForce.equal_range(hashedPermutation);
					for (auto crackedIdx = searchResult.first; crackedIdx != searchResult.second; ++crackedIdx)
					{
						mSolvedPasswords[crackedIdx->second].second = phrase;
					}
				}
			}
		}
	}
}

void ProcessData::WritePasswordToFile()
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
