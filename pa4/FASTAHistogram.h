#pragma once
#include <unordered_map>
#include <memory>
#include "FASTA.h"

class FASTAHistogram
{
public:
	FASTAHistogram(std::shared_ptr<FASTA> FASTAFile);
	~FASTAHistogram();
//	const auto & GetFrequencyTable() const;
//	const auto & GetTotalAminoCount() const;
	const std::unordered_map<char, std::string> & GetCodeToNameMap() const;

private:
	std::shared_ptr<FASTA> mFASTAFile;
	std::unordered_map<char, std::string> mCodeToFullNameMap
	= {
		{ 'M', "Methionine" },
		{ 'F', "Phenylalanine" },
		{ 'L', "Leucine" },
		{ 'I', "Isoleucine" },
		{ 'S', "Serine" },
		{ 'T', "Threonine"},
		{ 'Y', "Tyrosine" },
		{ 'C', "Cysteine" },
		{ 'W', "Tryptophan" },
		{ 'P', "Proline" },
		{ 'H', "Histidine" },
		{ 'Q', "Glutamine" },
		{ 'N', "Asparagine"},
		{ 'K', "Lysine" },
		{ 'R', "Arginine" },
		{ 'V', "Valine" },
		{ 'A', "Alanine" },
		{ 'D', "Aspartic acid" },
		{ 'E', "Glutamic acid" },
		{ 'S', "Serine" },
		{ 'R', "Arginine" },
		{ 'G', "Glycine" }
	};
	std::unordered_map<char, unsigned int> mCodonFrequencies
	= {
		{ 'M', 0 },
		{ 'F', 0 },
		{ 'L', 0 },
		{ 'I', 0 },
		{ 'S', 0 },
		{ 'T', 0 },
		{ 'Y', 0 },
		{ 'C', 0 },
		{ 'W', 0 },
		{ 'P', 0 },
		{ 'H', 0 },
		{ 'Q', 0 },
		{ 'N', 0 },
		{ 'K', 0 },
		{ 'R', 0 },
		{ 'V', 0 },
		{ 'A', 0 },
		{ 'D', 0 },
		{ 'E', 0 },
		{ 'S', 0 },
		{ 'R', 0 },
		{ 'G', 0 }		
	};
	unsigned int mTotalAminoCount;
	char mCodonTable[24][4] =
// St| T | C | A | G |
// -----------------------
	{
// 0 | 0 | 0 | 1 | 0 | // WTB start codon
	 { 0,  0,  1,  0 },
// 1 | 2 | 0 | 0 | 0 |
	 { 2,  0,  1,  0 },
// 2 | 0 | 0 | 0 | M | // M and start
	 { 0,  0,  1, 'M' },
// 3 | 4 | 9 | 14 | 19 | // First letter of next amino acid 
	 { 4,  9,  14,  19 },
// 4 | 5 | 6 | 7 | 8 |	// T**
	 { 5,  6,  7,  8 },
// 5 | F | F | L | L |	// TT*
	 { 'F', 'F', 'L', 'L' },
// 6 | S | S | S | S |	// TC*
	 { 'S', 'S', 'S', 'S' },
// 7 | Y | Y | 0 | 0 |	// TA*, 0 = Stop and Goto 0
	 { 'Y', 'Y', 0, 0 },
// 8 | C | C | 0 | W |	// TG*, 0 = Stop and Goto 0
	 { 'C', 'C', 0, 'W' },
// 9 | 10 | 11 | 12 | 13 |	// C**
	 { 10,  11,  12,  13},
// 10 | L | L | L | L |	// CT*
	 { 'L', 'L', 'L', 'L' },
// 11 | P | P | P | P |	// CC*
	 { 'P', 'P', 'P', 'P' },
// 12 | H | H | Q | Q |	// CA*
	 { 'H', 'H', 'Q', 'Q' },
// 13 | R | R | R | R |	// CG*
	 { 'R', 'R', 'R', 'R' },
// 14 | 15 | 16 | 17 | 18 |	// A**
	 { 15, 16, 17, 18 },
// 15 | I | I | I | M |	// AT*
	 { 'I', 'I', 'I', 'M' },
// 16 | T | T | T | T |	// AC*
	 { 'T', 'T', 'T', 'T' },
// 17 | N | N | K | K |	// AA*
	 { 'N', 'N', 'K', 'K' },
// 18 | S | S | R | R |	// AG*
	 { 'S', 'S', 'R', 'R' },
// 19 | 20 | 21 | 22 | 23 |	// G**
	 {  20,  21,  22,  23 },
// 20 | V | V | V | V |	// GT*
	 { 'V', 'V', 'V', 'V' },
// 21 | A | A | A | A |	// GC*
	 { 'A', 'A', 'A', 'A' },
// 22 | D | D | E | E |	// GA*
	 { 'D', 'D', 'E', 'E' },
// 23 | G | G | G | G |	// GG*
	 { 'G', 'G', 'G', 'G' },
	};

	// Convert a Base to index
	char BaseToIdx(char c);
};

