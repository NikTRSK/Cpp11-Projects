#include "World.h"
#include "Exceptions.h"
#include <random>
#include <wx/msgdlg.h>

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
		std::pair<int, int> c;
		do
		{
			x = position(generator);
			y = position(generator);
			c = std::make_pair(x, y);
		} while (mGrid.find(c) != mGrid.end());
		state->mCoordinate->x = x;
		state->mCoordinate->y = y;
		mZombieMachine.BindState(*state);
		mZombies.push_back(state);
		mGrid[c] = state;
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
		std::pair<int, int> c;
		do
		{
			x = position(generator);
			y = position(generator);
			c = std::make_pair(x, y);
		} while (mGrid.find(c) != mGrid.end());
		state->mCoordinate->x = x;
		state->mCoordinate->y = y;
		mHumanMachine.BindState(*state);
		mHumans.push_back(state);
		mGrid[c] = state;
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

	for (auto z = mZombies.begin(); z != mZombies.end(); ++z)
	{
		// Not popping up a message box on exception since it's really distracting
		try
		{
			if (*z != nullptr)
			{
				mZombieMachine.TakeTurn(**z);
			}
		}
		catch (const InvalidOp &e)
		{
			std::cerr << e.what() << std::endl;
			KillZombie(**z, 0);
		}
		catch (const GoToExcept &e)
		{
			std::cerr << e.what() << std::endl;
			KillZombie(**z, 0);
		}
		catch (const RangedAttackExcept &e)
		{
			std::cerr << e.what() << std::endl;
			KillZombie(**z, 0);
		}
		// DEBUG ONLY. Should never fire
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// After the turn Kill zombies and Convert humans
	ConvertHumans();
	DeleteKilledZombies();

	for (auto h = mHumans.begin(); h != mHumans.end(); ++h)
	{
		try
		{
			if (*h != nullptr)
			{
				mHumanMachine.TakeTurn(**h);
			}
		}
		catch (const InvalidOp &e)
		{
			std::cerr << e.what() << std::endl;
			KillHuman(**h, 0);
		}
		catch (const GoToExcept &e)
		{
			std::cerr << e.what() << std::endl;
			KillHuman(**h, 0);
		}
		catch (const RangedAttackExcept &e)
		{
			std::cerr << e.what() << std::endl;
			KillHuman(**h, 0);
		}
		// DEBUG ONLY. Should never fire
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	// After the turn Kill humans & zombies
	DeleteKilledHumans();
	DeleteKilledZombies();
}

void World::ClearData() noexcept
{
	// Clear vectors
	mZombies.clear();
	for(unsigned int i = 0; i < mZombies.size(); ++i)
	{
		delete mZombies[i];
	}

	for(unsigned int i = 0; i < mHumans.size(); ++i)
	{
		delete mHumans[i];
	}
	mHumans.clear();

	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		delete mDeleteAfterTurn[i];
	}
	mDeleteAfterTurn.clear();

	for (unsigned int i = 0; i < mHumansToTurn.size(); ++i)
	{
		delete mHumansToTurn[i];
	}
	mHumansToTurn.clear();

	mGrid.clear();
}

bool World::HasHuman(const int& x, const int& y) const noexcept
{
	auto it = mGrid.find(std::pair<int, int>(x, y));
	return it != mGrid.end() && it->second->GetInfect() != true;
}

bool World::HasZombie(const int& x, const int& y) const noexcept
{
	auto it = mGrid.find(std::pair<int, int>(x, y));
	return it != mGrid.end() && it->second->GetInfect() == true;
}

void World::KillZombie(const MachineState& state, int offset) noexcept
{
	std::map<std::pair<int, int>, MachineState*>::iterator zombieToKill;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		zombieToKill = mGrid.find(std::pair<int, int>(x, y - offset));
		break;
	case MachineState::DOWN:
		zombieToKill = mGrid.find(std::pair<int, int>(x, y + offset));
		break;
	case MachineState::LEFT:
		zombieToKill = mGrid.find(std::pair<int, int>(x - offset, y));
		break;
	case MachineState::RIGHT:
		zombieToKill = mGrid.find(std::pair<int, int>(x + offset, y));
		break;
	default:
		break;
	}

	if (zombieToKill != mGrid.end())
	{
		for (unsigned int i = 0; i < mZombies.size(); ++i)
		{
			// Only invalidate the killed zombie from the board.
			// We actually remove it from zombies later.
			if (mZombies[i] == zombieToKill->second)
			{
				mDeleteAfterTurn.push_back(mZombies[i]);
				mGrid.erase(zombieToKill);
				return;
			}
		}
	}
}

void World::KillHuman(const MachineState& state, int offset) noexcept
{
	std::map<std::pair<int, int>, MachineState*>::iterator humanToKill;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToKill = mGrid.find(std::pair<int, int>(x, y - offset));
		break;
	case MachineState::DOWN:
		humanToKill = mGrid.find(std::pair<int, int>(x, y + offset));
		break;
	case MachineState::LEFT:
		humanToKill = mGrid.find(std::pair<int, int>(x - offset, y));
		break;
	case MachineState::RIGHT:
		humanToKill = mGrid.find(std::pair<int, int>(x + offset, y));
		break;
	default:
		break;
	}

	if (humanToKill != mGrid.end())
	{
		for (unsigned int i = 0; i < mHumans.size(); ++i)
		{
			// Only invalidate the killed zombie from the board.
			// We actually remove it from zombies later.
			if (mHumans[i] == humanToKill->second)
			{
				mDeleteAfterTurn.push_back(mHumans[i]);
				mGrid.erase(humanToKill);
				return;
			}
		}
	}
}

void World::ConvertHuman(const MachineState& state) noexcept
{
	std::map<std::pair<int, int>, MachineState*>::iterator humanToConvert;

	int x = state.GetX();
	int y = state.GetY();
	switch (state.mFacing)
	{
	case MachineState::UP:
		humanToConvert = mGrid.find(std::pair<int, int>(x, y - 1));
		break;
	case MachineState::DOWN:
		humanToConvert = mGrid.find(std::pair<int, int>(x, y + 1));
		break;
	case MachineState::LEFT:
		humanToConvert = mGrid.find(std::pair<int, int>(x - 1, y));
		break;
	case MachineState::RIGHT:
		humanToConvert = mGrid.find(std::pair<int, int>(x + 1, y));
		break;
	default:
		break;
	}

	if (humanToConvert != mGrid.end())
	{
		for (unsigned int i = 0; i < mHumans.size(); ++i)
		{
			if (mHumans[i] == humanToConvert->second)
			{
				// We only invalidate the human from the board.
				// We remove it after it is converted
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
				delete mDeleteAfterTurn[i];
				break;
			}
		}
	}
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
				delete mDeleteAfterTurn[i];
				break;
			}
		}
	}
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
				mZombies.push_back(mHumansToTurn[i]);
				mZombieMachine.BindState(*mZombies.back());
			}
		}
	}
}
