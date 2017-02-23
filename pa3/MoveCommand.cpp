#include "MoveCommand.h"
#include <iostream>


MoveCommand::MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
	: Command(start, shape)
{
}

MoveCommand::~MoveCommand()
{
}

void MoveCommand::Update(const wxPoint & newPoint)
{
	mShape->SetOffset(newPoint - mStartPoint);
}

void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mOldOffset = mShape->GetCumulativeOffset();
	mShape->AddToCumulativeOffset(mShape->GetOffset());
	mShape->SetOffset(wxPoint(0, 0));
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();
}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->Undo();
	RedoOffset();
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();
	RedoOffset();
}

void MoveCommand::RedoOffset()
{
	wxPoint tempOffset = mShape->GetCumulativeOffset();
	mShape->SetCumulativeOffset(mOldOffset);
	mOldOffset = tempOffset;
}
