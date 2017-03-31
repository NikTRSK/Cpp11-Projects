#include "World.h"
#include "Exceptions.h"
#include <random>
#include <wx/msgdlg.h>
//#include <wx/defs.h>



World::World()
{
	srand(static_cast<unsigned int>(time(nullptr)));
}


World::~World()
{
}

void World::SetZombieMachine(Machine<ZombieTraits> zombieMachine) noexcept
{
	mZombieMachine = zombieMachine;
}

void World::SetHumanMachine(Machine<HumanTraits> humanMachine) noexcept
{
	mHumanMachine = humanMachine;
}

void World::GenerateZombies() noexcept
{
//	MachineState *state = new MachineState;
//	state->mFacing = MachineState::RIGHT;
//	int x = 5, y = 5;
//	state->mCoordinate->x = x;
//	state->mCoordinate->y = y;
//	mZombieMachine.BindState(*state);
//	mZombies.push_back(state);
//	mGridZombies[x][y] = state;
//
//	state = new MachineState;
//	state->mFacing = MachineState::LEFT;
//	x = 6, y = 5;
//	state->mCoordinate->x = x;
//	state->mCoordinate->y = y;
//	mZombieMachine.BindState(*state);
//	mZombies.push_back(state);
//	mGridZombies[x][y] = state;

	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  direction(0, 3);
	std::uniform_int_distribution<int>  position(0, 19);

	MachineState* state;
	for (auto i = 0; i < 20; ++i)
	{
		state = new MachineState;
		state->mFacing = static_cast<MachineState::Facing>(direction(generator));
		int x, y;
		do
		{
			x = position(generator);
			y = position(generator);
		} while (mGridZombies[x][y] != nullptr || mGridHumans[x][y] != nullptr);
		state->mCoordinate->x = x;
		state->mCoordinate->y = y;
		mZombieMachine.BindState(*state);
		mZombies.push_back(state);
		mGridZombies[x][y] = state;
	}
}

void World::GenerateHumans() noexcept
{
//	MachineState *state = new MachineState;
//	state->mFacing = MachineState::LEFT;
//	int x = 6, y = 4;
//	state->mCoordinate->x = x;
//	state->mCoordinate->y = y;
//	mHumanMachine.BindState(*state);
//	mHumans.push_back(state);
//	mGridHumans[x][y] = state;

	std::random_device                  rand_dev;
	std::mt19937                        generator(rand_dev());
	std::uniform_int_distribution<int>  direction(0, 3);
	std::uniform_int_distribution<int>  position(0, 19);
	
	MachineState* state;
	for (auto i = 0; i < 10; ++i)
	{
		state = new MachineState;
		state->mFacing = static_cast<MachineState::Facing>(direction(generator));
		int x, y;
		do
		{
			x = position(generator);
			y = position(generator);
		} while (mGridZombies[x][y] != nullptr || mGridHumans[x][y] != nullptr);
		state->mCoordinate->x = x;
		state->mCoordinate->y = y;
		mHumanMachine.BindState(*state);
		mHumans.push_back(state);
		mGridHumans[x][y] = state;
	}
}


std::vector<MachineState*> World::GetZombies() const noexcept
{
	return mZombies;
}

std::vector<MachineState*> World::GetHumans() const noexcept
{
	return mHumans;
}

void World::UpdateWorld() noexcept
{
	// We get the size ahead of time to avoid the problem of converting a human
//	MachineState *deleteIfInvalidMove = nullptr;

//	std::for_each(mZombies.begin(), mZombies.end(), [this](auto z)
//	{
//		mZombieMachine.TakeTurn(**z);
//	});
//	for (auto z = mZombies.begin(); z != mZombies.end(); ++z)
//	{
//		mZombieMachine.TakeTurn(**z);
//	}

//	for (unsigned int z = 0; z < mZombies.size(); ++z)
//	{
//		mZombieMachine.TakeTurn(*mZombies[z]);
//	}

	

/*	for (int i = mDeleteAfterTurn.size()-1; i >= 0; --i)
	{
		for (unsigned int j = 0; j < mZombies.size(); ++j)
//		for (auto z = mZombies.begin(); z != mZombies.end(); ++z)
		{
			if (mZombies[j] == mDeleteAfterTurn[i])
			{
				mZombies.erase(mZombies.begin()+j);
			}
//			delete mDeleteAfterTurn[i];
//			mDeleteAfterTurn.pop_back();
		}
	}*/
	MachineState *deleteIfInvalidMove = nullptr;
	for (auto z = mZombies.begin(); z != mZombies.end();)
	{
		try
		{
			mZombieMachine.TakeTurn(**z);
		}
		catch (const InvalidOp &e)
		{
			std::cerr << e.what() << std::endl;
			deleteIfInvalidMove = *z;
		}
		catch (const GoToExcept &e)
		{
			std::cerr << e.what() << std::endl;
			deleteIfInvalidMove = *z;
		}
		catch (const RangedAttackExcept &e)
		{
			std::cerr << e.what() << std::endl;
			deleteIfInvalidMove = *z;
		}
		if (deleteIfInvalidMove)
		{
			mGridZombies[(*z)->GetX()][(*z)->GetY()] = nullptr;
			z = mZombies.erase(z);
			deleteIfInvalidMove = nullptr;
		}
		else
		{
			++z;
		}
	}

	DeleteKilledZombies();

	deleteIfInvalidMove = nullptr;
	for (auto h = mHumans.begin(); h != mHumans.end();)
	{
		try
		{
			mHumanMachine.TakeTurn(**h);
		}
		catch (const InvalidOp &e)
		{
			std::cerr << e.what() << std::endl;
			deleteIfInvalidMove = *h;
		}
		catch (const GoToExcept &e)
		{
			std::cerr << e.what() << std::endl;
			deleteIfInvalidMove = *h;
		}
		catch (const RangedAttackExcept &e)
		{
			std::cerr << e.what() << std::endl;
			deleteIfInvalidMove = *h;
		}
		if (deleteIfInvalidMove)
		{
			mGridHumans[(*h)->GetX()][(*h)->GetY()] = nullptr;
			h = mHumans.erase(h);
			deleteIfInvalidMove = nullptr;
		}
		else
		{
			++h;
		}
	}

	DeleteKilledHumans();
}

void World::ClearData() noexcept
{
	// Delete all states & clear vectors
	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		delete mZombies[i];
	}
	mZombies.clear();
	
	for (unsigned int i = 0; i < mHumans.size(); ++i)
	{
		delete mHumans[i];
	}
	mHumans.clear();
	// Clear grid
	for (auto i = 0; i < mWorldSize; ++i)
	{
		for (auto j = 0; j < mWorldSize; ++j)
		{
			mGridZombies[i][j] = nullptr;
			mGridHumans[i][j] = nullptr;
		}
	}
}

bool World::HasHuman(const int& x, const int& y) const noexcept
{
	return mGridHumans[x][y] != nullptr;
}

bool World::HasZombie(const int& x, const int& y) const noexcept
{
	return mGridZombies[x][y] != nullptr;
}

void World::KillZombie(const MachineState& state, int offset) noexcept
{
	MachineState *zombieToKill = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		zombieToKill = mGridZombies[x][y - offset];
		break;
	case MachineState::DOWN:
		zombieToKill = mGridZombies[x][y + offset];
		break;
	case MachineState::LEFT:
		zombieToKill = mGridZombies[x - offset][y];
		break;
	case MachineState::RIGHT:
		zombieToKill = mGridZombies[x + offset][y];
		break;
	default:
		break;
	}

	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		if (mZombies[i] == zombieToKill)
		{
			mDeleteAfterTurn.push_back(mZombies[i]);
			mGridZombies[zombieToKill->GetX()][zombieToKill->GetY()] = nullptr;
			//			zombieToKill = nullptr;
			return;
		}
	}
}

void World::KillHuman(const MachineState& state, int offset) noexcept
{
	MachineState *humanToKill = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToKill = mGridHumans[x][y - offset];
		break;
	case MachineState::DOWN:
		humanToKill = mGridHumans[x][y + offset];
		break;
	case MachineState::LEFT:
		humanToKill = mGridHumans[x - offset][y];
		break;
	case MachineState::RIGHT:
		humanToKill = mGridHumans[x + offset][y];
		break;
	default:
		break;
	}

	for (unsigned int i = 0; i < mHumans.size(); ++i)
	{
		if (mHumans[i] == humanToKill)
		{
			mDeleteAfterTurn.push_back(mHumans[i]);
			mGridHumans[humanToKill->GetX()][humanToKill->GetY()] = nullptr;
			//			zombieToKill = nullptr;
			return;
		}
	}
}

void World::ConvertHuman(MachineState& state) noexcept
{
	MachineState *humanToConvert = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToConvert = mGridHumans[x][y - 1];
		mGridHumans[x][y - 1] = nullptr;
		break;
	case MachineState::DOWN:
		humanToConvert = mGridHumans[x][y + 1];
		mGridHumans[x][y + 1] = nullptr;
		break;
	case MachineState::LEFT:
		humanToConvert = mGridHumans[x - 1][y];
		mGridHumans[x - 1][y] = nullptr;
		break;
	case MachineState::RIGHT:
		humanToConvert = mGridHumans[x + 1][y];
		mGridHumans[x + 1][y] = nullptr;
		break;
	default:
		break;
	}

	for (unsigned int i = 0; i < mHumans.size(); ++i)
	{
		if (mHumans[i] == humanToConvert)
		{
			mZombieMachine.BindState(*humanToConvert);
			mZombies.push_back(humanToConvert);
			mHumans.erase(mHumans.begin() + i);	
			mGridZombies[humanToConvert->GetX()][humanToConvert->GetY()] = humanToConvert;
			return;
		}
	}
}

void World::DeleteKilledZombies() noexcept
{
	for (int i = mDeleteAfterTurn.size() - 1; i >= 0; --i)
	{
		for (unsigned int j = 0; j < mZombies.size(); ++j)
		{
			if (mZombies[j] == mDeleteAfterTurn[i])
			{
				mZombies.erase(mZombies.begin() + j);
			}
		}
	}

	for (auto toDel = mDeleteAfterTurn.begin(); toDel != mDeleteAfterTurn.end(); ++toDel)
	{
		delete *toDel;
	}
	mDeleteAfterTurn.clear();
}

void World::DeleteKilledHumans() noexcept
{
	for (int i = mDeleteAfterTurn.size() - 1; i >= 0; --i)
	{
		for (unsigned int j = 0; j < mHumans.size(); ++j)
		{
			if (mHumans[j] == mDeleteAfterTurn[i])
			{
				mHumans.erase(mHumans.begin() + j);
			}
		}
	}

	for (auto toDel = mDeleteAfterTurn.begin(); toDel != mDeleteAfterTurn.end(); ++toDel)
	{
		delete *toDel;
	}
	mDeleteAfterTurn.clear();
}
