#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	// Initialize shared pointer to current command
	mCurrentActiveCommand = nullptr;
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
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
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
