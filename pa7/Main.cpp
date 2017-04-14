#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <sstream>


int main(int argc, const char* argv[])
{
	std::cout << argv[1] << std::endl;
	std::string inputFile = argv[1];
	int popSize = atoi(argv[2]), 
		generations = atoi(argv[3]), 
		mutationChance = atoi(argv[4]), 
		seed = atoi(argv[5]);
	
	std::mt19937 randGen(seed);

	std::ifstream input(inputFile);
	
	std::cout << inputFile << ", " << popSize << ", " << generations << ", " << mutationChance << ", " << seed << std::endl;

	auto locs = ParseLocations(input);
	for (auto l : locs)
	{
		std::cout << l.mName << ", " << l.mLatitude << ", " << l.mLongitude << std::endl;
	}

	std::ofstream logFile("out.txt");
	logFile << "INITIAL POPULATION:\n";
	Population population = GeneratePopulation(randGen, locs.size(), popSize);
	for (auto pop : population.mMembers)
	{
		logFile << ToString(pop, ",");
		logFile << std::endl;
	}

	return 0;
}