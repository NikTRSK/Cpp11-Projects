#include "SetPenCommand.h"



SetPenCommand::SetPenCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}


SetPenCommand::~SetPenCommand()
{
}

void SetPenCommand::Update(const wxPoint& newPoint)
{
}

void SetPenCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mUndoPen.push(model->GetPen());
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();

	// If a shape is selected add change the color and save teh old color
	if (model->GetSelectedShape())
	{
//		model->GetSelectedShape()->AddToUndoPenStack(); //model->GetPen()
		mShape = model->GetSelectedShape();
		mShape->SetPenColor(model->GetPen().GetColour());
		mShape->SetPenWidth(model->GetPenWidth());
	}
}

void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->SetPenColor(mUndoPen.top().GetColour());
	model->SetPenWidth(mUndoPen.top().GetWidth());
	mRedoPen.push(mUndoPen.top());
	mUndoPen.pop();
	model->Undo();
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->SetPenColor(mRedoPen.top().GetColour());
	model->SetPenWidth(mRedoPen.top().GetWidth());
	mUndoPen.push(mRedoPen.top());
	mRedoPen.pop();
	model->Redo();
}

void SetPenCommand::ClearStacks()
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
