#pragma once
#include "MiniCppUnit.hpp"
#include "RleData.h"
#include <iostream>
#include <iomanip>

// Helper functions
bool buffersAreSame(const char* expected, const char* actual, size_t size);
void runCompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);
void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize);

// IMPORTANT NOTE --
// The tests all use "sizeof" to find the size of the input
// buffer. However, remember that this only works because the
// character arrays are statically allocated.
// If you just have a pointer to a C-style string, "sizeof"
// will return the size of the pointer type (either 4 or 8).
class CompressionTests : public TestFixture<CompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(CompressionTests, "Testing Compression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive runs test");
		// TODO: Add more Compression test cases
		TEST_CASE_DESCRIBE(testBasicNegativeRuns, "Basic negative runs test");
		TEST_CASE_DESCRIBE(testOverMaxPositiveRuns, "Over max length positive runs test");
		TEST_CASE_DESCRIBE(testOverMaxNegativeRuns, "Over max length negative runs test");
		TEST_CASE_DESCRIBE(testAlternatingRuns, "Alternating runs test");
		TEST_CASE_DESCRIBE(testLengthOneRuns, "String length one test");
		TEST_CASE_DESCRIBE(testOverMaxAlternatingRuns1, "Over max length alternating char test (end with positive run)");
		TEST_CASE_DESCRIBE(testOverMaxAlternatingRuns2, "Over max length alternating char test (end with negative run)");
		TEST_CASE_DESCRIBE(testOverMaxSingleUniqueStirngRuns, "Over max length single char test");
		// Normal single unique string
		// Normal single char positive
		// Non letters
		// Same repetition negative
		// Different repetion negative
		// Alternating runs over max
		// 0 length input
		// alternating run negative run at the end

		// VERIFY NEGATIVE RUNS 2s COMPLEMENT
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "aaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbc"
			"ccdddaaabbbcccdddaaabbbcccdddaaabbbcccdddaaabbbcccddd";
		char expected[] = "\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d"
			"\x03" "a" "\x03" "b" "\x03" "c" "\x03" "d";
		
		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testBasicNegativeRuns()
	{
		char test[] = "abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakeh"
			"tfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn";

		char expected[] = "\xa0" "abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testOverMaxPositiveRuns()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"cccccccccccccccccccccccccccccccccccccccccccccccccc"
			"cccccccccccccccccccccccccccccccccccccccccccccccccc"
			"cccccccccccccccccccccccccccc";
		char expected[] = "\x7f" "a" "\x3b" "a" "\x7f" "b" "\x7f" "c" "\x01" "c";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
	
	void testOverMaxNegativeRuns()
	{
		char test[] = "abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakeh"
			"tfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsg";

		char expected[] = "\x81" "abcdenbhawqpbnakehtfnajsgabcdenbha"
			"wqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbn"
			"akehtfnabcdenbhawqpbnakehtfnajsgabcden"
			"\x81" "bhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcd"
			"enbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnabcdenbhawqpb"
			"nakehtfnajsgabcdenbhawqp" "\xc1" "bnakehtfnajsgabcdenbh"
			"awqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
	
	void testAlternatingRuns()
	{
		char test[] = "aaabbbcccccdddddabqwertyuiopnnnnnnnnnnnnnnnnnnnn"
			"kkkkkkkkkkkkkkkkkkkkkkkkkabcdeabcdeqqqqq";

		char expected[] = "\x03" "a" "\x03" "b" "\x05" "c" "\x05" "d"
			"\xf4" "abqwertyuiop" "\x14" "n" "\x19" "k"
			"\xf6" "abcdeabcde" "\x05" "q";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testLengthOneRuns()
	{
		char test[] = "w";
		char expected[] = "\x01" "w";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testOverMaxAlternatingRuns1()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakeh"
			"tfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsg"

			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbb"

			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakeh"
			"tfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsg"

			"cccccccccccccccccccccccccccccccccccccccccccccccccc"
			"cccccccccccccccccccccccccccccccccccccccccccccccccc"
			"cccccccccccccccccccccccccccc";

		char expected[] = "\x7f" "a" "\x3b" "a" 

			"\x81" "abcdenbhawqpbnakehtfnajsgabcdenbha"
			"wqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbn"
			"akehtfnabcdenbhawqpbnakehtfnajsgabcden"
			"\x81" "bhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcd"
			"enbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnabcdenbhawqpb"
			"nakehtfnajsgabcdenbhawqp" "\xc1" "bnakehtfnajsgabcdenbh"
			"awqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			
			"\x7f" "b" "\x7f" 
			
			"\x81" "abcdenbhawqpbnakehtfnajsgabcdenbha"
			"wqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbn"
			"akehtfnabcdenbhawqpbnakehtfnajsgabcden"
			"\x81" "bhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcd"
			"enbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnabcdenbhawqpb"
			"nakehtfnajsgabcdenbhawqp" "\xc1" "bnakehtfnajsgabcdenbh"
			"awqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"

			"c" "\x01" "c";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testOverMaxAlternatingRuns2()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakeh"
			"tfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsg"

			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
			"bbbbbbbbbbbbbbbbbbbbbbbbbbb"

			"cccccccccccccccccccccccccccccccccccccccccccccccccc"
			"cccccccccccccccccccccccccccccccccccccccccccccccccc"
			"cccccccccccccccccccccccccccc"

			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakeh"
			"tfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfn"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"
			"abcdenbhawqpbnakehtfnajsg";

		char expected[] = "\x7f" "a" "\x3b" "a"

			"\x81" "abcdenbhawqpbnakehtfnajsgabcdenbha"
			"wqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbn"
			"akehtfnabcdenbhawqpbnakehtfnajsgabcden"
			"\x81" "bhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcd"
			"enbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnabcdenbhawqpb"
			"nakehtfnajsgabcdenbhawqp" "\xc1" "bnakehtfnajsgabcdenbh"
			"awqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg"

			"\x7f" "b" "\x7f"

			"c" "\x01" "c"

			"\x81" "abcdenbhawqpbnakehtfnajsgabcdenbha"
			"wqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcdenbhawqpbn"
			"akehtfnabcdenbhawqpbnakehtfnajsgabcden"
			"\x81" "bhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsgabcd"
			"enbhawqpbnakehtfnajsgabcdenbhawqpbnakehtfnabcdenbhawqpb"
			"nakehtfnajsgabcdenbhawqp" "\xc1" "bnakehtfnajsgabcdenbh"
			"awqpbnakehtfnajsgabcdenbhawqpbnakehtfnajsg";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}

	void testOverMaxSingleUniqueStirngRuns()
	{
		char test[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
			"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
		char expected[] = "\x7f" "a" "\x7f" "a" "\x7f" "a" "\x7f" "a"
			"\x7f" "a" "\x7f" "a" "\x7f" "a" "\x29" "a";

		runCompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
};

class DecompressionTests : public TestFixture<DecompressionTests>
{
public:
	TEST_FIXTURE_DESCRIBE(DecompressionTests, "Testing Decompression...")
	{
		TEST_CASE_DESCRIBE(testBasicPositiveRuns, "Basic positive run test");
		// TODO: Add more Decompression test  cases
	}
	
	void testBasicPositiveRuns()
	{
		char test[] = "\x28" "x";
		char expected[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		
		runDecompressionTest(test, sizeof(test) - 1, expected, sizeof(expected) - 1);
	}
};

REGISTER_FIXTURE(CompressionTests);
REGISTER_FIXTURE(DecompressionTests);

bool buffersAreSame(const char* expected, const char* actual, size_t size)
{
	if (actual == nullptr)
	{
		std::cerr << std::endl << "Actual data is null." << std::endl;
		return false;
	}

	bool retVal = true;

	for (size_t i = 0; i < size; i++)
	{
		if (expected[i] != actual[i])
		{
			retVal = false;
			std::cerr << std::endl
				<< "Expected 0x" << std::hex << static_cast<unsigned>(expected[i])
				<< " at index " << std::dec << i << " but saw 0x"
				<< std::hex << static_cast<unsigned>(actual[i]) << std::endl;
			break;
		}
	}

	return retVal;
}

void runCompressionTest(char* test, size_t testSize, 
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Compress(test, testSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}

void runDecompressionTest(char* test, size_t testSize,
	char* expected, size_t expectedSize)
{
	RleData r;
	r.Decompress(test, testSize, expectedSize);
	ASSERT_TEST_MESSAGE(buffersAreSame(expected, r.mData, expectedSize),
		"Buffer mismatch (see above for details)");
}
