//
//  DNADrawPanel.h
//  DNAwx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <string>
#include <memory>
#include "FASTAHistogram.h"

class DNADrawPanel : public wxPanel
{
public:
	DNADrawPanel(wxFrame* parent);
	void PaintNow();
	void AddData(std::shared_ptr<FASTA> FASTAData, std::shared_ptr<FASTAHistogram> FASTAHistogram);

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
 
	DECLARE_EVENT_TABLE()
	
public:
	// Variables here
	std::shared_ptr<FASTA> mFASTAData;
	std::shared_ptr<FASTAHistogram> mFASTAHistogram;
};

