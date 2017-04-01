#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <fstream>
#include <ctime>
#include "Op.h"
#include "Exceptions.h"
#include "Coordinate.h"

// Defines state data accessible by the machine and ops
struct MachineState
{
	// Friend Machine so BindState works
	template <typename MachineTraits>
	friend class Machine;

	enum Facing { UP, RIGHT, DOWN, LEFT };
	MachineState()
		: mProgramCounter(1)
		, mActionsTaken(0)
		, mFacing(UP)
		, mTest(false)
	{
		mCoordinate = std::make_shared<Coordinate>();
		srand(static_cast<unsigned int>(time(nullptr)));
	}

	~MachineState()
	{ }

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;
	std::shared_ptr<Coordinate> mCoordinate;

	bool GetInfect() const noexcept { return mInfectOnAttack; }
	static bool GetRandomBool() noexcept { return (rand() % 2) != 0; }
	const int & GetX() const noexcept { return mCoordinate->x; };
	const int & GetY() const noexcept { return mCoordinate->y; };
	bool IsInbound(const int & x, const int & y) const noexcept { return (x < 19 && x > 0 && y < 19 && y > 0); };
	int GetOpSize() const noexcept { return this->mNumberOfOperations; };
private:
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;

	int mNumberOfOperations;
};

// Describes the machine which processes ops.
// Different policies dictate behavior possible for machine.
template <typename MachineTraits>
class Machine
{
	enum Operation
	{
		attack, ranged_attack, rotate, forward, endturn, test_human, test_wall,
		test_zombie, test_random, test_passable, je, jne, GoTo, invalid
	};

public:
	// Destructor
	~Machine();

	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	// Set the Commands for a player
	void SetOps(std::vector<std::shared_ptr<Op>> opVector) noexcept;

private:
	// Parses a single line of file and returns a vector containing the opperation and the parameter if any
	std::vector<std::string> ParseInstruction(std::string & input) const noexcept;

	// Trims whitespace from the sides of a string
	void TrimInPlace(std::string & input) const noexcept;
	std::string Trim(const std::string & input) const noexcept;
	
	// Checks if a string is a number
	bool IsNumber(const std::string &input) const noexcept;

	// Convert a string to an Op
	Operation Convert(const std::string & key) const noexcept
	{
		if (key == "attack") return attack;
		if (key == "ranged_attack") return ranged_attack;
		if (key == "rotate") return rotate;
		if (key == "forward") return forward;
		if (key == "endturn") return endturn;
		if (key == "test_human") return test_human;
		if (key == "test_wall") return test_wall;
		if (key == "test_zombie") return test_zombie;
		if (key == "test_random") return test_random;
		if (key == "test_passable") return test_passable;
		if (key == "je") return je;
		if (key == "jne") return jne;
		if (key == "goto") return GoTo;

		return invalid;
	};

	std::vector<std::shared_ptr<Op>> mOps;
};

template <typename MachineTraits>
void Machine<MachineTraits>::LoadMachine(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		throw FileLoadExcept();
	}
	mOps.clear();
	std::string currentLine;
	while (std::getline(file, currentLine))
	{
		auto parameterizedString = ParseInstruction(currentLine);
		// Check if parameter is a digit. Only digits are allowed.
		if (parameterizedString[0].empty())
		{
			continue;
		}
		if (parameterizedString.size() == 2 && !IsNumber(parameterizedString[1]))
		{
			throw FileLoadExcept();
		}
		switch (Convert(parameterizedString[0]))
		{
		case attack:
			mOps.push_back(std::make_shared<OpAttack>(0));
			break;
		case ranged_attack:
			mOps.push_back(std::make_shared<OpRangedAttack>(0));
			break;
		case rotate:
			mOps.push_back(std::make_shared<OpRotate>(std::stoi(parameterizedString[1])));
			break;
		case forward:
			mOps.push_back(std::make_shared<OpForward>(0));
			break;
		case endturn:
			mOps.push_back(std::make_shared<OpEndturn>(0));
			break;
		case test_human:
			if (parameterizedString.size() == 1 || parameterizedString[1] != "1" && parameterizedString[1] != "2")
			{
				throw FileLoadExcept();
			}
			mOps.push_back(std::make_shared<OpTestHuman>(std::stoi(parameterizedString[1])));
			break;
		case test_wall:
			mOps.push_back(std::make_shared<OpTestWall>(0));
			break;
		case test_zombie:
			if (parameterizedString.size() == 1 || parameterizedString[1] != "1" && parameterizedString[1] != "2")
			{
				throw FileLoadExcept();
			}
			mOps.push_back(std::make_shared<OpTestZombie>(std::stoi(parameterizedString[1])));
			break;
		case test_random:
			mOps.push_back(std::make_shared<OpTestRandom>(0));
			break;
		case test_passable:
			mOps.push_back(std::make_shared<OpTestPassable>(0));
			break;
		case je:
			mOps.push_back(std::make_shared<OpJE>(std::stoi(parameterizedString[1])));
			break;
		case jne:
			mOps.push_back(std::make_shared<OpJNE>(std::stoi(parameterizedString[1])));
			break;
		case GoTo:
			mOps.push_back(std::make_shared<OpGoto>(std::stoi(parameterizedString[1])));
			break;
		default:
			throw FileLoadExcept(); // If the operation is not valid just throw an exception
		}
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;

	state.mNumberOfOperations = mOps.size();
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		// If the PC goes out of range
		if (state.mProgramCounter > static_cast<int>(mOps.size()))
		{
			throw InvalidOp();
		}
		
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
}

template <typename MachineTraits>
void Machine<MachineTraits>::SetOps(std::vector<std::shared_ptr<Op>> opVector) noexcept
{
	this->mOps = opVector;
}

template <typename MachineTraits>
Machine<MachineTraits>::~Machine()
{
	mOps.clear();
}

template <typename MachineTraits>
std::vector<std::string> Machine<MachineTraits>::ParseInstruction(std::string& input) const noexcept
{
	std::string commentRemoved;
	// Remove the comment
	std::size_t found = input.find_first_of(";");
	if (found != std::string::npos)
	{
		input.erase(input.begin() + found, input.end());
	}

	// Get operation and parameter
	std::vector<std::string> result;
	found = input.find(",");
	if (found != std::string::npos)
	{
		result.push_back(Trim(input.substr(0, found)));
		result.push_back(Trim(input.substr(found + 1)));
	}
	else
	{
		result.push_back(Trim(input));
	}

	// Return a vector with command and parameter
	return result;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TrimInPlace(std::string& input) const noexcept
{
	// Remove whitespace on the left
	auto left = input.find_first_not_of(' ');
	if (left != std::string::npos)
	{
		input.erase(0, left);
	}
	// Remove whitespace on the right
	auto right = input.find_last_not_of(' ');
	if (right != std::string::npos)
	{
		input.erase(right + 1, input.length() - right - 1);
	}
}

template <typename MachineTraits>
std::string Machine<MachineTraits>::Trim(const std::string& input) const noexcept
{
	// Remove whitespace on the left
	auto left = input.find_first_not_of(' ');
	if (left == std::string::npos)
	{
		left = 0;
	}
	// Remove whitespace on the right
	auto right = input.find_last_not_of(' ');
	if (right == std::string::npos)
	{
		right = 0;
	}
	return input.substr(left, right - left + 1);
}

template <typename MachineTraits>
bool Machine<MachineTraits>::IsNumber(const std::string& input) const noexcept
{
	return !input.empty() && std::all_of(input.begin(), input.end(), ::isdigit);
}
