#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "Traits.h"
#include "Machine.h"

#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

class World : public Singleton<World>
{
public:
	World();
	~World();
	void AddZombie(const Machine<ZombieTraits> & zombie);
	std::vector<std::shared_ptr<Machine<ZombieTraits>>> GetZombies() const;
	std::vector<MachineState> GetZombieStates() const;
	void UpdateWorld();

private:
	const int mWorldSize = 20;

	std::vector<std::shared_ptr<Machine<ZombieTraits>>> mZombies;
	std::vector<MachineState> mZombieStates;
	std::vector<std::shared_ptr<MachineState>> mHumanStates;
	std::vector<std::shared_ptr<Machine<HumanTraits>>> mHumans;
};

