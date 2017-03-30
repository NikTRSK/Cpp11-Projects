//
//  ZomFrame.h
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#pragma once
#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "Machine.h"
#include "Traits.h"

class ZomFrame : public wxFrame
{
public:
	ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~ZomFrame();
private:
	void OnExit(wxCommandEvent& event);
	void OnNew(wxCommandEvent& event);
	void OnSimStart(wxCommandEvent& event);
	void OnSimReset(wxCommandEvent& event);
	void OnTurnTimer(wxTimerEvent& event);
	void OnLoadZombie(wxCommandEvent& event);
	void OnLoadSurvivor(wxCommandEvent& event);
	void OnRandomize(wxCommandEvent& event);

	bool ZOMFilesLoaded() const noexcept;
	// If enable = true enables menues. Disables otherwise
	void EnableDisableMenus(bool enable) const noexcept;

	wxDECLARE_EVENT_TABLE();
private:
	// Panel for drawing
	class ZomDrawPanel* mPanel;

	// File menu
	wxMenu* menuFile;

	// Simulation menu
	wxMenu* mSimMenu;

	// Turn timer
	wxTimer* mTurnTimer;
	
	std::unique_ptr<Machine<ZombieTraits>> mZombieMachine;
	std::unique_ptr<Machine<HumanTraits>> mHumanMachine;

	bool mIsActive;
};
