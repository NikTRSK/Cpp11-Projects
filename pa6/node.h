#pragma once

#include <list>
#include <vector>
#include <string>
#include <map>

struct CodeContext
{
	std::vector<std::string> mOps;
};

class Node
{
public:
	virtual void CodeGen(CodeContext& context) const = 0;
};

class NNumeric : public Node
{
public:
	NNumeric(std::string& value);
	int mvalue;
	virtual void CodeGen(CodeContext& context) const override { }
};

class NStatement : public Node
{
};

class NRotate : public NStatement
{
public:
	NRotate(NNumeric* dir);
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NForward : public NStatement
{
public:
	NForward();
	virtual void CodeGen(CodeContext& context) const override;
};

class NAttack : public NStatement
{
public:
	NAttack();
	virtual void CodeGen(CodeContext& context) const override;
};

class NRanged_Attack : public NStatement
{
public:
	NRanged_Attack();
	virtual void CodeGen(CodeContext& context) const override;
};

class NBoolean : public Node
{
};

class NIs_Zombie : public NBoolean
{
public:
	NIs_Zombie(NNumeric* dir);
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NIs_Human : public NBoolean
{
public:
	NIs_Human(NNumeric* dir);
	virtual void CodeGen(CodeContext& context) const override;
private:
	NNumeric* mDir;
};

class NIs_Wall : public NBoolean
{
public:
	NIs_Wall();
	virtual void CodeGen(CodeContext& context) const override;
};

class NIs_Passable : public NBoolean
{
public:
	NIs_Passable();
	virtual void CodeGen(CodeContext& context) const override;
};

class NIs_Random : public NBoolean
{
public:
	NIs_Random();
	virtual void CodeGen(CodeContext& context) const override;
};

class NGoto : public NStatement
{
public:
	NGoto();
	virtual void CodeGen(CodeContext& context) const override;
};

class NBlock : public Node
{
public:
	NBlock() { mbMainBlock = false; }
	virtual void CodeGen(CodeContext& context) const override;
	void AddStatement(NStatement* statement);
	void SetMainBlock() { mbMainBlock = true; }
private:
	std::list<NStatement*> mStatements;
	bool mbMainBlock;
};
