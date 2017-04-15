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
		location.mLatitude = std::stod(tokenizedLine[1])*0.0174533;
		location.mLongitude = std::stod(tokenizedLine[2])*0.0174533;
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
	
	int index = 0;

	std::for_each(population.mMembers.begin(), population.mMembers.end(), [&locations, &fitness, &index](const std::vector<int>& member)
	{
		std::vector<double> differences;

		std::adjacent_difference(member.begin(), member.end(), std::back_inserter(differences), [&locations](const int &from, const int &to)
		{
			return CalculateDistance(locations.at(from), locations.at(to));
		});

		double singleFitness = std::accumulate(differences.begin(), differences.end(), 0.0);
		singleFitness += CalculateDistance(locations.at(member.back()), locations.at(member.front()));
		fitness.push_back(std::make_pair(index++, singleFitness));
	});

	return fitness;
}

double CalculateDistance(const Location &start, const Location &end) noexcept
{
	double lon1 = start.mLongitude;
	double lat1 = start.mLatitude;

	double lon2 = end.mLongitude;
	double lat2 = end.mLatitude;

	double dlon = lon2 - lon1;
	double dlat = lat2 - lat1;

	double a1 = pow(sin(dlat / 2.0), 2) + cos(lat1)*cos(lat2)*pow(sin(dlon / 2.0), 2);
	double c = 2*atan2(sqrt(a1), sqrt(1 - a1));
	return 3961.0 * c;
}

std::vector<std::pair<int, double>> SortFitness(const std::vector<std::pair<int, double>> & fitness) noexcept
{
	auto copy(fitness);
	std::sort(copy.begin(), copy.end(), [](auto a, auto b)
	{
		return a.second < b.second;
	});

	return copy;
}

std::vector<double> GenerateProbabilities(const int & popSize) noexcept
{
	std::vector<double> probabilities(popSize);
	std::generate(probabilities.begin(), probabilities.end(), [&popSize]() { return 1.0 / popSize; });

	// The front 2 items are the highest ones so we multiply by 6.0
	probabilities[0] *= 6.0;
	probabilities[1] *= 6.0;
	// Multiply by 3.0 from 2 to (size/2 - 1)
	std::transform(probabilities.begin() + 2, probabilities.begin() + (popSize / 2 - 1), probabilities.begin() + 2, [](const auto &p)
	{
		return p * 3.0;
	});

	/* Normalize the vector */
	// Sum up the vector.
	double sum = std::accumulate(probabilities.begin(), probabilities.end(), 0.0);
	// Divide each element with sum to normalize
	std::transform(probabilities.begin(), probabilities.end(), probabilities.begin(), [&sum](const auto &prob)
	{
		return prob / sum;
	});

	return probabilities;
}

int FindAvailableIndex(std::vector<double> probabilities, double randomDouble, size_t currIdx, double runningSum) noexcept
{
	// Base case. Check end of vector
	if (currIdx >= probabilities.size())
	{
		return -1;
	}

	runningSum += probabilities[currIdx];
	if (runningSum >= randomDouble)
	{
		return currIdx;
	}
	return FindAvailableIndex(probabilities, randomDouble, ++currIdx, runningSum);
}

std::vector<std::pair<int, int>> GeneratePairs(std::vector<std::pair<int, double>> fitness, const int &popSize, std::mt19937 &randGen) noexcept
{
	auto sortedFitness = SortFitness(fitness);

	auto probabilities = GenerateProbabilities(popSize);

	std::vector<std::pair<int, int>> pairs(popSize);

	std::generate(pairs.begin(), pairs.end(), [&sortedFitness, &probabilities, &randGen] ()
	{
		int idxFirst, idxSecond;
		std::uniform_real_distribution<double> urand(0.0, 1.0);
		idxFirst = FindAvailableIndex(probabilities, urand(randGen));
		idxSecond = FindAvailableIndex(probabilities, urand(randGen));

		return std::make_pair(sortedFitness[idxFirst].first, sortedFitness[idxSecond].first);
	});

	return pairs;
}