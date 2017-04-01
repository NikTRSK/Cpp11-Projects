#include "Op.h"
#include "Machine.h"
#include <iostream>
#include "World.h"

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
//	std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam << std::endl;
}

void OpRotate::Execute(MachineState& state)
{
	DebugOutput(state);
	switch (state.mFacing)
	{
	case (MachineState::UP) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::RIGHT;
		}
		else
		{
			state.mFacing = MachineState::LEFT;
		}
		break;
	case (MachineState::RIGHT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::DOWN;
		}
		else
		{
			state.mFacing = MachineState::UP;
		}
		break;
	case (MachineState::DOWN) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::LEFT;
		}
		else
		{
			state.mFacing = MachineState::RIGHT;
		}
		break;
	default:
	case (MachineState::LEFT) :
		if (mParam == 0)
		{
			state.mFacing = MachineState::UP;
		}
		else
		{
			state.mFacing = MachineState::DOWN;
		}
		break;
	}

	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	if (mParam < 1 || mParam >= state.GetOpSize())
	{
		throw GoToExcept();
	}
	else
	{
		state.mProgramCounter = mParam;

	}
	state.mActionsTaken++;
}

void OpAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	if (HasPlayer(state))
	{
		Attack(state);
	}
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpRangedAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.GetInfect())
	{
		throw RangedAttackExcept();
	}
	else
	{
		Attack(state);
	}
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpForward::Execute(MachineState& state)
{
	int x = state.GetX();
	int y = state.GetY();
	DebugOutput(state);
	if (TileIsOpen(state))
	{
		auto player = World::get().mGrid.find(std::pair<int, int>(x, y));
		if (player != World::get().mGrid.end())
		{
			World::get().mGrid.erase(player);
		}
		
		UpdateLocation(state);
		World::get().mGrid[std::pair<int, int>(state.GetX(), state.GetY())] = &state;
	}
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpEndturn::Execute(MachineState& state)
{
	DebugOutput(state);
	// If Zombie set actions taken to 1, if human set to 2
	if (state.GetInfect())
	{
		state.mActionsTaken = 1;
	}
	else
	{
		state.mActionsTaken = 2;
	}
	state.mProgramCounter++;
}

void OpTestHuman::Execute(MachineState& state)
{
	DebugOutput(state);
	int x = state.GetX();
	int y = state.GetY();
	state.mTest = false;
	switch (state.mFacing)
	{
	case MachineState::UP:
		if (state.IsInbound(x, y - mParam) && World::get().HasHuman(x, y - mParam))
		{
			state.mTest = true;
		}
		break;
	case MachineState::DOWN:
		if (state.IsInbound(x, y + mParam) && World::get().HasHuman(x, y + mParam))
		{
			state.mTest = true;
		}
		break;
	case MachineState::LEFT:
		if (state.IsInbound(x - mParam, y) && World::get().HasHuman(x - mParam, y))
		{
			state.mTest = true;
		}
		break;
	case MachineState::RIGHT:
		if (state.IsInbound(x + mParam, y) && World::get().HasHuman(x + mParam, y))
		{
			state.mTest = true;
		}
		break;
	default:
		break;
	}
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestWall::Execute(MachineState& state)
{
	DebugOutput(state);
	int x = state.GetX();
	int y = state.GetY();
	state.mTest = false;
	switch (state.mFacing)
	{
	case MachineState::UP:
		if (state.IsInbound(x, y - 1))
		{
			state.mTest = true;
		}
		break;
	case MachineState::DOWN:
		if (state.IsInbound(x, y + 1))
		{
			state.mTest = true;
		}
		break;
	case MachineState::LEFT:
		if (state.IsInbound(x - 1, y))
		{
			state.mTest = true;
		}
		break;
	case MachineState::RIGHT:
		if (state.IsInbound(x + 1, y))
		{
			state.mTest = true;
		}
		break;
	default:
		break;
	}
	// If facing a wall set mTest to true
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestZombie::Execute(MachineState& state)
{
	DebugOutput(state);
	int x = state.GetX();
	int y = state.GetY();
	state.mTest = false;
	switch (state.mFacing)
	{
	case MachineState::UP:
		if (state.IsInbound(x, y - mParam) && World::get().HasZombie(x, y - mParam))
		{
			state.mTest = true;
		}
		break;
	case MachineState::DOWN:
		if (state.IsInbound(x, y + mParam) && World::get().HasZombie(x, y + mParam))
		{
			state.mTest = true;
		}
		break;
	case MachineState::LEFT:
		if (state.IsInbound(x - mParam, y) && World::get().HasZombie(x - mParam, y))
		{
			state.mTest = true;
		}
		break;
	case MachineState::RIGHT:
		if (state.IsInbound(x + mParam, y) && World::get().HasZombie(x + mParam, y))
		{
			state.mTest = true;
		}
		break;
	default:
		break;
	}
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestRandom::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mTest = state.GetRandomBool();
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestPassable::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mTest = TileIsOpen(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpJE::Execute(MachineState& state)
{
	DebugOutput(state);
	if (state.mTest)
	{
		state.mProgramCounter = mParam;
	}
	else
	{
		state.mProgramCounter++;
	}
	state.mActionsTaken++;
}

void OpJNE::Execute(MachineState& state)
{
	DebugOutput(state);
	if (!state.mTest)
	{
		state.mProgramCounter = mParam;
	}
	else
	{
		state.mProgramCounter++;
	}
	state.mActionsTaken++;
}

void Op::UpdateLocation(MachineState& state) const noexcept
{
	switch (state.mFacing)
	{
	case MachineState::UP:
		--state.mCoordinate->y;
		break;
	case MachineState::DOWN:
		++state.mCoordinate->y;
		break;
	case MachineState::LEFT:
		--state.mCoordinate->x;
		break;
	case MachineState::RIGHT:
		++state.mCoordinate->x;
		break;
	default:
		break;
	}
}

bool Op::TileIsOpen(MachineState& state) const noexcept
{
	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		if (y != 0 && World::get().mGrid.find(std::pair<int, int>(x, y - 1)) == World::get().mGrid.end())
		{
			return true;
		}
		break;
	case MachineState::DOWN:
		if (y != 19 && World::get().mGrid.find(std::pair<int, int>(x, y + 1)) == World::get().mGrid.end())
		{
			return true;
		}
		break;
	case MachineState::LEFT:
		if (x != 0  && World::get().mGrid.find(std::pair<int, int>(x - 1, y)) == World::get().mGrid.end())
		{
			return true;
		}
		break;
	case MachineState::RIGHT:
		if (x != 19 && World::get().mGrid.find(std::pair<int, int>(x + 1, y)) == World::get().mGrid.end())
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

bool Op::HasPlayer(MachineState& state) const noexcept
{
	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		if (World::get().mGrid.find(std::pair<int, int>(x, y - 1)) != World::get().mGrid.end())
		{
			return true;
		}
		break;
	case MachineState::DOWN:
		if (World::get().mGrid.find(std::pair<int, int>(x, y + 1)) != World::get().mGrid.end())
		{
			return true;
		}
		break;
	case MachineState::LEFT:
		if (World::get().mGrid.find(std::pair<int, int>(x - 1, y)) != World::get().mGrid.end())
		{
			return true;
		}
		break;
	case MachineState::RIGHT:
		if (World::get().mGrid.find(std::pair<int, int>(x + 1, y)) != World::get().mGrid.end())
		{
			return true;
		}
		break;
	default:
		break;
	}
	return false;
}

void OpAttack::Attack(MachineState& state) const noexcept
{
	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		// Anyone can kill a zombie
		if (World::get().HasZombie(x, y - 1))
		{
			World::get().KillZombie(state, 1);
		}
		// Zombies convert humans
		else if (World::get().HasHuman(x, y - 1) && state.GetInfect())
		{
			World::get().ConvertHuman(state);
		}
		// Humans can kill other humans
		else if (World::get().HasHuman(x, y - 1) && !state.GetInfect())
		{
			World::get().KillHuman(state, 1);
		}
		break;
	case MachineState::DOWN:
		if (World::get().HasZombie(x, y + 1))
		{
			World::get().KillZombie(state, 1);
		}
		else if (World::get().HasHuman(x, y + 1) && state.GetInfect())
		{
			World::get().ConvertHuman(state);
		}
		else if (World::get().HasHuman(x, y + 1) && !state.GetInfect())
		{
			World::get().KillHuman(state, 1);
		}
		break;
	case MachineState::LEFT:
		if (World::get().HasZombie(x - 1, y))
		{
			World::get().KillZombie(state, 1);
		}
		else if (World::get().HasHuman(x - 1, y) && state.GetInfect())
		{
			World::get().ConvertHuman(state);
		}
		else if (World::get().HasHuman(x - 1, y) && !state.GetInfect())
		{
			World::get().KillHuman(state, 1);
		}
		break;
	case MachineState::RIGHT:
		if (World::get().HasZombie(x + 1, y))
		{
			World::get().KillZombie(state, 1);
		}
		else if (World::get().HasHuman(x + 1, y) && state.GetInfect())
		{
			World::get().ConvertHuman(state);
		}
		else if (World::get().HasHuman(x + 1, y) && !state.GetInfect())
		{
			World::get().KillHuman(state, 1);
		}
		break;
	default:
		break;
	}
}

void OpRangedAttack::Attack(MachineState& state) const noexcept
{
	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		if (World::get().HasZombie(x, y - 2))
		{
			World::get().KillZombie(state, 2);
		}
		else if (World::get().HasHuman(x, y - 2))
		{
			World::get().KillHuman(state, 2);
		}
		break;
	case MachineState::DOWN:
		if (World::get().HasZombie(x, y + 2))
		{
			World::get().KillZombie(state, 2);
		}
		else if (World::get().HasHuman(x, y + 2))
		{
			World::get().KillHuman(state, 2);
		}
		break;
	case MachineState::LEFT:
		if (World::get().HasZombie(x - 2, y))
		{
			World::get().KillZombie(state, 2);
		}
		else if (World::get().HasHuman(x - 2, y))
		{
			World::get().KillHuman(state, 2);
		}
		break;
	case MachineState::RIGHT:
		if (World::get().HasZombie(x + 2, y))
		{
			World::get().KillZombie(state, 2);
		}
		else if (World::get().HasHuman(x + 2, y))
		{
			World::get().KillHuman(state, 2);
		}
		break;
	default:
		break;
	}
}