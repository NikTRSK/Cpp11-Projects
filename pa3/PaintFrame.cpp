#include "PaintFrame.h"
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/toolbar.h>
#include <wx/image.h>
#include <wx/colordlg.h>
#include <wx/textdlg.h>
#include <wx/filedlg.h>
#include "PaintDrawPanel.h"
#include "PaintModel.h"

wxBEGIN_EVENT_TABLE(PaintFrame, wxFrame)
EVT_MENU(wxID_EXIT, PaintFrame::OnExit)
EVT_MENU(wxID_NEW, PaintFrame::OnNew)
EVT_MENU(ID_Import, PaintFrame::OnImport)
EVT_TOOL(ID_Import, PaintFrame::OnImport)
EVT_MENU(ID_Export, PaintFrame::OnExport)
EVT_TOOL(ID_Export, PaintFrame::OnExport)
EVT_MENU(wxID_UNDO, PaintFrame::OnUndo)
EVT_TOOL(wxID_UNDO, PaintFrame::OnUndo)
EVT_MENU(wxID_REDO, PaintFrame::OnRedo)
EVT_TOOL(wxID_REDO, PaintFrame::OnRedo)
EVT_MENU(ID_Unselect, PaintFrame::OnUnselect)
EVT_MENU(ID_Delete, PaintFrame::OnDelete)
EVT_MENU(ID_SetPenColor, PaintFrame::OnSetPenColor)
EVT_MENU(ID_SetPenWidth, PaintFrame::OnSetPenWidth)
EVT_MENU(ID_SetBrushColor, PaintFrame::OnSetBrushColor)
// The different draw modes
EVT_TOOL(ID_Selector, PaintFrame::OnSelectTool)
EVT_TOOL(ID_DrawLine, PaintFrame::OnSelectTool)
EVT_TOOL(ID_DrawEllipse, PaintFrame::OnSelectTool)
EVT_TOOL(ID_DrawRect, PaintFrame::OnSelectTool)
EVT_TOOL(ID_DrawPencil, PaintFrame::OnSelectTool)
wxEND_EVENT_TABLE()

PaintFrame::PaintFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	// Initialize image handlers to support BMP, PNG, JPEG
	wxImage::AddHandler(new wxPNGHandler());
	wxImage::AddHandler(new wxJPEGHandler());

	SetupMenu();

	SetupToolbar();

	SetupModelAndView();

	Show(true);

	SetMinSize(GetSize());
	SetMaxSize(GetSize());

	mMoveCursor = false;
}

void PaintFrame::SetupMenu()
{
	// File menu
	mFileMenu = new wxMenu();
	mFileMenu->Append(wxID_NEW);
	mFileMenu->Append(ID_Export, "Export...",
		"Export current drawing to image file.");
	mFileMenu->AppendSeparator();
	mFileMenu->Append(ID_Import, "Import...",
		"Import image into file.");
	mFileMenu->Append(wxID_EXIT);

	// Edit menu
	mEditMenu = new wxMenu();
	mEditMenu->Append(wxID_UNDO);
	mEditMenu->Append(wxID_REDO);
	mEditMenu->AppendSeparator();
	mEditMenu->Append(ID_Unselect, "Unselect",
		"Unselect the current selection");
	mEditMenu->AppendSeparator();
	mEditMenu->Append(ID_Delete, "Delete\tDel",
		"Delete the current selection");

	mEditMenu->Enable(wxID_UNDO, false);
	mEditMenu->Enable(wxID_REDO, false);
	mEditMenu->Enable(ID_Unselect, false);
	mEditMenu->Enable(ID_Delete, false);

	// Colors menu
	mColorMenu = new wxMenu();
	mColorMenu->Append(ID_SetPenColor, "Pen Color...", "Set the pen color.");
	mColorMenu->Append(ID_SetPenWidth, "Pen Width...", "Set the pen width.");
	mColorMenu->AppendSeparator();
	mColorMenu->Append(ID_SetBrushColor, "Brush Color...", "Set brush color");

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(mFileMenu, "&File");
	menuBar->Append(mEditMenu, "&Edit");
	menuBar->Append(mColorMenu, "&Colors");
	SetMenuBar(menuBar);
	CreateStatusBar();
}

void PaintFrame::SetupToolbar()
{
	// Create the toolbar
	mToolbar = CreateToolBar();
	mToolbar->AddTool(wxID_NEW, "New",
		wxBitmap("Icons/New.png", wxBITMAP_TYPE_PNG),
		"New");
	mToolbar->AddTool(ID_Export, "Export",
		wxBitmap("Icons/Save.png", wxBITMAP_TYPE_PNG),
		"Export");
	mToolbar->AddTool(ID_Import, "Import",
		wxBitmap("Icons/Import.png", wxBITMAP_TYPE_PNG),
		"Import");
	mToolbar->AddSeparator();

	mToolbar->AddTool(wxID_UNDO, "Undo",
		wxBitmap("Icons/Undo.png", wxBITMAP_TYPE_PNG),
		"Undo");
	mToolbar->AddTool(wxID_REDO, "Redo",
		wxBitmap("Icons/Redo.png", wxBITMAP_TYPE_PNG),
		"Redo");
	mToolbar->AddSeparator();

	mToolbar->AddTool(ID_Selector, "Selector",
		wxBitmap("Icons/Cursor.png", wxBITMAP_TYPE_PNG),
		"Selector", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawLine, "Draw Line",
		wxBitmap("Icons/Line.png", wxBITMAP_TYPE_PNG),
		"Draw Line", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawEllipse, "Draw Ellipse",
		wxBitmap("Icons/Ellipse.png", wxBITMAP_TYPE_PNG),
		"Draw Ellipse", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawRect, "Draw Rectangle",
		wxBitmap("Icons/Rectangle.png", wxBITMAP_TYPE_PNG),
		"Draw Rectangle", wxITEM_CHECK);
	mToolbar->AddTool(ID_DrawPencil, "Pencil",
		wxBitmap("Icons/Pencil.png", wxBITMAP_TYPE_PNG),
		"Pencil", wxITEM_CHECK);

	mToolbar->Realize();

	ToggleTool(ID_Selector);

	// Both undo and redo are disabled initially
	mToolbar->EnableTool(wxID_UNDO, false);
	mToolbar->EnableTool(wxID_REDO, false);
}

void PaintFrame::SetupModelAndView()
{
	// Prepare the draw panel and show this frame
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	mPanel = new PaintDrawPanel(this);
	sizer->Add(mPanel, 1, wxEXPAND);

	// Programatically bind the mouse events on the draw panel to us
	mPanel->Bind(wxEVT_LEFT_DOWN, &PaintFrame::OnMouseButton, this);
	mPanel->Bind(wxEVT_LEFT_UP, &PaintFrame::OnMouseButton, this);
	mPanel->Bind(wxEVT_MOTION, &PaintFrame::OnMouseMove, this);

	// Create the model
	mModel = std::make_shared<PaintModel>();
	mPanel->SetModel(mModel);
	SetSizer(sizer);

	SetAutoLayout(true);
}

void PaintFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void PaintFrame::OnNew(wxCommandEvent& event)
{
	mModel->New();
	mPanel->PaintNow();
	EnableUndoRedoMenus();
}

void PaintFrame::OnExport(wxCommandEvent& event)
{
	wxFileDialog
		saveFileDialog(this, _("Save image as"), "", "",
			"PNG files (*.png)|*.png|JPEG files (*.jpeg)|*.jpeg|JPG files (*.jpg)|*.jpg|BMP files (*.bmp)|*.bmp",
			wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
	{
		return;
	}
	mModel->Export(saveFileDialog.GetPath(), mPanel->GetSize());
}

void PaintFrame::OnImport(wxCommandEvent& event)
{
	wxFileDialog
		openFileDialog(this, _("Open an image file"), "", "",
			"PNG files (*.png)|*.png|JPEG files (*.jpeg)|*.jpeg|JPG files (*.jpg)|*.jpg|BMP files (*.bmp)|*.bmp",
			wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	mModel->Import(openFileDialog.GetPath());
	mPanel->PaintNow();
}

void PaintFrame::OnUndo(wxCommandEvent& event)
{
	mModel->GetTopInUndo()->Undo(mModel);
	mPanel->PaintNow();
	// Check if we need to enable/disable menu
	EnableUndoRedoMenus();
}

void PaintFrame::OnRedo(wxCommandEvent& event)
{
	mModel->GetTopInRedo()->Redo(mModel);
	mPanel->PaintNow();
	// Check if we need to enable/disable menu
	EnableUndoRedoMenus();
}

void PaintFrame::OnUnselect(wxCommandEvent& event)
{
	mModel->GetSelectedShape().reset();
	mEditMenu->Enable(ID_Delete, false);
	mEditMenu->Enable(ID_Unselect, false);
	mPanel->PaintNow();
}

void PaintFrame::OnDelete(wxCommandEvent& event)
{
	mModel->CreateCommand(CM_Delete, wxPoint(1, 1));
	mModel->FinalizeCommand();
	mEditMenu->Enable(ID_Delete, false);
	mEditMenu->Enable(ID_Unselect, false);
	SetCursor(CU_Default);
	mPanel->PaintNow();
}

void PaintFrame::OnSetPenColor(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(mModel->GetPen().GetColour());
	wxColourDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK)
	{
		// Use dialog.GetColourData() to get the color and make stuff happen
		mModel->CreateCommand(CM_SetPen, wxPoint(1, 1));
		mModel->FinalizeCommand();
		mModel->SetPenColor(dialog.GetColourData().GetColour()); // Yay API calls
		mPanel->PaintNow(); // Don't forget to wipe
	}
	EnableUndoRedoMenus();
}

void PaintFrame::OnSetPenWidth(wxCommandEvent& event)
{
	wxTextEntryDialog dialog(this, "Enter font size (1-10): ");
	wxTextValidator textValidator(wxFILTER_INCLUDE_LIST);
	wxArrayString input;

	// Why you no initializer list wx, why?
	for (int i = 1; i < 11; i++)
	{
		wxString inString;
		inString << i;
		input.Add(inString);
	}

	textValidator.SetIncludes(input);
	dialog.SetTextValidator(textValidator);

	if (dialog.ShowModal() == wxID_OK)
	{
		// Use dialog.GetColourData() to get the color and make stuff happen
		mModel->CreateCommand(CM_SetPen, wxPoint(1, 1));
		mModel->FinalizeCommand();
		mModel->SetPenWidth(wxAtoi(dialog.GetValue()));
		mPanel->PaintNow(); // Don't forget to wipe
	}
	EnableUndoRedoMenus();
}

void PaintFrame::OnSetBrushColor(wxCommandEvent& event)
{
	wxColourData data;
	data.SetColour(mModel->GetBrush().GetColour());
	wxColourDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK)
	{
		// Use dialog.GetColourData() to get the color and make stuff happen
		mModel->CreateCommand(CM_SetBrush, wxPoint(1, 1));
		mModel->FinalizeCommand();
		mModel->SetBrushColor(dialog.GetColourData().GetColour()); // Yay API calls
		mPanel->PaintNow(); // Don't forget to wipe
	}
	EnableUndoRedoMenus();
}

void PaintFrame::OnMouseButton(wxMouseEvent& event)
{
	if (event.LeftDown())
	{
		// This is when the left mouse button is pressed
		switch (mCurrentTool)
		{
		case ID_DrawLine:
			mModel->CreateCommand(CM_DrawLine, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_DrawRect:
			mModel->CreateCommand(CM_DrawRect, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_DrawEllipse:
			mModel->CreateCommand(CM_DrawEllipse, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_DrawPencil:
			mModel->CreateCommand(CM_DrawPencil, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_SetPenColor:
			mModel->CreateCommand(CM_SetPen, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_SetPenWidth:
			mModel->CreateCommand(CM_SetPen, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_SetBrushColor:
			mModel->CreateCommand(CM_SetBrush, event.GetPosition());
			mPanel->PaintNow();
			break;
		case ID_Selector:
			if (mMoveCursor)
			{
				mModel->CreateCommand(CM_Move, event.GetPosition());
			}
			
			// Enable Unselect/Delete menus if needed
			if (mModel->SelectShape(event.GetPosition()))
			{
				mEditMenu->Enable(ID_Unselect, true);
				mEditMenu->Enable(ID_Delete, true);
			}
			else
			{
				mEditMenu->Enable(ID_Unselect, false);
				mEditMenu->Enable(ID_Delete, false);
			}
			mPanel->PaintNow();
		default: break;
		}
		mModel->ClearRedo();
	}
	else if (event.LeftUp())
	{
		// This is when the left mouse button is released
		if (mModel->HasActiveCommand())
		{
			mModel->UpdateCommand(event.GetPosition());
			mModel->FinalizeCommand();
			mPanel->PaintNow();
		}
	}
	// Enable Redo/Undo menus if needed
	EnableUndoRedoMenus();
}

void PaintFrame::OnMouseMove(wxMouseEvent& event)
{
	// This is when the mouse is moved inside the drawable area
	if (mModel->HasActiveCommand())
	{
		mModel->UpdateCommand(event.GetPosition());
		mPanel->PaintNow();
	}
	
	// Change cursor if its within shape area
	if (mModel->GetSelectedShape())
	{
		if (mModel->GetSelectedShape()->Intersects(event.GetPosition()))
		{
			SetCursor(CU_Move);
			mMoveCursor = true;
		}
		else if (mMoveCursor)
		{
			mMoveCursor = false;
			SetCursor(CU_Default);
		}
	}
}

void PaintFrame::ToggleTool(EventID toolID)
{
	// Deselect everything
	for (int i = ID_Selector; i <= ID_DrawPencil; i++)
	{
		mToolbar->ToggleTool(i, false);
	}

	// Select the new tool
	mToolbar->ToggleTool(toolID, true);

	mCurrentTool = toolID;
}

void PaintFrame::SetCursor(CursorType type)
{
	wxCursor* cursor = mCursors.GetCursor(type);
	if (cursor != nullptr)
	{
		mPanel->SetCursor(*cursor);
	}
}

void PaintFrame::EnableUndoRedoMenus()
{
	// Undo menu
	if (mModel->CanUndo())
	{
		mEditMenu->Enable(wxID_UNDO, true);
		mToolbar->EnableTool(wxID_UNDO, true);
	}
	else
	{
		mEditMenu->Enable(wxID_UNDO, false);
		mToolbar->EnableTool(wxID_UNDO, false);
	}
	// Redo menu
	if (mModel->CanRedo())
	{
		mEditMenu->Enable(wxID_REDO, true);
		mToolbar->EnableTool(wxID_REDO, true);
	}
	else
	{
		mEditMenu->Enable(wxID_REDO, false);
		mToolbar->EnableTool(wxID_REDO, false);
	}

	if (!mModel->GetSelectedShape())
	{
		mEditMenu->Enable(ID_Delete, false);
		mEditMenu->Enable(ID_Unselect, false);
	}
}

void PaintFrame::OnSelectTool(wxCommandEvent& event)
{
	EventID id = static_cast<EventID>(event.GetId());
	ToggleTool(id);

	// Select appropriate cursor
	switch (id)
	{
	case ID_DrawLine:
	case ID_DrawEllipse:
	case ID_DrawRect:
		SetCursor(CU_Cross);
		break;
	case ID_DrawPencil:
		SetCursor(CU_Pencil);
		break;
	default:
		SetCursor(CU_Default);
		break;
	}
}
