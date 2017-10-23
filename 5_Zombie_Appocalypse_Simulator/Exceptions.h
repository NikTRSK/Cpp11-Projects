#pragma once
#include <exception>

class InvalidOp : public std::exception
{
public:
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};

class FileLoadExcept : public std::exception
{
public:
	const char* what() const noexcept override { return "ZOM file load exception"; }
};

class GoToExcept : public std::exception
{
public:
	const char* what() const noexcept override { return "Cannot go to that location"; }
};

class RangedAttackExcept : public std::exception
{
public:
	const char* what() const noexcept override { return "Zombies cannot perform ranged attacks"; }
};
