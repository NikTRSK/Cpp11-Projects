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

void OptimizeGoTo(CodeContext & codeContext) noexcept;
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
	for (const auto &gt : codeContext.mGoto)
	{
		codeContext.mOps.at(gt.first - 1) = "goto,1";
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