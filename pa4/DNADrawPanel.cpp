//
//  DNADrawPanel.cpp
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "DNADrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <array>
#include <sstream>
#include <iomanip>
#include <utility>

BEGIN_EVENT_TABLE(DNADrawPanel, wxPanel)
	EVT_PAINT(DNADrawPanel::PaintEvent)
END_EVENT_TABLE()

DNADrawPanel::DNADrawPanel(wxFrame* parent)
: wxPanel(parent)
{
	
}

void DNADrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void DNADrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void DNADrawPanel::AddData(std::shared_ptr<FASTA> FASTAData, std::shared_ptr<FASTAHistogram> FASTAHistogram)
{
	mFASTAData = FASTAData;
	mFASTAHistogram = FASTAHistogram;
}

void DNADrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	if (mFASTAData && mFASTAHistogram)
	{
		std::cout << mFASTAData->GetHeader() << std::endl;
		// Print Header
		dc.DrawText(mFASTAData->GetHeader(), 50, 15);

		// Cycle through hasmaps and generate each amino frequency
		auto & codeToNameMap = mFASTAHistogram->GetCodeToNameMap();
		auto const & codonFrequencies = mFASTAHistogram->GetFrequencyTable();

		unsigned int x = 50, y = 60;
		wxString codonName;
		// Find the max codon value. Used for drawing the rectangles
		auto maxCodon = std::max_element(codonFrequencies.begin(), codonFrequencies.end(),
			[](const std::pair<int, int>& c1, const std::pair<int, int>& c2) {
			return c1.second < c2.second; });

		std::vector<wxBrush>brushColors = { *wxRED_BRUSH, *wxGREEN_BRUSH, *wxBLUE_BRUSH };
		std::vector<wxColour>textColor = { *wxRED, *wxGREEN, *wxBLUE };
		unsigned int id = 0;

		float percentage;
		// Max size of the bar
		float barMaxWidth = 800.0 / (maxCodon->second);
		dc.SetPen(*wxBLACK_PEN);
		for (auto const & codon : codonFrequencies)
		{
			// Set brush and text colors
			dc.SetTextForeground(textColor[id]);
			dc.SetBrush(brushColors[id++]);
			if (id == 3)
				id = 0;

			percentage = (static_cast<float>(codon.second) / mFASTAHistogram->GetTotalAminoCount()) * 100;
			codonName = codeToNameMap.find(codon.first)->second + ": " 
						+ ToStringWithPrecision(percentage)
						+ "% (" + std::to_string(codon.second) + ")";
			dc.DrawText(codonName, x, y + 10);
			float barWidth = barMaxWidth * (codon.second);
			dc.DrawRectangle(x + 150, y, barWidth, 30);
			y += 30;
		}
	}
}
