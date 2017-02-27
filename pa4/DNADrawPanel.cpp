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
	
	// TODO: Draw histogram, if one has been generated
	if (mFASTAData && mFASTAHistogram)
	{
		std::cout << mFASTAData->GetHeader() << std::endl;
		// Print Header
		dc.DrawText(mFASTAData->GetHeader(), 50, 50);

		// Cycle through hasmaps and generate each amino frequency
				auto & codeToNameMap = mFASTAHistogram->GetCodeToNameMap();
		//		auto const & codonFrequencies = mFASTAHistogram->GetFrequencyTable();

		//		unsigned int x = 50, y = 100;
		//		wxString codonName;
		//		for (auto const & codon : codonFrequencies)
		//		{
		//			dc.SetBrush(*wxRED_BRUSH);
		//			dc.SetPen(*wxBLACK_PEN);
		////			codonName = codeToNameMap;
		///			dc.DrawText()
		//		}

	}
}
