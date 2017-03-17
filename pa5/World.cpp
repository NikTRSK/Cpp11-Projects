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

std::vector<std::shared_ptr<Machine<ZombieTraits>>> World::GetZombies() const
{
	return mZombies;
}

std::vector<MachineState> World::GetZombieStates() const
{
	return mZombieStates;
}

void World::UpdateWorld()
{
	for (unsigned int i = 0; i < mZombies.size(); ++i)
	{
		mZombies.at(i)->TakeTurn(mZombieStates.at(i));
	}
}
