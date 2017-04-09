#include "node.h"
#include <sstream>
#include <iostream>

void NBlock::AddStatement(NStatement* statement)
{
	mStatements.push_back(statement);
}

void NBlock::CodeGen(CodeContext& context) const
{
	// TODO: Loop through statements in list and code gen them
	for (const auto &statement : mStatements)
	{
		std::cout << "Adding statement\n";
		statement->CodeGen(context);
	}
	if (mbMainBlock)
	{
		NStatement * goto_begin = new NGoto();
		goto_begin->CodeGen(context);
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

NGoto::NGoto()
{
}

void NGoto::CodeGen(CodeContext& context) const
{
	context.mOps.push_back("goto,1");
}


NIf::NIf(NBoolean* condition, NBlock* ifBlock, NBlock* elseBlock)
	:mCondition(condition), mIfBlock(ifBlock), mElseBlock(elseBlock)
{
}

void NIf::CodeGen(CodeContext& context) const
{
	// Push in condition
	mCondition->CodeGen(context);

	// Create JE block for If
	context.mOps.push_back("je,");
	int JEPosition = context.mOps.size() - 1;

	// Else block
	mElseBlock->CodeGen(context);

	// Create goto for passing if block
	context.mOps.push_back("goto,");
	int gotoPos = context.mOps.size();

	// If block
	int ifBeginPos = context.mOps.size() + 1;

	mIfBlock->CodeGen(context);
	int ifEndPos = context.mOps.size() + 1;

	// Flip if and else
	context.mOps.at(JEPosition) += std::to_string(ifBeginPos); // Append je line number
	context.mOps.at(gotoPos - 1) += std::to_string(ifEndPos); // Append goto for jump line number
}