#pragma once
#include <vector>
#include "Singleton.h"
#include "Traits.h"
#include "Machine.h"

#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;

class World : public Singleton<World>
{
public:
	World();
	~World();
	void SetZombieMachine(Machine<ZombieTraits> zombieMachine) noexcept;
	void SetHumanMachine(Machine<HumanTraits> humanMachine) noexcept;
	void GenerateZombies() noexcept;
	void GenerateHumans() noexcept;
	std::vector<MachineState*> GetZombies() const noexcept;
	std::vector<MachineState*> GetHumans() const noexcept;
	void UpdateWorld() noexcept;
	void ClearData() noexcept;
	bool HasHuman(const int& x, const int& y) const noexcept;
	bool HasZombie(const int& x, const int& y) const noexcept;
	void KillZombie(const MachineState& state, int offset) noexcept;
	void KillHuman(const MachineState& state, int offset) noexcept;
	void ConvertHuman(MachineState& state) noexcept;
	void DeleteKilledZombies() noexcept;
	void DeleteKilledHumans() noexcept;

private:
	const int mWorldSize = 20;

	// Machines
	Machine<ZombieTraits> mZombieMachine;
	Machine<HumanTraits> mHumanMachine;

	// Players
	std::vector<MachineState*> mZombies;
	std::vector<MachineState*> mHumans;

	std::vector<MachineState*> mDeleteAfterTurn;

public:
	MachineState* mGridZombies[20][20];
	MachineState* mGridHumans[20][20];
};

