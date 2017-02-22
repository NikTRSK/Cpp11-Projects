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
	mUndoBrush.push(model->GetBrush());
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();

	if (model->GetSelectedShape())
	{
		// add to undo for brush
		mShape = model->GetSelectedShape();
		mShape->SetBrushColor(model->GetBrush().GetColour());
	}
}

void SetBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->SetBrushColor(mUndoBrush.top().GetColour());
	mRedoBrush.push(mUndoBrush.top());
	mUndoBrush.pop();
	model->Undo();
}

void SetBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->SetBrushColor(mRedoBrush.top().GetColour());
	mUndoBrush.push(mRedoBrush.top());
	mRedoBrush.pop();
	model->Redo();
}

void SetBrushCommand::ClearStacks()
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
