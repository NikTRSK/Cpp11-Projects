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
	ClearData();
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
	// Use uniform distribution instead of rand for accurate randomness.
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
	// Use uniform distribution instead of rand for accurate randomness.
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
	/*	To avoid invalidating the iterator we have to
	    conditionally advance depending on whether an
		exception has been thrown or not */
	MachineState *deleteIfInvalidMove = nullptr;
	for (auto z = mZombies.begin(); z != mZombies.end();)
	{
		// Not popping up a message box on exception since it's really distracting
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

	// After the turn Kill zombies and Convert humans
	ConvertHumans(); 
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

	// After the turn Kill humans & zombies
	DeleteKilledHumans();
	DeleteKilledZombies();
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
	for (auto i = 0; i < 20; ++i)
	{
		for (auto j = 0; j < 20; ++j)
		{
			mGridZombies[i][j] = nullptr;
			mGridHumans[i][j] = nullptr;
		}
	}
	mDeleteAfterTurn.clear();
	mHumansToTurn.clear();
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

	if (zombieToKill)
	{
		mGridZombies[zombieToKill->GetX()][zombieToKill->GetY()] = nullptr;

		for (unsigned int i = 0; i < mZombies.size(); ++i)
		{
			// Only invalidate the killed zombie from the board.
			// We actually remove it from zombies later.
			if (mZombies[i] == zombieToKill)
			{
				mDeleteAfterTurn.push_back(mZombies[i]);
				return;
			}
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

	if (humanToKill)
	{
		mGridHumans[humanToKill->GetX()][humanToKill->GetY()] = nullptr;

		for (unsigned int i = 0; i < mHumans.size(); ++i)
		{
			// Only invalidate the killed human from the board.
			// We actually remove it from humans later.
			if (mHumans[i] == humanToKill)
			{
				mDeleteAfterTurn.push_back(mHumans[i]);
				return;
			}
		}
	}
}

void World::ConvertHuman(const MachineState& state) noexcept
{
	MachineState *humanToConvert = nullptr;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToConvert = mGridHumans[x][y - 1];
		break;
	case MachineState::DOWN:
		humanToConvert = mGridHumans[x][y + 1];
		break;
	case MachineState::LEFT:
		humanToConvert = mGridHumans[x - 1][y];
		break;
	case MachineState::RIGHT:
		humanToConvert = mGridHumans[x + 1][y];
		break;
	default:
		break;
	}

	if (humanToConvert)
	{
		mGridHumans[humanToConvert->GetX()][humanToConvert->GetY()] = nullptr;
		mGridZombies[humanToConvert->GetX()][humanToConvert->GetY()] = humanToConvert;

		for (unsigned int i = 0; i < mHumans.size(); ++i)
		{
			if (mHumans[i] == humanToConvert)
			{
				// We only invalidate the human from the board.
				// We remove it after it is 
				mHumansToTurn.push_back(mHumans[i]);
				return;
			}
		}
	}
}

void World::DeleteKilledZombies() noexcept
{
	// Remove all zombies that were killed this turn
	for (int i = mDeleteAfterTurn.size() - 1; i >= 0; --i)
	{
		for (unsigned int j = 0; j < mZombies.size(); ++j)
		{
			if (mZombies[j] == mDeleteAfterTurn[i])
			{
				mZombies.erase(mZombies.begin() + j);
//				delete mGridZombies[mDeleteAfterTurn[i]->GetX()][mDeleteAfterTurn[i]->GetY()];
			}
		}
	}

	mDeleteAfterTurn.clear();
}

void World::DeleteKilledHumans() noexcept
{
	// Remove all humans that were killed this turn
	for (int i = mDeleteAfterTurn.size() - 1; i >= 0; --i)
	{
		for (unsigned int j = 0; j < mHumans.size(); ++j)
		{
			if (mHumans[j] == mDeleteAfterTurn[i])
			{
				mHumans.erase(mHumans.begin() + j);
//				delete mGridHumans[mDeleteAfterTurn[i]->GetX()][mDeleteAfterTurn[i]->GetY()];
			}
		}
	}

	mDeleteAfterTurn.clear();
}

void World::ConvertHumans() noexcept
{
	for (int i = mHumansToTurn.size() - 1; i >= 0; --i)
	{
		for (unsigned int j = 0; j < mHumans.size(); ++j)
		{
			if (mHumans[j] == mHumansToTurn[i])
			{
				mHumans.erase(mHumans.begin() + j);
			}
		}
		mZombieMachine.BindState(*mHumansToTurn[i]);
		mZombies.push_back(mHumansToTurn[i]);
		mGridZombies[mHumansToTurn[i]->GetX()][mHumansToTurn[i]->GetY()] = mZombies.back();
	}

	mHumansToTurn.clear();
}
