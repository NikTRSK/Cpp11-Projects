//
//  ZomFrame.cpp
//  zombiewx
//
//  Created by Sanjay Madhav on 12/27/14.
//  Copyright (c) 2014 Sanjay Madhav. All rights reserved.
//

#include "ZomFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
//#include <wx/sizer.h>
#include <wx/filedlg.h>
#include "ZomDrawPanel.h"
#include "World.h"

enum
{
	ID_SImSTART = 1000,
	ID_SIM_RESET,
	ID_TURN_TIMER,
	ID_LOAD_ZOMBIE,
	ID_LOAD_SURVIVOR,
	ID_RANDOMIZE
};

wxBEGIN_EVENT_TABLE(ZomFrame, wxFrame)
EVT_MENU(wxID_EXIT, ZomFrame::OnExit)
EVT_MENU(wxID_NEW, ZomFrame::OnNew)
EVT_MENU(ID_SImSTART, ZomFrame::OnSimStart)
EVT_MENU(ID_SIM_RESET, ZomFrame::OnSimReset)
EVT_TIMER(ID_TURN_TIMER, ZomFrame::OnTurnTimer)
EVT_MENU(ID_LOAD_ZOMBIE, ZomFrame::OnLoadZombie)
EVT_MENU(ID_LOAD_SURVIVOR, ZomFrame::OnLoadSurvivor)
EVT_MENU(ID_RANDOMIZE, ZomFrame::OnRandomize)
wxEND_EVENT_TABLE()

ZomFrame::ZomFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
	, mIsActive(false)
{
	// File menu
	menuFile = new wxMenu;
	menuFile->Append(wxID_NEW);
	menuFile->Append(wxID_EXIT);

	// Simulation menu
	mSimMenu = new wxMenu;
	mSimMenu->Append(ID_LOAD_ZOMBIE, "Load Zombie File");
	mSimMenu->Append(ID_LOAD_SURVIVOR, "Load Survivor File");
	mSimMenu->Append(ID_SImSTART, "Start/stop\tSpace", "Start or stop the simulation");
	mSimMenu->Append(ID_SIM_RESET, "Reset sim");
	mSimMenu->Append(ID_RANDOMIZE, "Randomize", "Randomly generate zombies/humans");

	wxMenuBar* menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(mSimMenu, "&Simulation");
	SetMenuBar(menuBar);
	CreateStatusBar();

	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new ZomDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);

	SetSizer(sizer);
	SetAutoLayout(true);
	Show(true);

	mTurnTimer = new wxTimer(this, ID_TURN_TIMER);
	
	// Disable sim menu on start
	EnableDisableMenus(false);
}

ZomFrame::~ZomFrame()
{
	delete mTurnTimer;
}

void ZomFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void ZomFrame::OnNew(wxCommandEvent& event)
{
	// TODO: Add code for File>New
	mTurnTimer->Stop();
	mPanel->mMonth = 0;
	World::get().ClearData();
	mPanel->PaintNow();

	mZombieMachine = nullptr;
	mHumanMachine = nullptr;
	mPanel->mZombieFile.clear();
	mPanel->mHumanFile.clear();
}

void ZomFrame::OnSimStart(wxCommandEvent& event)
{
	//	World::get().AddZombie(mZombieMachine);
	if (!mIsActive)
	{
		// Add timer to run once per second
		mTurnTimer->Start(1000);
		mIsActive = true;
	}
	else
	{
		mTurnTimer->Stop();
		mIsActive = false;
	}
	mPanel->PaintNow();
}

void ZomFrame::OnSimReset(wxCommandEvent& event)
{
	mTurnTimer->Stop();
	mPanel->mMonth = 0;
	World::get().ClearData();
	mPanel->PaintNow();
}

void ZomFrame::OnTurnTimer(wxTimerEvent& event)
{
	World::get().UpdateWorld();
	mPanel->PaintNow();
}

void ZomFrame::OnLoadZombie(wxCommandEvent& event)
{
	wxFileDialog fileDlg(this, "Load a Zombie file...",
		"./zom", "", "ZOM Files|*.zom",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (fileDlg.ShowModal() == wxID_OK)
	{
		std::cout << "Loading file.\n";
		// EXCEPTION ?
		mPanel->mZombieFile = fileDlg.GetFilename().ToStdString();
		mZombieMachine = std::make_unique<Machine<ZombieTraits>>();
		mZombieMachine->LoadMachine(fileDlg.GetPath().ToStdString());
	}

	if (ZOMFilesLoaded())
	{
		EnableDisableMenus(true);
	}
}

void ZomFrame::OnLoadSurvivor(wxCommandEvent& event)
{
	wxFileDialog fileDlg(this, "Load a Survivor file...",
		"./zom", "", "ZOM Files|*.zom",
		wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (fileDlg.ShowModal() == wxID_OK)
	{
		std::cout << "Loading file.\n";
		// EXCEPTION ?
		mPanel->mHumanFile = fileDlg.GetFilename().ToStdString();
		mHumanMachine = std::make_unique<Machine<HumanTraits>>();
		mHumanMachine->LoadMachine(fileDlg.GetPath().ToStdString());
	}

	if (ZOMFilesLoaded())
	{
		EnableDisableMenus(true);
	}
}

void ZomFrame::OnRandomize(wxCommandEvent& event)
{
	World::get().ClearData();
	World::get().GenerateZombies(*mZombieMachine);
	World::get().GenerateHumans(*mHumanMachine);
}

bool ZomFrame::ZOMFilesLoaded() const noexcept
{
	return !mPanel->mHumanFile.empty() && !mPanel->mZombieFile.empty();
}

void ZomFrame::EnableDisableMenus(bool enable) const noexcept
{
	mSimMenu->Enable(ID_SImSTART, enable);
	mSimMenu->Enable(ID_SIM_RESET, enable);
	mSimMenu->Enable(ID_RANDOMIZE, enable);
	menuFile->Enable(wxID_NEW, enable); // Do we need to enable/disable this
}
