#include "SetBrushCommand.h"

SetBrushCommand::SetBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}

SetBrushCommand::~SetBrushCommand()
{
}

void SetBrushCommand::Update(const wxPoint& newPoint)
{
}

void SetBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mOldBrush = model->GetBrush();
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();

	// If the shape is selected paint it
	if (model->GetSelectedShape())
	{
		mShape = model->GetSelectedShape();
		mShape->SetBrushColor(model->GetBrush().GetColour());
	}
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->Undo();
	// Only Paint a shape if there is one.
	// Makes sure we can also undo colors without objects.
	if (mShape)
	{
		mShape->SetBrushColor(mOldBrush.GetColour());
	}
	mCurrBrush = model->GetBrush();
	model->SetBrushColor(mOldBrush.GetColour());
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
	if (mShape)
	{
		mShape->SetBrushColor(mCurrBrush.GetColour());
	}
	model->SetBrushColor(mCurrBrush.GetColour());
}