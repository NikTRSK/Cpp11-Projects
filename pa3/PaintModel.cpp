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
	for (auto iter = mShapes.begin(); iter != mShapes.end(); ++iter)
	{
//		iter.d
		(*iter)->Draw((dc));
		// TODO: call draw on each shape. (once draw is implemented)
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

	mPen = *wxBLACK_PEN;
	mBrush = *wxWHITE_BRUSH;
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

const std::shared_ptr<Command>& PaintModel::GetTopInUndo()
{
	return mUndo.top();
}

const std::shared_ptr<Command>& PaintModel::GetTopInRedo()
{
	return mRedo.top();
}

std::shared_ptr<Command> & PaintModel::GetCurrentCommand()
{
	return mCurrentActiveCommand;
}

void PaintModel::ClearRedo()
{
	// Because C++ is stupid
	while(!mRedo.empty())
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
