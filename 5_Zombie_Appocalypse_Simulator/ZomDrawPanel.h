//
//  ZomDrawPanel.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>

struct MachineState;

class ZomDrawPanel : public wxPanel
{

public:
	ZomDrawPanel(wxFrame* parent);
	void PaintNow();

protected:
	void PaintEvent(wxPaintEvent & evt);
	void Render(wxDC& dc);
	void DrawGrid(wxDC& dc);
	// Draw a zombie/human
	void DrawState(const MachineState& state, wxDC& dc) const noexcept;

	DECLARE_EVENT_TABLE()

public:
	std::string mZombieFile;
	std::string mHumanFile;
	int mMonth;
};

