#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include "Op.h"
#include "Exceptions.h"
#include <fstream>
#include <unordered_map>
#include <ctime>

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
		mXCoordinate = 0;
		mYCoordinate = 0;
	}

	~MachineState()
	{
		srand(static_cast<unsigned int>(time(nullptr)));
	}

	// Active line number in behavior program
	int mProgramCounter;
	// Number of actions taken this turn
	int mActionsTaken;
	// Current facing of this character
	Facing mFacing;
	// Test flag for branches
	bool mTest;

	int GetActionsPerTurn() const noexcept { return mActionsPerTurn; }
	bool GetInfect() const noexcept { return mInfectOnAttack; }
	static bool GetRandomBool() noexcept { return (rand() % 2) != 0; }
	const int & GetX() const noexcept { return mXCoordinate; };
	const int & GetY() const noexcept { return mYCoordinate; };
	void UpdateLocation() noexcept;
	bool IsInbound(const int & x, const int & y) const noexcept { return (x < 20 && x >= 0 && y < 20 && y >= 0); };
private:
	// Data which is set by the traits
	int mActionsPerTurn;
	bool mInfectOnAttack;

	int mXCoordinate;
	int mYCoordinate;
};

inline void MachineState::UpdateLocation() noexcept
{
	switch (mFacing)
	{
		case MachineState::UP:
			if (IsInbound(this->GetX(), this->GetY() - 1))
				--this->mYCoordinate;
			break;
		case MachineState::DOWN:
			if (IsInbound(this->GetX(), this->GetY() + 1))
				++this->mYCoordinate;
			break;
		case MachineState::LEFT:
			if (IsInbound(this->GetX() - 1, this->GetY()))
				--this->mXCoordinate;
			break;
		case MachineState::RIGHT:
			if (IsInbound(this->GetX() + 1, this->GetY()))
				++this->mXCoordinate;
			break;
		default:
			break;
	}
}

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
	// Load in all the ops for this machine from the specified file
	void LoadMachine(const std::string& filename);

	// Given the state, binds the trait parameters to it
	void BindState(MachineState& state);

	// Take a turn using this logic for the passed in state
	void TakeTurn(MachineState& state);

	// Destructor
	~Machine();
private:
	// Parses a single line of file and returns a vector containing the opperation and the parameter if any
	std::vector<std::string> ParseInstruction(std::string & input) const noexcept;
	// Trims whitespace from the sides of a string
	void TrimInPlace(std::string & input) const noexcept;
	std::string Trim(const std::string & input) const noexcept;
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
	std::ifstream file(filename.c_str()/*, std::ios::ate*/);
	if (!file.is_open())
	{
		throw FileLoadExcept();
	}
	// Should we worry about malformated input files? Whitespace? casing?
	mOps.clear();
	std::string currentLine;
	while (std::getline(file, currentLine))
	{
		auto parameterizedString = ParseInstruction(currentLine);
		{ // debug
			std::cout << "PARSE DEBUG: ";
			for (auto s : parameterizedString)
				std::cout << s;
			std::cout << std::endl;
		} // debug
		switch(Convert(parameterizedString[0]))
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
				mOps.push_back(std::make_shared<OpTestHuman>(0));
				break;
			case test_wall:
				mOps.push_back(std::make_shared<OpTestWall>(0));
				break;
			case test_zombie:
				mOps.push_back(std::make_shared<OpTestZombie>(0));
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
				break;
		}
	}
	// TEMP CODE: Add your parsing code here!
//	mOps.clear();
//	mOps.push_back(std::make_shared<OpRotate>(0));
//	mOps.push_back(std::make_shared<OpRotate>(0));
//	mOps.push_back(std::make_shared<OpRotate>(1));
//	mOps.push_back(std::make_shared<OpGoto>(1));
	// END TEMP CODE
}

template <typename MachineTraits>
void Machine<MachineTraits>::BindState(MachineState& state)
{
	state.mActionsPerTurn = MachineTraits::ACTIONS_PER_TURN;
	state.mInfectOnAttack = MachineTraits::INFECT_ON_ATTACK;
}

template <typename MachineTraits>
void Machine<MachineTraits>::TakeTurn(MachineState& state)
{
	std::cout << "TAKING TURN" << std::endl;
	state.mActionsTaken = 0;
	while (state.mActionsTaken < MachineTraits::ACTIONS_PER_TURN)
	{
		mOps.at(state.mProgramCounter - 1)->Execute(state);
	}
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
		result.push_back(input.substr(found+1));
	}
	else
	{
		result.push_back(Trim(input));
	}

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
