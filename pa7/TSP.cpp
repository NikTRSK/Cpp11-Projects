#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <sstream>

std::vector<Location> ParseLocations(std::ifstream &input) noexcept
{
	std::vector<Location> locations;
	std::string line;
	while (std::getline(input, line))
	{
		auto tokenizedLine = SplitString(line, ",");
		Location location;
		location.mName = tokenizedLine[0];
		location.mLatitude = std::stod(tokenizedLine[1]);
		location.mLongitude = std::stod(tokenizedLine[2]);
		locations.push_back(location);
	}
	return locations;
}

std::vector<std::string> SplitString(std::string input, std::string delimiter) noexcept
{
	std::vector<std::string> tokens;
	std::string token;
	size_t position = 0, found = 0;

	while ((found = input.find_first_of(delimiter, position)) != std::string::npos)
	{
		tokens.push_back(input.substr(position, found - position));
		position = found + 1;
	}
	tokens.push_back(input.substr(position));

	return tokens;
}

std::vector<int> GeneratePopulationMember(std::mt19937 &randGen, const int &locationsSize) noexcept
{
	std::vector<int> populationMember(locationsSize);
	// 1. Create an initial vector that contains the sequential values from 0…n – 1
	std::iota(std::begin(populationMember), std::end(populationMember), 0); // Fill vector from 0 to n-1

	// 2. Use std::shuffle to shuffle from begin() + 1 to end().
	std::shuffle(populationMember.begin() + 1, populationMember.end(), randGen);

	return populationMember;
}

Population GeneratePopulation(std::mt19937 &randGen, const int &locationsSize, const int &popSize) noexcept
{
	Population population;
	population.mMembers.resize(popSize);
	
	// 3. Generate Random population members for every population
	std::generate(population.mMembers.begin(), population.mMembers.end(), [&locationsSize, &randGen]()
	{
		return GeneratePopulationMember(randGen, locationsSize);
	});

	return population;
}

std::string ToString(std::vector<int> input, std::string delimiter) noexcept
{
	std::ostringstream outStream;
	std::copy(input.begin(), input.end(), std::ostream_iterator<int>(outStream, delimiter.c_str()));
	std::string out = outStream.str();
	out.pop_back();
	return out;
}

std::vector<std::pair<int, double>> ComputerFitness(const Population &population, const std::vector<Location>& locations) noexcept
{
	std::vector<std::pair<int, double>> fitness;

	std::pair<int, double> individual;

	std::for_each(population.mMembers.begin(), population.mMembers.end(), [&locations, &fitness](const std::vector<int>& member)
	{
		
	});

	return fitness;
}

//double CalculateDistance(const Location &start, const Location &end) noexcept
//{
//
//}