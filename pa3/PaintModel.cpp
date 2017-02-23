#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	// Initialize shared pointer to current command
	mCurrentActiveCommand = nullptr;

	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
	// TODO
	// Draw an imported image
	if (mImage.IsOk())
	{
		dc.DrawBitmap(mImage, 0, 0);
	}
	// Redraw all shapes
	// If none of the shapes are selected reset the selection
	bool resetSelected = true;
	for (auto const &shape : mShapes)
	{
		shape->Draw(dc);
		if (mSelectedShape == shape)
		{
			resetSelected = false;

			// Show the selection around the shape
			if (showSelection)
			{
				mSelectedShape->DrawSelection(dc);
			}
		}
		// TODO: call draw on each shape. (once draw is implemented)
	}
	// Reset selection if needed
	if (resetSelected && showSelection)
	{
		mSelectedShape.reset();
	}

}

// Clear the current paint model and start fresh
void PaintModel::New()
{
	// TODO
	// Clear all the data
	mShapes.clear();
	mCurrentActiveCommand.reset();

	ClearRedo();
	ClearUndo();

	// Clear Pen/Brush Stacks
	ClearBrushStacks();
	ClearPenStacks();

	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;

	mSelectedShape.reset();
	mImage = wxBitmap();
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	shape->SetPenColor(GetPen().GetColour());
	shape->SetBrushColor(GetBrush().GetColour());
	shape->SetPenWidth(GetPenWidth());
	mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto foundShape = std::find(mShapes.begin(), mShapes.end(), shape);
	if (foundShape != mShapes.end())
	{
		mShapes.erase(foundShape);
	}
}

bool PaintModel::HasActiveCommand()
{
	return (mCurrentActiveCommand != nullptr);
}

void PaintModel::CreateCommand(CommandType commandType, const wxPoint& start)
{
	// Create the command using the command factory
	mCurrentActiveCommand = CommandFactory::Create(shared_from_this(), commandType, start);
}

void PaintModel::UpdateCommand(const wxPoint& newPoint)
{
	mCurrentActiveCommand->Update(newPoint);
}

void PaintModel::FinalizeCommand()
{
	// TODO: not sure if this is what I'm supposed to be doing
	mCurrentActiveCommand->Finalize(shared_from_this());
	mCurrentActiveCommand.reset();
}

void PaintModel::AddCurrentCommandToUndoStack()
{
	mUndo.push(mCurrentActiveCommand);
}

bool PaintModel::CanUndo()
{
	return !mUndo.empty();
}

bool PaintModel::CanRedo()
{
	return !mRedo.empty();
}

void PaintModel::Undo()
{
	mRedo.push(mUndo.top());
	mUndo.pop();
}

void PaintModel::Redo()
{
	mUndo.push(mRedo.top());
	mRedo.pop();
}

std::shared_ptr<Command>& PaintModel::GetTopInUndo()
{
	return mUndo.top();
}

std::shared_ptr<Command>& PaintModel::GetTopInRedo()
{
	return mRedo.top();
}

void PaintModel::UndoBrush()
{
	mRedoBrush.push(GetBrush());
	SetBrushColor(mUndoBrush.top().GetColour());
	wxString wtf = mUndoBrush.top().GetColour().GetAsString();
//	mRedoBrush.push(mUndoBrush.top());
	mUndoBrush.pop();
}

void PaintModel::RedoBrush()
{
	mUndoBrush.push(GetBrush());
	// Try doing it with GetShape on Command
	SetBrushColor(mRedoBrush.top().GetColour());
	wxString wtf = mRedoBrush.top().GetColour().GetAsString();
//	mUndoBrush.push(mRedoBrush.top());
	mRedoBrush.pop();
}

void PaintModel::ClearBrushStacks()
{
	// Because C++ is stupid
	while (!mUndoBrush.empty())
	{
		mUndoBrush.pop();
	}
	// Because C++ is stupid
	while (!mRedoBrush.empty())
	{
		mRedoBrush.pop();
	}
}

void PaintModel::AddCurrentBrushToBrushUndoStack()
{
	mUndoBrush.push(GetBrush());
	wxString wtf = mUndoBrush.top().GetColour().GetAsString();
}

wxBrush & PaintModel::GetTopInBrushUndo()
{
	return mUndoBrush.top();
}

wxBrush & PaintModel::GetTopInBrushRedo()
{
	return mRedoBrush.top();
}

void PaintModel::AddCurrentPenToPenUndoStack()
{
	mUndoPen.push(GetPen());
}

wxPen& PaintModel::GetTopInPenUndo()
{
	return mUndoPen.top();
}

void PaintModel::UndoPen()
{
	mRedoPen.push(GetPen());
	SetPenColor(mUndoPen.top().GetColour());
	SetPenWidth(mUndoPen.top().GetWidth());
//	mRedoPen.push(mUndoPen.top());
	mUndoPen.pop();
}

void PaintModel::RedoPen()
{
	SetPenColor(mRedoPen.top().GetColour());
	SetPenWidth(mRedoPen.top().GetWidth());
	mUndoPen.push(mRedoPen.top());
	mRedoPen.pop();
}

void PaintModel::ClearPenStacks()
{
	// Because C++ is stupid
	while (!mUndoPen.empty())
	{
		mUndoPen.pop();
	}
	// Because C++ is stupid
	while (!mRedoPen.empty())
	{
		mRedoPen.pop();
	}
}

wxPen& PaintModel::GetTopInPenRedo()
{
	return mRedoPen.top();
}

std::shared_ptr<Command> & PaintModel::GetCurrentCommand()
{
	return mCurrentActiveCommand;
}

void PaintModel::ClearRedo()
{
	// Because C++ is stupid
	while (!mRedo.empty())
	{
		mRedo.pop();
	}
}

void PaintModel::ClearUndo()
{
	// Because C++ is stupid
	while (!mUndo.empty())
	{
		mUndo.pop();
	}
}

std::vector<std::shared_ptr<Shape>> & PaintModel::GetShapes()
{
	return mShapes;
}

int PaintModel::GetPenWidth()
{
	return mPen.GetWidth();
}

const wxPen& PaintModel::GetPen()
{
	return mPen;
}

const wxBrush& PaintModel::GetBrush()
{
	return mBrush;
}

void PaintModel::SetPenWidth(const int& size)
{
	// If shape selected update it with new width
	if (mSelectedShape)
	{
		mSelectedShape->SetPenWidth(size);
	}
	mPen.SetWidth(size);
}

void PaintModel::SetPenColor(const wxColour& color)
{
	// If shape selected update it with new color
	if (mSelectedShape)
	{
		mSelectedShape->SetPenColor(color);
	}
	mPen.SetColour(color);
}

void PaintModel::SetBrushColor(const wxColour& color)
{
	// If shape selected update it with new color
	if (mSelectedShape)
	{
		mSelectedShape->SetBrushColor(color);
	}
	mBrush.SetColour(color);
}

std::shared_ptr<Shape>& PaintModel::GetSelectedShape()
{
	return mSelectedShape;
}

bool PaintModel::SelectShape(wxPoint point)
{
	for (auto const &shape: mShapes)
	{
		if (shape->Intersects(point))
		{
			mSelectedShape = shape;
			return true;
		}
		mSelectedShape.reset();
	}
	return false;
}

void PaintModel::Export(wxString fname, wxSize imageSize)
{
	wxString fileExtension = fname.substr(fname.find_last_of("."));

	wxBitmapType imageType;

	if (fileExtension == ".png")
	{
		imageType = wxBITMAP_TYPE_PNG;
	}
	else if (fileExtension == ".bmp")
	{
		imageType = wxBITMAP_TYPE_BMP;
	}
	else if (fileExtension == ".jpeg" || fileExtension == ".jpg")
	{
		imageType = wxBITMAP_TYPE_JPEG;
	}

	wxBitmap image;
	// Create the bitmap of the specified wxSize
	image.Create(imageSize);
	// Create a memory DC to draw to the bitmap
	wxMemoryDC dc(image);
	// Clear the background color
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	// Draw all the shapes (make sure not the selection!)
	DrawShapes(dc, false);
	// Write the bitmap with the specified file name and wxBitmapType
	image.SaveFile(fname, imageType);
}

void PaintModel::Import(const wxString& fname)
{
	New();
	wxString fileExtension = fname.substr(fname.find_last_of("."));

	wxBitmapType imageType;

	if (fileExtension == ".png")
	{
		imageType = wxBITMAP_TYPE_PNG;
	}
	else if (fileExtension == ".bmp")
	{
		imageType = wxBITMAP_TYPE_BMP;
	}
	else if (fileExtension == ".jpeg" || fileExtension == ".jpg")
	{
		imageType = wxBITMAP_TYPE_JPEG;
	}

	mImage.LoadFile(fname, imageType);
}
