//
//  ZomDrawPanel.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomDrawPanel.h"
#include <wx/dcclient.h>
#include "Machine.h"
#include "World.h"

BEGIN_EVENT_TABLE(ZomDrawPanel, wxPanel)
EVT_PAINT(ZomDrawPanel::PaintEvent)
END_EVENT_TABLE()

ZomDrawPanel::ZomDrawPanel(wxFrame* parent)
	: wxPanel(parent)
{
	mMonth = 0;
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

	// Draw the grid.
	for (int pos = 40; pos < 610; pos += 30)
	{
		dc.DrawLine(pos, 10, pos, 610);
		dc.DrawLine(10, pos, 610, pos);
	}

	// Draw all zombies
	for (auto zombie : World::get().GetZombies())
	{
		dc.SetBrush(wxBrush(*wxRED_BRUSH));
		DrawState(*zombie, dc);
	}

	// Draw all humans
	for (auto human : World::get().GetHumans())
	{
		dc.SetBrush(wxBrush(*wxGREEN_BRUSH));
		DrawState(*human, dc);
	}

	// Draw stats
	dc.SetTextForeground(*wxRED);
	dc.DrawText("Zombies", 620, 10);
	dc.DrawText("Program: " + mZombieFile, 620, 30);
	dc.DrawText("Alive: " + wxString::Format(wxT("%i"), World::get().GetZombies().size()), 620, 50);


	dc.SetTextForeground(*wxGREEN);
	dc.DrawText("Humans", 620, 100);
	dc.DrawText("Program: " + mHumanFile, 620, 120);
	dc.DrawText("Alive: " + wxString::Format(wxT("%i"), World::get().GetHumans().size()), 620, 140);

	dc.SetTextForeground(*wxBLACK);
	dc.DrawText("Month: " + wxString::Format(wxT("%i"), mMonth), 620, 210);
	
}

void ZomDrawPanel::DrawState(MachineState& state, wxDC& dc)
{
	wxPointList *points = new wxPointList();

	int x = state.GetX();
	int y = state.GetY();

	switch (state.mFacing)
	{
	case (MachineState::UP):
		points->Append(new wxPoint(x * 30 + 10, y * 30 + 40));
		points->Append(new wxPoint(x * 30 + 25, y * 30 + 10)); // midpoint
		points->Append(new wxPoint(x * 30 + 40, y * 30 + 40));
		break;
	case (MachineState::RIGHT):
		points->Append(new wxPoint(x * 30 + 10, y * 30 + 10));
		points->Append(new wxPoint(x * 30 + 10, y * 30 + 40));
		points->Append(new wxPoint(x * 30 + 40, y * 30 + 25)); // midpoint
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
	delete points;
}
