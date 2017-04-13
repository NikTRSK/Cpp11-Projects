// zompiler.cpp : Defines the entry point for the console application.
//

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "node.h"
#include <fstream>

extern int g_LineNumber;
extern NBlock* g_MainBlock;

extern int zompilerparse();
extern FILE* zompilerin;

// Performs optimizations on goto statements
void OptimizeGoTo(CodeContext & codeContext) noexcept;
// Writes the zomsembly code to an output file
void WriteToOutputFile(const CodeContext & codeContext) noexcept;

int main(int argc, char* argv[])
{
	// Read input from the first command line parameter
	zompilerin = fopen(argv[1], "r");
	
	// Start the parse
	zompilerparse();
	
	// CodeGen from g_MainBlock
	if (g_MainBlock)
	{
		CodeContext codeContext;
		g_MainBlock->CodeGen(codeContext);

		OptimizeGoTo(codeContext);

		WriteToOutputFile(codeContext);
	}
	
	// Close the file stream
	fclose(zompilerin);

	return 0;
}

void zompilererror(const char* s)
{
	std::cout << s << " on line " << g_LineNumber << std::endl;
}

void OptimizeGoTo(CodeContext & codeContext) noexcept
{
//	for (const auto &gt : codeContext.mGoto)
//	{
//		codeContext.mOps.at(gt.first - 1) = "goto,1";
//	}
	// Iterate over the map and find a goto chain.
	int gotoEnd = -1;
	auto gotoPos = codeContext.mGoto.begin();
	while (gotoPos != codeContext.mGoto.end() && gotoEnd == -1)
	{
		auto it = gotoPos;
		while ((it = codeContext.mGoto.find(it->second)) != codeContext.mGoto.end())
		{
			gotoEnd = it->second;
		}
		++gotoPos;
	}
	// If gotoEnd is -1 we know we don't have a chain so we can keep going.
	if (gotoEnd == -1)
		return;

	// Finally replace the gotos from the chain.
	for (const auto &gt : codeContext.mGoto)
	{
		if (gt.second == gotoEnd)
		{
			break;
		}
	
		codeContext.mOps.at(gt.first - 1) = "goto," + std::to_string(gotoEnd);
	}
}

void WriteToOutputFile(const CodeContext & codeContext) noexcept
{
	FILE * output = fopen("out.zom", "w");

	if (output)
	{
		for (const auto &op : codeContext.mOps)
		{
			fputs((op + "\n").c_str(), output);
		}
	}
	else
	{
		std::cerr << "Couldn't create an otput file\n"; // Should this be an exception
	}
	// Close the file
	fclose(output);
}