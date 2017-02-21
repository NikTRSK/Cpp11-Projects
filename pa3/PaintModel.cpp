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
	for (auto shape = mShapes.begin(); shape != mShapes.end(); ++shape)
	{
		(*shape)->Draw((dc));
		if (mSelectedShape == *shape)
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
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
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

void PaintModel::UndoShape()
{
	AddShape(mUndoShape.top());
	mUndoShape.push(mRedoShape.top());
	mRedoShape.pop();
}

void PaintModel::RedoShape()
{
	RemoveShape(mRedoShape.top());
	mUndoShape.push(mRedoShape.top());
	mRedoShape.pop();
}

std::stack<std::shared_ptr<Shape>>& PaintModel::GetTopInUndoShape()
{
	return mUndoShape;
}

std::stack<std::shared_ptr<Shape>>& PaintModel::GetTopInRedoShape()
{
	return mRedoShape;
}

void PaintModel::AddSelectedShapeToUndoStack()
{
	mUndoShape.push(GetSelectedShape());
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
	mPen.SetWidth(size);
}

void PaintModel::SetPenColor(const wxColour& color)
{
	mPen.SetColour(color);
}

void PaintModel::SetBrushColor(const wxColour& color)
{
	mBrush.SetColour(color);
}

std::shared_ptr<Shape>& PaintModel::GetSelectedShape()
{
	return mSelectedShape;
}

bool PaintModel::SelectShape(wxPoint point)
{
	for (auto shape = mShapes.rbegin(); shape != mShapes.rend(); ++shape)
	{
		if ((*shape)->Intersects(point))
		{
			mSelectedShape = *shape;
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
