#include "node.h"
#include <sstream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	// TODO: Loop through statements in list and code gen them
	for (const auto &statement : mStatements)
	{
		statement->CodeGen(context);
	}
}

NNumeric::NNumeric(std::string& value)
{
	mvalue = std::atoi(value.c_str());
}

NRotate::NRotate(NNumeric* dir)
	: mDir(dir)
{
}

void NRotate::CodeGen(CodeContext& context) const
{
	if (mDir->mvalue == 0)
	{
		context.mOps.push_back("rotate,0");
	}
	else if (mDir->mvalue == 1)
	{
		context.mOps.push_back("rotate,1");
	}
}

NForward::NForward()
{
}

void NForward::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("forward");
}

NAttack::NAttack()
{
}

void NAttack::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("attack");
}

NRanged_Attack::NRanged_Attack()
{
}

void NRanged_Attack::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("ranged_attack");
}

NIs_Zombie::NIs_Zombie(NNumeric* dir)
	:mDir(dir)
{
}

void NIs_Zombie::CodeGen(CodeContext& context) const
{
	// Should we handle wrong values here?
	if (mDir->mvalue == 1)
	{
		context.mOps.push_back("is_zombie,1");
	}
	else if (mDir->mvalue == 2)
	{
		context.mOps.push_back("is_zombie,2");
	}
}

NIs_Human::NIs_Human(NNumeric* dir)
	:mDir(dir)
{
}

void NIs_Human::CodeGen(CodeContext& context) const
{
	if (mDir->mvalue == 1)
	{
		context.mOps.push_back("is_human,1");
	}
	else if (mDir->mvalue == 2)
	{
		context.mOps.push_back("is_human,2");
	}
}

NIs_Wall::NIs_Wall()
{
}

void NIs_Wall::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("is_wall");
}

NIs_Passable::NIs_Passable()
{
}

void NIs_Passable::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("is_passable");
}

NIs_Random::NIs_Random()
{
}

void NIs_Random::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("is_random");
}
