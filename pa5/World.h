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
	void GenerateZombies(const Machine<ZombieTraits> & zombie);
	void GenerateHumans(const Machine<HumanTraits> & human);
	std::vector<std::shared_ptr<Machine<ZombieTraits>>> GetZombies() const;
	std::vector<std::shared_ptr<Machine<HumanTraits>>> GetHumans() const;
	std::vector<MachineState> GetZombieStates() const;
	std::vector<MachineState> GetHumanStates() const;
	void UpdateWorld();
	void ClearData();
	bool HasHuman(const int& x, const int& y) const noexcept;
	bool HasZombie(const int& x, const int& y) const noexcept;
	void KillZombie(MachineState& state) noexcept;
	void ConvertHuman(MachineState& state) noexcept;

private:
	const int mWorldSize = 20;

	std::vector<std::shared_ptr<Machine<ZombieTraits>>> mZombies;
	std::vector<MachineState> mZombieStates;
	std::vector<std::shared_ptr<Machine<HumanTraits>>> mHumans;
	std::vector<MachineState> mHumanStates;

	std::shared_ptr<Machine<ZombieTraits>> mGridZombies[20][20];
	std::shared_ptr<Machine<HumanTraits>> mGridHumans[20][20];
};

