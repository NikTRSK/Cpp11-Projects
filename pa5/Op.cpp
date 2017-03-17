#include "Op.h"
#include "Machine.h"
#include <iostream>

// Output state information for debugging purposes
void Op::DebugOutput(MachineState& state)
{
	std::cout << state.mProgramCounter << ":" << mOpName << "," << mParam
			  << " | Loc: " << state.GetX() << ", " << state.GetY() << std::endl;
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

	/*std::cout << "Now facing ";
	switch (state.mFacing)
	{
	case (MachineState::UP):
		std::cout << "UP";
		break;
	case (MachineState::RIGHT):
		std::cout << "RIGHT";
		break;
	case (MachineState::DOWN):
		std::cout << "DOWN";
		break;
	case (MachineState::LEFT):
		std::cout << "LEFT";
		break;
	}
	std::cout << std::endl;*/
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpGoto::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter = mParam;
	// why doesn't go to increment action count
}

void OpAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	state.UpdateLocation();
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpRangedAttack::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpForward::Execute(MachineState& state)
{
	DebugOutput(state);
	state.UpdateLocation();
	std::cout << "new loc: " << state.GetX() << ", " << state.GetY() << std::endl;
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpEndturn::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestHuman::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestWall::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mTest = state.GetRandomBool();
	// If facing a wall set mTest to true
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestZombie::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestRandom::Execute(MachineState& state)
{
	DebugOutput(state);
	state.mProgramCounter++;
	state.mActionsTaken++;
}

void OpTestPassable::Execute(MachineState& state)
{
	DebugOutput(state);
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
	//	state.mProgramCounter = mParam;
}