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
//	model->AddCurrentPenToPenUndoStack();
	// Pen is added to the PenUndoStack in PaintModel before the command is created
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
	model->Undo();
	mShape->SetPenColor(model->GetTopInPenUndo().GetColour());
	mShape->SetPenWidth(model->GetTopInPenUndo().GetWidth());
	model->UndoPen();
}

void SetPenCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
	mShape->SetPenColor(model->GetTopInPenRedo().GetColour());
	mShape->SetPenWidth(model->GetTopInPenRedo().GetWidth());
	model->RedoPen();
}