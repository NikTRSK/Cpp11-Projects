#pragma once
#include <string>
#include <vector>
#include <random>
#include <memory>

struct Location
{
	std::string mName;
	double mLatitude;
	double mLongitude;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

std::vector<Location> ParseLocations(std::ifstream &input) noexcept;
std::vector<std::string> SplitString(std::string input, std::string delimiter) noexcept;
std::vector<int> GeneratePopulationMember(std::mt19937 &randGen, const int &locationsSize) noexcept;
Population GeneratePopulation(std::mt19937 &randGen, const int &locationsSize, const int &popSize) noexcept;
std::vector<std::pair<int, double>> ComputerFitness(const Population &population, const std::vector<Location>& locations) noexcept;
double CalculateDistance(const Location &start, const Location &end) noexcept;
std::vector<double> GenerateProbabilities(const int & popSize, const std::vector<std::pair<int, double>> &sortedFitness) noexcept;
std::vector<std::pair<int, double>> SortFitness(const std::vector<std::pair<int, double>> & fitness) noexcept;
int FindAvailableIndex(std::vector<double> probabilities, double randomDouble, size_t currIdx = 0, double runningSum = 0.0) noexcept;
std::vector<std::pair<int, int>> GeneratePairs(std::vector<std::pair<int, double>> fitness, const int &popSize, std::mt19937 &randGen) noexcept;
Population GenerateCrossover(const std::vector<std::pair<int, int>> &pairs, const Population &population, std::mt19937 &randGen, const double &mutationChance) noexcept;
std::vector<int> CrossoverPairs(const std::vector<int> & parentA, const std::vector<int> & parentB, std::mt19937 &randGen, const double &mutationChance) noexcept;
std::string ToString(std::vector<int> input, std::string delimiter) noexcept;