#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <sstream>
#include <iostream>

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

std::vector<double> GenerateProbabilities(const int & popSize, const std::vector<std::pair<int, double>> &sortedFitness) noexcept
{
	std::vector<double> probabilities(popSize);
	std::generate(probabilities.begin(), probabilities.end(), [&popSize]() { return 1.0 / popSize; });

	int idx = sortedFitness.at(0).first;
	probabilities.at(idx) *= 6.0;
	idx = sortedFitness.at(1).first;
	probabilities.at(idx) *= 6.0;

	// Change this to remove loop
	for (unsigned int i = 2; i < sortedFitness.size() / 2; ++i)
	{
		idx = sortedFitness.at(i).first;
		probabilities.at(idx) *= 3.0;
	}
	// The front 2 items are the highest ones so we multiply by 6.0
//	probabilities[0] *= 6.0;
//	probabilities[1] *= 6.0;
//	// Multiply by 3.0 from 2 to (size/2 - 1)
//	std::transform(probabilities.begin() + 2, probabilities.begin() + (popSize / 2 - 1), probabilities.begin() + 2, [](const auto &p)
//	{
//		return p * 3.0;
//	});

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

	auto probabilities = GenerateProbabilities(popSize, sortedFitness);

	std::vector<std::pair<int, int>> pairs(popSize);

	std::generate(pairs.begin(), pairs.end(), [&fitness, &probabilities, &randGen] ()
	{
		int idxFirst, idxSecond;
		std::uniform_real_distribution<double> urand(0.0, 1.0);
		idxFirst = FindAvailableIndex(probabilities, urand(randGen));
		idxSecond = FindAvailableIndex(probabilities, urand(randGen));

		return std::make_pair(fitness[idxFirst].first, fitness[idxSecond].first);
	});

	return pairs;
}

Population GenerateCrossover(const std::vector<std::pair<int ,int>> &pairs, std::mt19937 &randGen, const double &mutationChance) noexcept
{
	Population resultPopulation;

	std::vector<int> A;
	std::vector<int> B;

	// Extract parents from pairs
	std::transform(pairs.begin(), pairs.end(), std::back_inserter(A), [](const auto &member)
	{
		return member.first;
	});

	std::transform(pairs.begin(), pairs.end(), std::back_inserter(B), [](const auto &member)
	{
		return member.second;
	});
	
	for (unsigned int i = 0; i < pairs.size(); ++i)
		resultPopulation.mMembers.push_back(CrossoverPairs(A, B, randGen, mutationChance));

	return resultPopulation;
}

std::vector<int> CrossoverPairs(const std::vector<int> & parentA, const std::vector<int> & parentB, std::mt19937 &randGen, const double &mutationChance) noexcept
{
	std::uniform_int<int> distributionIndexGenerator(1, parentA.size() - 2);
	std::uniform_int<int> parentBool(0, 1);

	int crossoverIndex = distributionIndexGenerator(randGen);
	int crossoverOrder = parentBool(randGen);

	std::vector<int> result;
	if (crossoverOrder == 1)
	{
		// A goes first
		std::copy_n(parentA.begin(), crossoverIndex, std::back_inserter(result));
		std::copy_if(parentB.begin(), parentB.end(), std::back_inserter(result), [&result](const int &val)
		{
			return std::find(result.begin(), result.end(), val) == result.end();
		});
	}
	else
	{
		// B goes first
		std::copy_n(parentB.begin(), crossoverIndex, std::back_inserter(result));
		std::copy_if(parentA.begin(), parentA.end(), std::back_inserter(result), [&result](const int &val)
		{
			return std::find(result.begin(), result.end(), val) == result.end();
		});
	}

	std::uniform_real_distribution<double> mutRnd(0.0, 1.0);
	double randomChance = mutRnd(randGen);
	if (randomChance <= mutationChance)
	{
		std::uniform_int_distribution<int> rnd(1, result.size() - 1);
		int idx1 = rnd(randGen);
		int idx2 = rnd(randGen);
		std::swap(result.at(idx1), result.at(idx2));
	}
	return result;
}