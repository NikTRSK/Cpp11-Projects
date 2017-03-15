#pragma once
#include "Singleton.h"
#include <vector>
#include "Machine.h"
#include "Traits.h"

#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

class World : public Singleton<World>
{
public:
	World();
	~World();
//	void Attack()

private:
	const int mWorldSize = 20;

	std::vector<std::shared_ptr<Machine<ZombieTraits>>> mZombies;
	std::vector<std::shared_ptr<Machine<HumanTraits>>> mHumans;
};

