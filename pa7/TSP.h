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

std::string ToString(std::vector<int> input, std::string delimiter) noexcept;