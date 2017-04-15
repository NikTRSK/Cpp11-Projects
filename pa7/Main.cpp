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

	auto locations = ParseLocations(input);
	for (const auto &l : locations)
	{
		std::cout << l.mName << ", " << l.mLatitude << ", " << l.mLongitude << std::endl;
	}

	std::ofstream logFile("out.txt");
	logFile << "INITIAL POPULATION:\n";
	Population population = GeneratePopulation(randGen, locations.size(), popSize);
	for (const auto &pop : population.mMembers)
	{
		logFile << ToString(pop, ",");
		logFile << std::endl;
	}
	auto fitness = ComputerFitness(population, locations);
	logFile << "FITNESS:\n";
	for (const auto & fit : fitness)
	{
		logFile << fit.first << ":" << fit.second << std::endl;
	}
	auto pairs = GeneratePairs(fitness, popSize, randGen);
	logFile << "SELECTED PAIRS\n";
	for (const auto & pair : pairs)
	{
		logFile << "(" << pair.first << "," << pair.second << ")\n";
	}
	auto crossedoverPopulation = GenerateCrossover(pairs, randGen, mutationChance);
	logFile << "GENERATION: 1\n";
	for (auto member: crossedoverPopulation.mMembers)
	{
		logFile << ToString(member, ",");
		logFile << std::endl;
	}

	return 0;
}