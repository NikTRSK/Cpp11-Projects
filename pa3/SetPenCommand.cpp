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
	mOldPen = model->GetPen();
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();

	// If a shape is selected add change the color and save the old color
	if (model->GetSelectedShape())
	{
		mShape = model->GetSelectedShape();
		mShape->SetPenColor(model->GetPen().GetColour());
		mShape->SetPenWidth(model->GetPenWidth());
	}
}

void SetPenCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->Undo();
	if (mShape)
	{
		mShape->SetPenColor(mOldPen.GetColour());
		mShape->SetPenWidth(mOldPen.GetWidth());
	}
	mCurrPen = model->GetPen();
	model->SetPenColor(mOldPen.GetColour());
	model->SetPenWidth(mOldPen.GetWidth());
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
	if (mShape)
	{
		mShape->SetPenColor(mCurrPen.GetColour());
		mShape->SetPenWidth(mCurrPen.GetWidth());
	}
	model->SetPenColor(mCurrPen.GetColour());
	model->SetPenWidth(mCurrPen.GetWidth());
}