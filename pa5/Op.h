// Defines all the classes for different operations
// which can be performed.
#pragma once

struct MachineState;

// Abstract Base Class
class Op
{
public:
	Op(const char* OpName, int parameter)
		: mOpName(OpName)
		, mParam(parameter)
	{ }

	void DebugOutput(MachineState& state);
	void UpdateLocation(MachineState& state) const noexcept;
	bool TileIsOpen(MachineState &state) const noexcept;
	bool HasPlayer(MachineState &state) const noexcept;
	virtual void Execute(MachineState& state) = 0;
	
	virtual ~Op() { }
protected:
	int mParam;
	const char* mOpName;
};

// Defines the rotate operation
// Param of 0 means rotate clockwise
// Param of 1 means rotate counter-clockwise
struct OpRotate : Op
{
	OpRotate(int parameter)
		: Op("OpRotate", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the goto operation
// Parameter determines the line number
struct OpGoto : Op
{
	OpGoto(int parameter)
		: Op("OpGoto", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the attack operation
// Parameter determines the line number
struct OpAttack : Op
{
	OpAttack(int parameter)
		: Op("OpAttack", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
	void Attack(MachineState& state) const noexcept;
};

// Defines the ranged attack operation
// Parameter determines the line number
struct OpRangedAttack : Op
{
	OpRangedAttack(int parameter)
		: Op("OpRangedAttack", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
	void Attack(MachineState& state) const noexcept;
};

// Defines the forward operation
// Parameter determines the line number
struct OpForward : Op
{
	OpForward(int parameter)
		: Op("OpForward", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the forward operation
// Parameter determines the line number
struct OpEndturn : Op
{
	OpEndturn(int parameter)
		: Op("OpEndturn", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the endturn operation
// Parameter determines the line number
struct OpTestHuman : Op
{
	OpTestHuman(int parameter)
		: Op("OpTestHuman", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the test_wall operation
// Parameter determines the line number
struct OpTestWall : Op
{
	OpTestWall(int parameter)
		: Op("OpTestWall", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the test_zombie operation
// Parameter determines the line number
struct OpTestZombie : Op
{
	OpTestZombie(int parameter)
		: Op("OpTestZombie", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the test_random operation
// Parameter determines the line number
struct OpTestRandom : Op
{
	OpTestRandom(int parameter)
		: Op("OpTestRandom", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the test_passable operation
// Parameter determines the line number
struct OpTestPassable : Op
{
	OpTestPassable(int parameter)
		: Op("OpTestPassable", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the je operation
// Parameter determines the line number
struct OpJE : Op
{
	OpJE(int parameter)
		: Op("OpJE", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};

// Defines the jne operation
// Parameter determines the line number
struct OpJNE : Op
{
	OpJNE(int parameter)
		: Op("OpJNE", parameter)
	{ }

	virtual void Execute(MachineState& state) override;
};