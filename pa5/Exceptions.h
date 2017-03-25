#pragma once
#include <exception>

class InvalidOp : public std::exception
{
	const char* what() const noexcept override { return "Invalid OP code or parameters."; }
};

class FileLoadExcept : public std::exception
{
public:
	const char* what() const noexcept override { return "ZOM file load exception"; }
};

class GoToExcept : public std::exception
{
	const char* what() const noexcept override { return "Cannot go to that location"; }
};
