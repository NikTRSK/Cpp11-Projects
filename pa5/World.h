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
	void ConvertHuman(const MachineState& state) noexcept;
	/* We can only kill/convert after everyone has turned */
	void DeleteKilledZombies() noexcept;
	void DeleteKilledHumans() noexcept;
	void ConvertHumans() noexcept;

private:

	// Machines
	Machine<ZombieTraits> mZombieMachine;
	Machine<HumanTraits> mHumanMachine;

	/* Note I'm using regular pointers since it's easier to manage that smart ptrs.
	   Had issues with smart pointers for some reason */
	// Players
	std::vector<MachineState*> mZombies;
	std::vector<MachineState*> mHumans;

	// Using this to avoid invalidating the vector iterator
	std::vector<MachineState*> mDeleteAfterTurn;
	std::vector<MachineState*> mHumansToTurn;

public:
	// The grid is public since there is really no need for encapsulation
	MachineState* mGridZombies[20][20];
	MachineState* mGridHumans[20][20];
};

