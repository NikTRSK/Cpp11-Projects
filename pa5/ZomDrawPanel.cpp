//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include "Machine.h"
#include "World.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
	EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
	: wxPanel(parent)
{
	
}

void ZomDrawPanel::PaintEvent(wxPaintEvent & evt)
{
	wxPaintDC dc(this);
	Render(dc);
}

void ZomDrawPanel::PaintNow()
{
	wxClientDC dc(this);
	Render(dc);
}

void ZomDrawPanel::Render(wxDC& dc)
{
	// Clear
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	
	// Draw the grid
	DrawGrid(dc);
}

void ZomDrawPanel::DrawGrid(wxDC& dc)
{
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawRectangle(10, 10, 600, 600);
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			dc.DrawRectangle(j * 30 + 10, i * 30 + 10, 30, 30);
		}
	}
	
	// Draw all zombies
	std::cout << "SIZE: " << World::get().GetZombieStates().size() << std::endl;
	for (auto zombie : World::get().GetZombieStates())
	{
		dc.SetBrush(wxBrush(*wxRED_BRUSH));

//		wxPointList *points = new wxPointList(); 
//		int x = 0, y = 0;
//		points->Append(new wxPoint(x * 30 + 10, y * 30 + 40));
//		points->Append(new wxPoint(x * 30 + 25, y * 30 + 10));
//		points->Append(new wxPoint(x * 30 + 40, y * 30 + 40)); // midpoint
//		dc.DrawPolygon(points);
//		zombie.mFacing = MachineState::RIGHT;
		DrawState(zombie, dc);
	}
}

void ZomDrawPanel::DrawState(MachineState& state, wxDC& dc)
{
	// Is this a memory leak????
	wxPointList *points = new wxPointList();

	int x = state.GetX();
	int y = state.GetY();

	std::cout << "Drawing: " << state.mFacing << ", x: " << x << ", y: " << y << std::endl;
	switch (state.mFacing)
	{
	case (MachineState::UP):
		points->Append(new wxPoint(x * 30 + 10, y * 30 + 40));
		points->Append(new wxPoint(x * 30 + 25, y * 30 + 10)); // midpoint
		points->Append(new wxPoint(x * 30 + 40, y * 30 + 40));
		break;
	case (MachineState::RIGHT):
		points->Append(new wxPoint(x*30 + 10, y*30 + 10));
		points->Append(new wxPoint(x*30 + 10, y*30 + 40));
		points->Append(new wxPoint(x*30 + 40, y*30 + 25)); // midpoint
		break;
	case (MachineState::DOWN):
		points->Append(new wxPoint(x * 30 + 10, y * 30 + 10));
		points->Append(new wxPoint(x * 30 + 25, y * 30 + 40)); // midpoint
		points->Append(new wxPoint(x * 30 + 40, y * 30 + 10));
		break;
	case (MachineState::LEFT):
		points->Append(new wxPoint(x * 30 + 40, y * 30 + 10));
		points->Append(new wxPoint(x * 30 + 40, y * 30 + 40));
		points->Append(new wxPoint(x * 30 + 10, y * 30 + 25)); // midpoint
		break;
	default:
		break;
	}
	dc.DrawPolygon(points);
}
