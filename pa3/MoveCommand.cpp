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
	mShape->UpdateOffset(newPoint - mStartPoint);
}

void MoveCommand::Finalize(std::shared_ptr<PaintModel> model)
{
	mOldOffset = mShape->mCumulativeOffset;
	mShape->mCumulativeOffset += mShape->GetOffset();
	mShape->mOffset = wxPoint(0, 0);
	model->AddCurrentCommandToUndoStack();
	model->GetCurrentCommand().reset();
}

void MoveCommand::Undo(std::shared_ptr<PaintModel> model)
{
	model->Undo();

	wxPoint tempOffset = mShape->mCumulativeOffset;
	mShape->mCumulativeOffset = mOldOffset;
	mOldOffset = tempOffset;
}

void MoveCommand::Redo(std::shared_ptr<PaintModel> model)
{
	model->Redo();

	wxPoint tempOffset = mShape->mCumulativeOffset;
	mShape->mCumulativeOffset = mOldOffset;
	mOldOffset = tempOffset;
}
