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

	// Extra option
	bool extra = false;
	if (argc == 7)
	{
		extra = true;
	}
	
	std::mt19937 randGen(seed);

	std::ifstream input(inputFile);
	
	std::cout << inputFile << ", " << popSize << ", " << generations << ", " << mutationChance << ", " << seed << std::endl;

	auto locations = ParseLocations(input);
	std::for_each(locations.begin(), locations.end(), [](const auto &l)
	{
		std::cout << l.mName << ", " << l.mLatitude << ", " << l.mLongitude << std::endl;
	});

	std::ofstream logFile("out.txt");
	logFile << "INITIAL POPULATION:\n";
	Population population = GeneratePopulation(randGen, locations.size(), popSize);
	std::for_each(population.mMembers.begin(), population.mMembers.end(), [&logFile](const auto &pop)
	{
		logFile << ToString(pop, ",");
		logFile << std::endl;
	});

	auto fitness = ComputerFitness(population, locations);
	logFile << "FITNESS:\n";
	std::for_each(fitness.begin(), fitness.end(), [&logFile](const auto &fit)
	{
		logFile << fit.first << ":" << fit.second << std::endl;
	});

	for (int genIter = 1; genIter <= generations; ++genIter)
	{

		auto pairs = GeneratePairs(fitness, popSize, randGen);
		logFile << "SELECTED PAIRS:\n";
		std::for_each(pairs.begin(), pairs.end(), [&logFile](const auto &pair)
		{
			logFile << "(" << pair.first << "," << pair.second << ")\n";
		});

		auto crossedoverPopulation = GenerateCrossover(pairs, population, randGen, mutationChance, extra);
		logFile << "GENERATION: " << genIter << "\n";
		std::for_each(crossedoverPopulation.mMembers.begin(), crossedoverPopulation.mMembers.end(), [&logFile](const auto &member)
		{
			logFile << ToString(member, ",");
			logFile << std::endl;
		});
		population = crossedoverPopulation;

		fitness = ComputerFitness(population, locations);
		logFile << "FITNESS:\n";
		std::for_each(fitness.begin(), fitness.end(), [&logFile](const auto &fit)
		{
			logFile << fit.first << ":" << fit.second << std::endl;
		});
	}

	logFile << "SOLUTION:\n";
	auto min = std::min_element(fitness.begin(), fitness.end(), [](auto a, auto b)
	{
		return a.second < b.second;
	});
	std::cout << "Sorted: " << min->second << ", " << min->first << " | " << std::distance(fitness.begin(), min) << std::endl;
	auto minIdx = min->first;
	std::vector<int> finalPopulation = population.mMembers.at(minIdx);
	std::for_each(finalPopulation.begin(), finalPopulation.end(), [&logFile, &locations](const auto &location)
	{
		logFile << locations[location].mName << "\n";
	});

	logFile << locations.front().mName << "\n";
	logFile << "DISTANCE: " << min->second << " miles\n";
	return 0;
}