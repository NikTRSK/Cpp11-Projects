#pragma once
#include "World.h"


World::World()
{
}


World::~World()
{
}

void World::AddZombie(const Machine<ZombieTraits> & zombie)
{
	mZombieStates.push_back(MachineState());
	mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(zombie));
	mZombies.back()->BindState(mZombieStates.back());
}

void World::GenerateZombies(const Machine<ZombieTraits> & zombie)
{
	for (auto i = 0; i < 20; ++i)
	{
		MachineState state;
		int x, y;
		do
		{
			x = rand() % 20;
			y = rand() % 20;
		} while (mGridZombies[x][y] != nullptr || mGridHumans[x][y] != nullptr);
		state.mCoordinate->x = x;
		state.mCoordinate->y = y;
//		std::cout << "X: " << x << " Y: " << y << std::endl;
		mZombieStates.push_back(state);
		mZombies.push_back(std::make_shared<Machine<ZombieTraits>>(zombie));
		mZombies.back()->BindState(mZombieStates.back());
//		mGridZombies[x][y] = mZombies.back();
		mGridZombies[x][y] = &mZombieStates.back();
	}
}

void World::GenerateHumans(const Machine<HumanTraits>& human)
{
	for (auto i = 0; i < 10; ++i)
	{
		MachineState state;
		int x, y;
		do
		{
			x = rand() % 20;
			y = rand() % 20;
		} while (mGridHumans[x][y] != nullptr || mGridZombies[x][y] != nullptr);
		state.mCoordinate->x = x;
		state.mCoordinate->y = y;
//		std::cout << "X: " << x << " Y: " << y << std::endl;
		mHumanStates.push_back(state);
		mHumans.push_back(std::make_shared<Machine<HumanTraits>>(human));
		mHumans.back()->BindState(mHumanStates.back());
//		mGridHumans[x][y] = mHumans.back();
		mGridHumans[x][y] = &mHumanStates.back();
	}
}


std::vector<std::shared_ptr<Machine<ZombieTraits>>> World::GetZombies() const
{
	return mZombies;
}

std::vector<std::shared_ptr<Machine<HumanTraits>>> World::GetHumans() const
{
	return mHumans;
}

std::vector<MachineState> World::GetZombieStates() const
{
	return mZombieStates;
}

std::vector<MachineState> World::GetHumanStates() const
{
	return mHumanStates;
}

void World::UpdateWorld()
{
	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		try
		{
			mZombies.at(i)->TakeTurn(mZombieStates.at(i));
		}
		//		catch(GoToExcept gte)
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
			//			wxMessageBox("SHIT", "Error", wxOK | wxICON_ERROR);
		}
	}

	for (unsigned int i = 0; i < mHumans.size(); ++i)
	{
		try
		{
			mHumans.at(i)->TakeTurn(mHumanStates.at(i));
		}
		//		catch(GoToExcept gte)
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
			//			wxMessageBox("SHIT", "Error", wxOK | wxICON_ERROR);
		}
	}
}

void World::ClearData()
{
	mZombies.clear();
	mZombieStates.clear();
	mHumanStates.clear();
	mHumans.clear();
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

void World::KillZombie(MachineState& state) noexcept
{
	// TODO:
	std::shared_ptr<Machine<ZombieTraits>> zombie;
	int x = state.GetX();
	int y = state.GetY();
	if (mGridZombies[x][y] == nullptr)
		return;
//	zombie = mGridZombies[x][y];
	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		if (mZombies[i] == zombie)
		{
			std::cout << "Killing zombie\n";
			mZombies.erase(mZombies.begin() + i);
			return;
		}
	}
}

void World::ConvertHuman(MachineState& state) noexcept
{
	// TODO:
	std::shared_ptr<Machine<HumanTraits>> human;
	int x = state.GetX();
	int y = state.GetY();
	if (mGridHumans[x][y] == nullptr)
		return;
//	human = mGridHumans[x][y];
	for (unsigned int i = 0; i < mHumans.size(); ++i)
	{
		if (mHumans[i] == human)
		{
			std::cout << "Converting human\n";
			human->BindState(state);
			mHumans.erase(mHumans.begin() + i);
			//			mZombies.push_back(human);
			return;
		}
	}
}

void World::PrintWorld() const noexcept
{
	std::cout << "Zombie Grid\n";
	for (unsigned int i = 0; i < 20; ++i)
	{
		for (unsigned int j = 0; j < 20; ++j)
		{
			if (mGridZombies[i][j] == nullptr) 
			{
				std::cout << "0" << " | ";
			}
			else 
			{
				std::cout << "Z" << " | ";
			}
		}
		std::cout << std::endl;
	}

	std::cout << "Human Grid\n";
	for (unsigned int i = 0; i < 20; ++i)
	{
		for (unsigned int j = 0; j < 20; ++j)
		{
			if (mGridHumans[i][j] == nullptr)
			{
				std::cout << "0" << " | ";
			}
			else
			{
				std::cout << "H" << " | ";
			}
		}
		std::cout << std::endl;
	}
}
